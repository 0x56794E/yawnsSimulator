#include <vector>

#include "Communicator.h"
#include "TopologyUtils.h"

using namespace std;

const int MAX_SIZE = 25; //allow inbox to reach up to 100 msgs.

//Keep outbox here to check occasionally
vector<int*> outboxMsg;
vector<MPI_Request> outboxStatus;
int lastEpochCount = 0;
int currentEpochCount = 0;

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

//TODO: FIX this
void sendMsg(Event* event, LPMap lpMap)
{
	//Determine which LP to send to:
	//TODO: randomly select next's neighbor
	int nextStop = genNextStop(lpMap, event->getCurrentStopId());
	
	if (lpMap.find(nextStop) == lpMap.end())
	{
		//Next stop is NOT on this proc
		//send to other proc
		//1. Turn the event into an array of int
//		int size = event->getRemainStopCount() + 1;
//		int* data =  new int[size]; //(int*)malloc(sizeof(int) * size);
//		data[0] = event->getTimestamp();
//		for (int i = 1; i < size; ++i)
//			data[i] = event->nextStop();
//
//		//2. Send the array
//		MPI_Request req;
//		MPI_Isend(data, size, MPI_INT, getRank(nextStop), MSG_TAG, MPI_COMM_WORLD, &req);
//
//        //Save this to free later
//		outboxMsg.push_back(data);
//		outboxStatus.push_back(req);
//		++currentEpochCount;
	}
	else
	{
		//Next stop is on this proc
		//schedule directly
		lpMap[nextStop]->scheduleEvent(event);
	}
}


void receiveMsg(MPI_Status status, LPMap lpMap)
{
	//determine how many ints to be receiv
	int count;
	MPI_Get_count(&status, MPI_INT, &count);

	//Msg would be an array of int <timestamp of msg> <next stop id => the LP to forward to> <the rest of the stops>
	//TODO: FIX THIS or del??? cuz next stop gen'ed on the fly
	int* number_buf = new int[count]; //(int*)malloc(sizeof(int) * count);
	MPI_Recv(number_buf, count, MPI_INT, status.MPI_SOURCE, MSG_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	//Construct event object & schedule it on appropriate LP
//	Event* event = new Event(number_buf[0]);
//	for (int i = 1; i < count; ++i)
//		event->addStop(number_buf[i]);

//	lpMap[number_buf[1]]->scheduleEvent(event);

	delete [] number_buf;
	//	free(number_buf);
}
