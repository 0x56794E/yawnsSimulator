#ifndef LP_H
#define LP_H

#include <queue> //For priority queue
#include <string> //for string
#include <map>
#include <unordered_set> //for the neighbor set

//own includes
#include "Event.h"
#include "EventComparator.h"

using namespace std;

class LP;
typedef map<int, LP*> LPMap; //key: LP's id; value: LP

class LP
{
	int id;
	std::priority_queue<Event*, std::vector<Event*>, EventComparator> fel;
	int totalEvent; //Total event processed
	unordered_set<int> neighbors; //Set of IDs of the neighbor of this LP
	
  public:
  	LP(int id);
  	void addNeighbor(LP* lp); //Add an LP as a neighbor
	int getTotalProcessedEvent();
	int getFELSize();
	void handleEvent(Event*, LPMap lpMap);
	void scheduleEvent(Event*);
	int done();
	int peekNextTimestamp(); //If queue is empty, return MAX_INT
	Event* nextEvent(); //return the next event and remove it from the fel.
};

#endif
