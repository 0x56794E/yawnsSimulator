#ifndef EVENT_H
#define EVENT_H

#include <queue> //for queue
using namespace std;

class Event
{
	int timestamp;
	int remainStopCt;
	queue<int> stops;
	
  public:
	Event (int ts, int stopCount);
	int getTimestamp();
	int getRemainStopCount();
	void addStop(int stop);
	int nextStop(); //return the id of the next stop; remove it from the queue
};

#endif
