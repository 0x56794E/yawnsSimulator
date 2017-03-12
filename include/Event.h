#ifndef EVENT_H
#define EVENT_H

#include <queue> //for queue
using namespace std;

class Event
{
	int timestamp;
	int stop_count; //Num of stop to pass INCL first one
	int lpId;        //ID of the LP to handle this event? => need to keep track of "home" LP?
	int stop_passed; //= [1, stop_count]; == stop_count means we're handling the last stop
	
  public:
	Event (int ts, int stop_count, int lpId, int stop_passed);
	int getTimestamp();
	int getStopCount();
	int getStopPassed(); //ONE based; number of stops have passed
	int getCurrentStopId();  //ID of current LP

	//Modifier
	void handled(); //to inc stop idx
	void setTimestamp(int); //to reuse the event for same-proc comm
};

#endif
