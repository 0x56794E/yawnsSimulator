#include "LP.h"
#include "Communicator.h"

#include <limits>
#include <time.h> //for time() used to seed rangen
#include <stdlib.h> //for rand(), srand()

const int MAX_INT = std::numeric_limits<int>::max();
const int ARR = 0;
const int DEPT = 1;

/***************************
 * LP 
 ***************************/
LP::LP(int id)
{
	LP::id = id;
	LP.totalEvent = 0;
	srand(time(NULL));
}

int LP::getId()
{
	return id;
}

int LP::getTotalProcessedEvent()
{
	return totalEvent;
}

void LP::incEventCount()
{
	++totalEvent;
}

/***************************
 * Link LP 
 ***************************/
LinkLP::LinkLP(int id, int node1Id, int node2Id) : LP(id)
{	
	LinkLP::node1Id = node1Id;
	LinkLP::node2Id = node2Id;
}

void LinkLP::addNeighbor(int neiId, int sharedNodeId)
{
	if (sharedNodeId == node1Id)
		node1Links.push_back(neiId);
	else
		node2Links.push_back(neiId);
}

int LinkLP::getRandNextStopId(int lastNodeId)
{
	if (lastNodeId == node1Id)
	{
		//Pick randomly from list for node 2
		int idx = rand() % node2Links.size();
		return node2Links.at(idx);
	}
	else
	{
		int idx = rand() % node1Links.size();
		return node1Links.at(idx);
	}
}

/***************************
 * Node LP 
 ***************************/
NodeLP::NodeLP(int id) : LP(id)
{ 
	//empty
}

void NodeLP::addNeighbor(int neiId)
{
	NodeLP::neighbors.push_back(neiId);
}

int NodeLP::getRandNextStopId(int lastNodeId)
{
	int ret = lastNodeId;
	int idx;

	while (ret == lastNodeId)
	{
		idx = rand() % neighbors.size();
		ret = neighbors.at(idx);		
	}
	return ret;
}

void NodeLP::handleEvent(Event* event, EventQueue &fel, LPMap &lpMap, map<int, pair<int, int>> &rankMap)
{
	//Rules:
	//If event type == DEPARTURE (1) 
	//  => Schedule Arrive on NEXT stop => create/modify the event!!!
	//If event type == ARRIVAL (0)
	//  => Schedule Departure on CURRENT stop
	//  => KEEP ALL DATA of the event the same EXCEPT for the type and timestamp
	
	int stopPassed = event->getStopPassed() + 1;

	if (event.getType() == ARR)
	{
		//THIS is the last stop
		//StopCount == Num of nodes INCLUDING last
		if (stopPassed == event->getStopCount())
		{
			//Free the mem occupied by event
			delete event;
		}
		else		
		{
			//Schedule departure on SAME LP; Keep the event data UNCHANGED
			//Except for the timestamp & event type		
			event->setTimestamp(event->getTimestamp() + LA);
			event->setType(DEPT);
			fel.push(event);
		}
	}
	else
	{
		//Create new event object and send off
		// => Schedule an arrival event at next stop
		int last_node_id = event->getCurrentStopId();
		int nextStopId = this->getRandNextStopId(last_node_id);
		int ts = event->getTimestamp() + LA;
		int type = ARR;	

		//Update the event
		event->incStopPassed();
		event->setLastNodeId(event->getCurrenStopId());
		event->setCurrentStopId(nextStopId);
		event->setEvetType(ARR);
		event->setTimestamp(ts);

		//Determine whether inter-proc comm is needed
		if (lpMap.find(nextStopId) == lpMap.end())
		{
			//ONLY call communicator AFTER determine 
			//if next stop is on other proc
			sendMsg(rankMap, event);
		}
		else
		{
			//Schedule on the current proc 
			//with new handler and updated data
			fel.push(event);
		}
	}
}
