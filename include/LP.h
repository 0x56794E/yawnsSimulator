#ifndef LP_H
#define LP_H

//#include <queue> //For priority queue
#include <string> //for string
#include <map>
#include <vector> //for the neighbor set
#include <utility> //for pair

//own includes
#include "Event.h"

using namespace std;

//Forward decls
class LP; 
class LinkLP;
class NodeLP;

//typedef map<int, LP*> LPMap; //key: LP's id; value: LP
typedef map<int, LinkLP*> LinkLPMap; //key: LP's id; value: NodeLP
typedef map<int, NodeLP*> NodeLPMap; //key: LP's id; value: LinkLP

class LP
{	
  public:
	int getId(); 
  	int getTotalProcessedEvent();
  	void incEventProcessedCount();
	
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
	int getOtherEnd(int lastNodeId);
	void addNeighbor(int neiId, int sharedNodeId);
	int getRandNextStopId(int lastNodeId);
	void handleEvent(Event* event, EventQueue &fel, LinkLPMap &lpMap, map<int, pair<int, int>> &rankMap);
};

class NodeLP : public LP
{
	vector<int> neighbors;

  public:
	NodeLP(int id);
	void addNeighbor(int neiId);
	int getRandNextStopId(int lastNodeId);
	void handleEvent(Event* event, EventQueue &fel, NodeLPMap &lpMap, map<int, pair<int, int>> &rankMap);
};
#endif
