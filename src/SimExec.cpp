#include "stdio.h" //For printf
#include <array> //For array in outbox
#include <cstdlib> //For rand gen
#include "mpi.h" //For mpi stuff

//My stuff
#include "SimExec.h"
#include "util/EventComparator.h"

const int MAX_PACKET_CT = 200; //max num of packets
const int MAX_STOP_CT = 100; //max num of stops
const int LA = 10; 
const int TS_IDX = 0;
const int STCT_IDX = 1;
const int MSG_TAG = 0;


//The future event list
std::priority_queue<Event*, std::vector<Event*>, EventComparator> fel;

SimExec::SimExec(int lpC, int r)
{
	lpCount = lpC;
	rank = r;
	msgCount = new int[lpC];

	int i, ts, stops;
	
	//Init msg count to all 0 for BEFORE the beginning of ea epoch
	for (i = 0; i < lpC; ++i)
		msgCount[i] = 0;		
	
	//Generate the number of packets originiated from this proc
	//and schedule initial events
	int numPacket = rand() % MAX_PACKET_CT;
	for (i = 0; i < numPacket; ++i)
	{
		//Ran gen the time
		ts = rand();
		
		//Ran gen the stop count
		stops = rand() % MAX_STOP_CT;
		
		fel.push(new Event(ts, stops));
	}
}

/**
 * Start event processing
 */
void SimExec::run()
{
	int local_lbts, global_lbts, epoch = 0;
	int local_done = 0, global_done = 0;
	while (!fel.empty() || !global_done)
	{
		if (!local_done)
		{
			Event* e = fel.top();
		
			//Determine LBTS
			local_lbts = e->getTimestamp();
			MPI_Allreduce(&local_lbts, &global_lbts, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
			
			//Receive msgs sent from prev epoch
			//a. Compute msgs this proc's supposed to receive
			int global_msgCount[lpCount];
			MPI_Allreduce(msgCount, global_msgCount, lpCount, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
		
			//b. Receive these msgs
			receiveMsg(global_msgCount[rank]);
		
			//printf("Rank %d: Epoch=%d; msgCount=%d; felSize=%d, local_lbts=%d, global_lbts=%d\n", 
			//		rank, epoch, global_msgCount[rank], fel.size(), local_lbts, global_lbts);
				
			//Proc all msgs with ts < lbts
			while (local_lbts <= global_lbts && !fel.empty())
			{
				handleEvent(e);
				fel.pop();
				delete e;
				e = 0;
			
				if (!fel.empty())
				{
					e = fel.top();
					local_lbts = e->getTimestamp();
				}
				//printf("\tRank %d done handling event; felSize=%d; Next ts=%d\n",
				//	 rank, fel.size(),  local_lbts);
			}
		
			//Reset msg count
			for (int i = 0; i < lpCount; ++i)
				msgCount[i] = 0;
		
			++epoch;
		}
		if (fel.empty())
			local_done = 1;
		MPI_Allreduce(&local_done, &global_done, 1, MPI_INT, MPI_LAND, MPI_COMM_WORLD);
	}
	
	//Done event proc
	//Cleanup
	delete[] msgCount; // use array delete to deallocate array
}


//Private stuff
void SimExec::receiveMsg(int count)
{
	MPI_Status status;
	int hasMsg;
	int i = 0;
	
	MPI_Iprobe(MPI_ANY_SOURCE, MSG_TAG, MPI_COMM_WORLD, &hasMsg, &status);
	while (i < count || hasMsg)
	{		
		if (hasMsg)
		{
			//Receive msg here
			int data[2];
			MPI_Recv(data, 2, MPI_INT, status.MPI_SOURCE, MSG_TAG, MPI_COMM_WORLD, &status);
			fel.push(new Event(data[TS_IDX], data[STCT_IDX]));
			++i;
		}
		
		//keep checking till all msgs have been received
		MPI_Iprobe(MPI_ANY_SOURCE, MSG_TAG, MPI_COMM_WORLD, &hasMsg, &status);
	}
	
}

void SimExec::handleEvent(Event* e)
{
	int ts = e->getTimestamp();
//	printf("\nRank %d is processing event with ts %d", rank, ts);
	
	if (e->getRemainStopCount() > 0)
	{
		//Randomly generate the next stop if remStop > 0
		int nextStop = rand() % lpCount;
		
		int* data = new int[2];
		data[TS_IDX] = ts + LA;
		data[STCT_IDX] = e->getRemainStopCount() - 1;
		outbox.push_back(data);
		
		MPI_Request req;
		MPI_Isend(data, 2, MPI_INT, nextStop, MSG_TAG, MPI_COMM_WORLD, &req);
	}
	//else
	//	printf("Last stop reach. Do nothing\n");
}
