#ifndef EVENT_H
#define EVENT_H

using namespace std;

class Event
{
	int timestamp;
	
  public:
	Event (int t);
	int getTimestamp();
};

#endif
