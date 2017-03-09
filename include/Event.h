#ifndef EVENT_H
#define EVENT_H

#include <queue> //for queue
using namespace std;

class Event
{
	int timestamp;
	int stop_count;
	int cur_stop_idx; //= [0, stop_count - 1]
	
  public:
	Event (int ts, int stop_count);
	int getTimestamp();
	int getStopCount();
	int getCurrentStopIdx();
	void setTimestamp(int); //to reuse the event for same-proc comm
};

#endif
