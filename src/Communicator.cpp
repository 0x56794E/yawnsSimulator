#include "Communicator.h"
#include "TopologyUtils.h"

using namespace std;

//TODO: KEEP TRACK OF LOCAL AND INTER-PROC comm %

//Keep outbox here to check occasionally
vector<int*> outboxMsg;
vector<MPI_Request> outboxStatus;

int lastEpochCount = 0;
int currentEpochCount = 0;

int totalMsgCount = 0;
int interProcCount = 0;

void newEpoch()
{
	//clear the first lastEpochCount msg in the vector
	int index = 0;
	for (int counter = 0; counter < lastEpochCount; ++counter) //only do 1 pass
    {
		if (done(outboxStatus[index]))
		{
			MPI_Request_free(&outboxStatus[index]);
			outboxStatus.erase(outboxStatus.begin() + index);
			outboxMsg.erase(outboxMsg.begin() + index);	
		}
		else
			++index;
    }

	//Reset the count
    lastEpochCount = currentEpochCount;
	currentEpochCount = 0;
}

int done(MPI_Request req)
{
	MPI_Status stat;
	int flag;
	MPI_Request_get_status(req, &flag, &stat);
	return flag;
}

void sendMsg(map<int, pair<int, int>> &rankMap, Event* updatedEvent)
{
	//Determine the rank of the reciever
	int recvRank = 0;
	for (map<int, pair<int, int>>::const_iterator it = rankMap.begin(); 
			it != rankMap.end(); ++it)
	{
		//rankMap's structure:
		//key: it->first == rank
		//value: it->second == pair of (min, max); eg mypair.first, mypair.second
		pair<int, int> maxmin = it->second;

		if (updatedEvent->getCurrentStopId() >= maxmin.first 
			&& updatedEvent->getCurrentStopId() <= maxmin.second)
		{
			recvRank = it->first;
			break;
		}
	}

	//Convert the event into msg and send
	//1. Turn the event into an array of int
	//Following order: [timestamp, stop_count, stop_passed]
	int* data = new int[MSG_SIZE](); //dynamic!! Delete it somewhere
	data[TIMESTAMP] = updatedEvent->getTimestamp();
	data[STOP_COUNT] = updatedEvent->getStopCount();
	data[STOP_PASSED] = updatedEvent->getStopPassed();
	data[RECV_ID] = updatedEvent->getCurrentStopId();
	data[LAST_NODE_ID] = updatedEvent->getLastNodeId();

	//2. Send off the msg
	MPI_Request req;
	MPI_Isend(data, MSG_SIZE, MPI_INT, recvRank, MSG_TAG, MPI_COMM_WORLD, &req);

    //Save this to free later
	outboxMsg.push_back(data);
	outboxStatus.push_back(req);

	//++currentEpochCount; ==> WTH is this???

	//Free mem occupied by event; no longer needed
	delete updatedEvent;
}

void receiveMsg(MPI_Status status, SE* se)
{
	int recv_buf[MSG_SIZE];
	MPI_Recv(&recv_buf, MSG_SIZE, MPI_INT, status.MPI_SOURCE, MSG_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	//Construct event obj and schedule on appropriate LP
	Event* e = new Event (recv_buf[TIMESTAMP], 
						  recv_buf[STOP_COUNT], 
    					  recv_buf[RECV_ID], 
						  recv_buf[STOP_PASSED], 
						  0, //event type is always arrival == 0
						  recv_buf[LAST_NODE_ID]);

	se->scheduleEvent(e);
}

/**
 *  Determine how many of the total msg are inter-proc
 */
void summarizeMsgCount(int rank)
{
	//Do reduce on total msg
	int gl_total;
	MPI_Reduce(&totalMsgCount, &gl_total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	//Do reduce on interproc
	int gl_interp;
	MPI_Reduce(&interProcCount, &gl_interp, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (!rank)
		printf("\nTotal msg sent = %d ; interproc = %d ; pct = %.5f\n",
				gl_total, gl_interp, gl_interp * 100.0 / gl_total);
}
