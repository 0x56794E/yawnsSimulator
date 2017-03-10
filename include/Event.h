#ifndef EVENT_H
#define EVENT_H

#include <queue> //for queue
using namespace std;

class Event
{
	int timestamp;
	int stop_count;
	int lpId;
	int cur_stop_idx; //= [1, stop_count]; == stop_count means we're handling the last stop
	
  public:
	Event (int ts, int stop_count, int lpId);
	int getTimestamp();
	int getStopCount();
	int getCurrentStopIdx(); //ONE based
	int getCurrentStopId();  //ID of current LP
	void handled(); //to inc stop idx
	void setTimestamp(int); //to reuse the event for same-proc comm
};

#endif
