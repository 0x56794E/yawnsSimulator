#ifndef LP_H
#define LP_H

#include <queue> //For priority queue
#include <string> //for string
#include <map>
#include <vector> //for the neighbor set
#include <utility> //for pair

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
	vector<int> neighbors; //Set of IDs of the neighbor of this LP
	
  public:
  	LP(int id);
  	int getId();
  	void addNeighbor(int); //Add a nei's ID
	int getRandNeiId();
  	int getTotalProcessedEvent();
	int getFELSize();
	void handleEvent(Event*, LPMap &lpMap, map<int, pair<int, int>> &rankMap);
	void scheduleEvent(Event*);
	int done();
	int peekNextTimestamp(); //If queue is empty, return MAX_INT
	Event* nextEvent(); //return the next event and remove it from the fel.
};

#endif
