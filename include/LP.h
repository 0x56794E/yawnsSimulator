#ifndef LP_H
#define LP_H

#include <queue> //For priority queue
#include <string> //for string
#include <map>

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
	
  public:
  	LP(int id);
	int getTotalProcessedEvent();
	int getFELSize();
	void handleEvent(Event*, LPMap lpMap);
	void scheduleEvent(Event*);
	int done();
	int peekNextTimestamp(); //If queue is empty, return MAX_INT
	Event* nextEvent(); //return the next event and remove it from the fel.
};

#endif
