#include "LP.h"
#include "Communicator.h"

#include <limits>
#include <time.h> //for time() used to seed rangen
#include <stdlib.h> //for rand(), srand()

const int MAX_INT = std::numeric_limits<int>::max();

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

void LinkLP::addNeighbor(int neiId, int nodeId)
{
	if (nodeId == node1Id)
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
