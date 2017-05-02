#ifndef EVENT_H
#define EVENT_H

#include "EventComparator.h"
#include <queue> //for queue

using namespace std;

//Forward decl
class Event;

typedef std::priority_queue<Event*, std::vector<Event*>, EventComparator> EventQueue;

class Event
{
	int type; //0==arrive; 1==dept
	int last_node_id; //for the node model, this is same as handler/lp id
	int timestamp;
	int stop_count; //Num of stop to pass INCL first one
	int lpId;        //ID of the LP to handle this event => need to keep track of "home" LP?
	int stop_passed; //= [1, stop_count]; == stop_count means we're handling the last stop
	
  public:
	Event (int ts, int stop_count, int lpId, int stop_passed, int type, int last_node_id);
	int getTimestamp();
	int getStopCount();
	int getStopPassed(); //ONE based; number of stops have passed
	int getCurrentStopId();  //ID of current LP - the event handler
	int getType();
	int getLastNodeId();

	//Modifier
	void setCurrentStopId(int);
	void incStopPassed(); //to inc stop idx
	void setTimestamp(int); //to reuse the event for same-proc comm
	void setLastNodeId(int);
	void setType(int);
};

#endif
