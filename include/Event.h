#ifndef EVENT_H
#define EVENT_H

#include <queue> //for queue
using namespace std;

class Event
{
	int timestamp;
	queue<int> stops;
	
  public:
	Event (int ts);
	int getTimestamp();
	int getRemainStopCount();
	void addStop(int stop);
	void setTimestamp(int); //to reuse the event for same-proc comm
	int nextStop(); //return the id of the next stop; remove it from the queue
	int peekNextStop(); //return the id of the next stop WIHOUT removing it from the queue
};

#endif
