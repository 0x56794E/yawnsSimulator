#include <limits> //For max val of int
#include "mpi.h"
#include <string> //for string
#include <math.h> //for sqrt

//own includes
#include "SE.h"
#include "TrafficLoader.h"
#include "Communicator.h"

using namespace std;

const int MAX_INT = std::numeric_limits<int>::max();
const int INTENDED_P_COUNT = 4;

/***********
 * PRIVATE
 **********/
static void compStats(Stats &stats, double &var, double &mean, int n, int curLPCount)
{
    //update sum
    stats.sum += curLPCount;

    //update max
    if (curLPCount > stats.max)
        stats.max = curLPCount;

    //update min
    if (curLPCount < stats.min)
        stats.min = curLPCount;

    //update var and mean
    if (n == 1) //first iter
    {
        mean = curLPCount;
        var = 0;
    }
    else     
    {
        //update var using old mean
        var = (n - 2) * var / (n - 1) + (curLPCount - mean) * (curLPCount - mean) / n; 
        
        //update mean               
        mean = ((n - 1) * mean + curLPCount) / n;
    }
}

void SE::nextEpoch()
{
    //On Communicator.
    //reset msg for next epoch
	onEpochEnd();

    //Iter thru LPs to collect stats
    //max, min, avg, std dev
    Stats stats;
    stats.max = 0;
    stats.min = MAX_INT;
    stats.sum = 0;
    stats.avg = 0;
    stats.stdDev = 0;

    double var = 0, mean = 0, n = 1;
    
    //TODO: hacky... for now
    if (type == NODE)
    {
        //work on nodeLPMap
        for(NodeLPMap::const_iterator it = nodeLPMap.begin(); it != nodeLPMap.end(); ++it)
        {
            //key = it->first
            //value = iter->second
            compStats(stats, var, mean, n, it->second->getCurEpochEventCount());
            ++n;
        }
    }
    else
    {
        //work on linkLPMap 
        for(LinkLPMap::const_iterator it = linkLPMap.begin(); it != linkLPMap.end(); ++it)
        {
            //key = it->first
            //value = iter->second
            compStats(stats, var, mean, n, it->second->getCurEpochEventCount());
            ++n;
        }
    }

    stats.avg = mean;
    stats.stdDev = sqrt(var);
    
    //Add stats for this epoch
    (this->perEpochStats).push_back(stats);

    //Advance the epoch
    this->curEpoch = this->curEpoch + 1;
    
	//Reset msgCount
	for (int i = 0; i < p; ++i)
		msgCount[i] = 0;
}

/******************
 * PUBLIC members *
 ******************/
SE::SE(int p, int rank, string graph_file_name, MODEL_TYPE type)
{
	this->p = p;
	this->rank = rank;
	this->type = type;
    this->curEpoch = 0;

	//init msg count to other procs
	msgCount = new int[p];
	for (int i = 0; i < p; ++i)
		msgCount[i] = 0;

	if (type == LINK)
	{
		doLoadLink(rank, p, graph_file_name, linkLPMap, rankMap);
		loadScalefreeTraffic(rank, p, graph_file_name, this, 0, "link_");
	}
	else
	{
		doLoadNode(rank, p, graph_file_name, nodeLPMap, rankMap);
		loadScalefreeTraffic(rank, p, graph_file_name, this, 1, "node_");
	}
}

void SE::runLink()
{

	int local_done = 0, global_done = 0;
	int global_lbts;
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
		while (!(this->fel).empty() && this->peekNextTimestamp() <= global_lbts)
		{
			Event* event = this->nextEvent();
			linkLPMap[event->getCurrentStopId()]
                    ->handleEvent(event, fel, linkLPMap, rankMap);
		}

		//Advance epoch count
		this->nextEpoch();

		//Determine if all LPs accross all procs are done
		local_done = (this->fel).empty();
		MPI_Allreduce(&local_done, &global_done, 1, MPI_INT, MPI_LAND, MPI_COMM_WORLD);
	}

}

void SE::runNode()
{

	int local_done = 0, global_done = 0;
	int global_lbts;
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
		while (!(this->fel).empty() && this->peekNextTimestamp() <= global_lbts)
		{
			Event* event = this->nextEvent();
			nodeLPMap[event->getCurrentStopId()]
                    ->handleEvent(event, fel, nodeLPMap, rankMap);
		}

		//Advance epoch count
		this->nextEpoch();

		//Determine if all LPs accross all procs are done
		local_done = (this->fel).empty();
		MPI_Allreduce(&local_done, &global_done, 1, MPI_INT, MPI_LAND, MPI_COMM_WORLD);
	}

}

/**
 * The "main loop"
 */
void SE::run()
{
	//VERY STUPID BUT will fix later
	if (type == LINK)
		runLink();
	else
		runNode();

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
//	printf("rank=%d, local=%d, global=%d\n",
//			rank, local_lbts, global_lbts);

	return global_lbts;	
}

void SE::receiveMsgs()
{	
	//a. Comp msgs this proc's supposed to receive
	int global_msgCount[p];
	MPI_Allreduce(msgCount, global_msgCount, p, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

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

int SE::getTotalProcessedEvent()
{
	int sum = 0;
	if (type == LINK)
	{
		for (LinkLPMap::const_iterator it = linkLPMap.begin(); it != linkLPMap.end(); ++it)
		{
			sum += it->second->getTotalProcessedEvent();
		}
	}
	else
	{
		for (NodeLPMap::const_iterator it = nodeLPMap.begin(); it != nodeLPMap.end(); ++it)
		{
			sum += it->second->getTotalProcessedEvent();
		}

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
