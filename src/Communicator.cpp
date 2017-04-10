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

int genNextStop(LPMap lpMap, int curStopId)
{
	//TODO: rand gen next stop
	int nextStopId = 0;

	//Get the current LP
	auto it = lpMap.find(curStopId);
	if (it != lpMap.end()) //TODO: handle not found case
		LP* curStop = it->second;


	return nextStopId;
}

/**
 * Send the msg
 * Determine whether the next stop is local.
 * If local, simply schedule the event
 * Otherwise, send the msg to appropriate proc
 */
void sendMsg(SE* se, Event* event, LPMap &lpMap, int nextStopId, map<int, pair<int, int>> &rankMap)
{
	//Incr the total msg count
	++totalMsgCount;

	//If the LP is NOT on this proc
	//Send the msg to other proc
	//Event has 3 main pc:
	// - timestamp (already updated)
	// - stop_count (unchanged)
	// - stop_passed (already updated)
	if (lpMap.find(nextStopId) == lpMap.end())
	{
		//Incr the inter-proc
		++interProcCount;

		//1. Turn the event into an array of int
		//Following order: [timestamp, stop_count, stop_passed]
		int* data = new int[MSG_SIZE](); //dynamic!! Delete it somewhere
		data[TIMESTAMP] = event->getTimestamp();
		data[STOP_COUNT] = event->getStopCount();
		data[STOP_PASSED] = event->getStopPassed();
		data[HANDLER_ID] = event->getCurrentStopId();

		//2. Send the array
		MPI_Request req;
		int recvRank = 0;

		//Determine the rank of the reciever
		for (map<int, pair<int, int>>::const_iterator it = rankMap.begin(); it != rankMap.end(); ++it)
		{
			//key: it->first
			//value: it->second => pair of (min, max); eg mypair.first, mypair.second
			pair<int, int> maxmin = it->second;

			if (nextStopId >= maxmin.first && nextStopId <= maxmin.second)
			{
				recvRank = it->first;
				break;
			}
		}

		MPI_Isend(data, MSG_SIZE, MPI_INT, recvRank, MSG_TAG, MPI_COMM_WORLD, &req);

        //Save this to free later
		outboxMsg.push_back(data);
		outboxStatus.push_back(req);
		++currentEpochCount;

		//Free mem occupied by event; no longer needed
		delete event;
	}
	else
	{
		event->setCurrentStopId(nextStopId);
		se->scheduleEvent(event);

		//Next stop is on this proc
		//schedule directly
		//lpMap[nextStopId]->scheduleEvent(event);
	}
}

void receiveMsg(MPI_Status status, SE* se)
{
	int recv_buf[MSG_SIZE];
	MPI_Recv(&recv_buf, MSG_SIZE, MPI_INT, status.MPI_SOURCE, MSG_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	//Construct event obj and schedule on appropriate LP
	Event* e = new Event (recv_buf[TIMESTAMP], recv_buf[STOP_COUNT], recv_buf[HANDLER_ID], recv_buf[STOP_PASSED]);

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
