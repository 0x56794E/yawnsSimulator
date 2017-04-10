#include <limits> //For max val of int
#include "mpi.h"
#include <string> //for string
//own includes
#include "SE.h"
#include "TopologyUtils.h"
#include "TrafficLoader.h"
#include "Communicator.h"

using namespace std;

const int MAX_INT = std::numeric_limits<int>::max();
const int INTENDED_P_COUNT = 4;
const int LA = 10;

/******************
 * PUBLIC members *
 ******************/
//TODO: REMOVE GRID SIZE!!!
//Find num of LPs in total?
SE::SE(int lpCount, int rank, string graph_file_name)
{
	this->lpCount = lpCount;
	this->rank = rank;

	//Load LPs & init msgCount
	loadScalefreeLP(LINK, rank, lpCount, graph_file_name, lpMap, rankMap);
//	printf("Rank %d done loading graph\n", rank);

	//init msg count to other procs
	msgCount = new int[lpCount];
	for (int i = 0; i < lpCount; ++i)
		msgCount[i] = 0;
//	printf("Rank %d done init msg ct\n", rank);

	loadScalefreeTraffic(rank, lpCount, graph_file_name, this);
//	printf("Rank %d done loading traffic\n", rank);
}

/**
 * Gen next stop on the fly
 */
void SE::handleEvent(Event* event, LP* handler)
{
	handler->incEventCount();
	event->handled(); //to inc num of stops passed

	printf("LP %d has handled %d events so far\n",
			handler->getId(), handler->getTotalProcessedEvent());
	printf("Rank %d's rem eventCount = %d\n", fel.size());

	//THIS is the last stop
	if (event->getStopPassed() == event->getStopCount())
	{
		//TODO: what?
		//Free the mem occupied by event
		delete event;
	}
	//TODO: no need for if. Just 2b safe :D => will check
	else if (event->getStopPassed() < event->getStopCount())
	{
		//Send msg to nextStop
		event->setTimestamp(event->getTimestamp() + LA);
		sendMsg(this, event, lpMap, handler->getRandNeiId(), rankMap);
	}
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
		//printf("Rank %d starting epoch %d\n", rank, epoch);

		//Determine LBTS
		global_lbts = compLBTS();
		//printf("\tRank %d, epoch %d: finish comp LBTS: %d\n", rank, epoch, global_lbts);

		//Receive all msgs from prev epoch
		//TODO: this can be done in another thread while e proc is IP
		receiveMsgs();
		//printf("\tRank %d, epoch %d: finish recv Msgs\n", rank, epoch);

		//Handle all events w ts < lbts
		//while there's still event
		while (!this->done() && this->peekNextTimestamp() <= global_lbts)
		{
			Event* event = this->nextEvent();
			this->handleEvent(event, lpMap[event->getCurrentStopId()]);
		}

//		for (LPMap::const_iterator it = lpMap.begin(); it != lpMap.end(); ++it)
//		{
//			lp = it->second;
//			while (!lp->done() && lp->peekNextTimestamp() <= global_lbts)
//			{
//				lp->handleEvent(this, lp->nextEvent(), lpMap, rankMap);
//			}
//		}
		//printf("\tRank %d, epoch %d: finish handling events\n", rank, epoch);

		//Reset msgCount
		for (int i = 0; i < lpCount; ++i)
			msgCount[i] = 0;

		//printf("\tRank %d, epoch %d: finish resetting msg ct\n", rank, epoch);

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
Event* SE::nextEvent()
{
	Event* event = fel.top();
	fel.pop();
	return event;
}

int SE::compLBTS()
{
	int local_lbts = this->peekNextTimestamp();
	int global_lbts;

	//Globally determine LBTS
	MPI_Allreduce(&local_lbts, &global_lbts, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
	printf("rank=%d, local=%d, global=%d\n",
			rank, local_lbts, global_lbts);

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
			//Msg would be an array of ints (see receiveMsg for more detail)
			receiveMsg(status, this);
		}

		//else, keep checking
		MPI_Iprobe(MPI_ANY_SOURCE, MSG_TAG, MPI_COMM_WORLD, &hasMsg, &status);
	}
}

int SE::done()
{
	return fel.empty();
//	int done = 1;
//	for (LPMap::const_iterator it = lpMap.begin(); it != lpMap.end(); ++it)
//	{
//		done = done && it->second->done();
//	}
//	return done;
}

int SE::getTotalProcessedEvent()
{
	int sum = 0;
	for (LPMap::const_iterator it = lpMap.begin(); it != lpMap.end(); ++it)
	{
		//printf("\tlp %d has %d; SUM = %d\n", it->first, it->second->getTotalProcessedEvent(), sum);
		sum += it->second->getTotalProcessedEvent();
	}
	//printf("Rank %d has %d\n", rank, sum);
	return sum;
}

void SE::scheduleEvent(Event* event)
{
	fel.push(event);
}

int SE::peekNextTimestamp()
{
	if (fel.empty())
		return MAX_INT;
	else
		return fel.top()->getTimestamp();
}
