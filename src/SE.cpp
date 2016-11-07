#include <limits> //For max val of int
#include "mpi.h"

//own includes
#include "SE.h"
#include "TopologyUtils.h"
#include "TrafficLoader.h"
#include "Communicator.h"

const int MAX_INT = std::numeric_limits<int>::max();

/******************
 * PUBLIC members *
 ******************/
SE::SE(int lpCount, int rank, int gridSize)
{
	this->lpCount = lpCount;
	this->rank = rank;

	//Load LPs & init msgCount
	loadLP(rank, lpCount, gridSize, lpMap, minRow, maxRow, minCol, maxCol);

	//init msg count to other procs
	msgCount = new int[lpCount];
	for (int i = 0; i < lpCount; ++i)
		msgCount[i] = 0;

	//load initial packets/msgs for ea LP here
	loadTrafficMultiForGrid(rank, lpCount, gridSize, lpMap);
}

/**
 * The "main loop"
 */
void SE::run()
{
	int local_done = 0, global_done = 0;
	int global_lbts, epoch = 0;
	LP* lp;

	while (!local_done || !global_done)
	{
		//Determine LBTS
		global_lbts = compLBTS();

		//Receive all msgs from prev epoch
		//TODO: this can be done in another thread while e proc is IP
		receiveMsgs();

		//Handle all events w ts < lbts
		for (LPMap::const_iterator it = lpMap.begin(); it != lpMap.end(); ++it)
		{
			lp = it->second;
			while (!lp->done() && lp->peekNextTimestamp() <= global_lbts)
			{
				lp->handleEvent(lp->nextEvent(), lpMap);				
			}
		}

		//Reset msgCount
		for (int i = 0; i < lpCount; ++i)
			msgCount[i] = 0;

		//Advance epoch count
		++epoch;
		newEpoch();

		//Determine if all LPs accross all procs are done
		local_done = done();	
		MPI_Allreduce(&local_done, &global_done, 1, MPI_INT, MPI_LAND, MPI_COMM_WORLD);
	}

	//clean up
	delete[] msgCount;
}


/*******************
 * PRIVATE members *
 *******************/
int SE::compLBTS()
{
	int local_lbts = MAX_INT, global_lbts;

	//Locally determine LBTS accross LPs
	for (LPMap::const_iterator it = lpMap.begin(); it != lpMap.end(); ++it)
	{
		if (it->second->peekNextTimestamp() < local_lbts)
		{
			local_lbts = it->second->peekNextTimestamp();
		}	
	}

	//Globally determine LBTS
	MPI_Allreduce(&local_lbts, &global_lbts, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);

	return global_lbts;	
}

void SE::receiveMsgs()
{	
	//a. Comp msgs this proc's supposed to receive
	int global_msgCount[lpCount];
	MPI_Allreduce(msgCount, global_msgCount, lpCount, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

	//b. Receive these msgs: global_msgCount[rank]
	MPI_Status status;
	int hasMsg, i = 0;
	MPI_Iprobe(MPI_ANY_SOURCE, MSG_TAG, MPI_COMM_WORLD, &hasMsg, &status);
	while (i < global_msgCount[rank])
	{
		if (hasMsg)
		{
			//receiv msg here
			//TODO: need to examine and determine the LP to forward to
			//Msg would be an array of int <timestamp of msg> <next stop id => the LP to forward to> <the rest of the stops>
			receiveMsg(status, lpMap);
		}

		//else, keep checking
		MPI_Iprobe(MPI_ANY_SOURCE, MSG_TAG, MPI_COMM_WORLD, &hasMsg, &status);
	}
}

int SE::done()
{
	int done = 1;	
	for (LPMap::const_iterator it = lpMap.begin(); it != lpMap.end(); ++it)
	{
		done = done && it->second->done();
	}
	return done;
}

int SE::getTotalProcessedEvent()
{
	int sum = 0;
	for (LPMap::const_iterator it = lpMap.begin(); it != lpMap.end(); ++it)
	{
		sum += it->second->getTotalProcessedEvent();
	}
	return sum;
}

