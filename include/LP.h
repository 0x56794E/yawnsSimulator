#ifndef LP_H
#define LP_H

//#include <queue> //For priority queue
#include <string> //for string
#include <map>
#include <vector> //for the neighbor set
#include <utility> //for pair

//own includes
#include "Event.h"
#include "EventComparator.h"

using namespace std;

class LP; //forward decl
typedef map<int, LP*> LPMap; //key: LP's id; value: LP

class LP
{	
  public:
	int getId(); 
  	int getTotalProcessedEvent();
  	void incEventCount();

 protected:
	LP(int id);
	int id;
	int totalEvent; //Total event processed
};

class LinkLP : public LP
{
	int node1Id;
	int node2Id;
	vector<int> node1Links; //IDs of all links touching node1
	vector<int> node2Links; //IDs of all links touching node2

 public:
	LinkLP(int id, int node1Id, int node2Id);
	void addNeighbor(int neiId, int nodeId);
	int getRandNextStopId(int lastNodeId);
}

class NodeLP : public LP
{
	vector<int> neighbors;

 public:
	NodeLP(int id);
	void addNeighbor(int neiId);
	int getRandNextStopId(int lastNodeId);
}
#endif
