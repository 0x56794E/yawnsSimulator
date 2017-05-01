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
	int id;
	int totalEvent; //Total event processed
	vector<int> neighbors; //Set of IDs of the neighbor of this LP
	
  public:
  	LP(int id);
  	int getId();
  	void addNeighbor(int); //Add a nei's ID
	int getRandNeiId();
  	int getTotalProcessedEvent();
  	void incEventCount();
};

#endif
