#ifndef EVENT_H
#define EVENT_H

using namespace std;

class Event
{
	int timestamp;
	int remainStopCt;
	
  public:
	Event (int ts, int stopCount);
	int getTimestamp();
	int getRemainStopCount();
};

#endif
