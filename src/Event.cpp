#include "Event.h"

using namespace std;

Event::Event(int ts, int remStopCt)
{
	Event::timestamp = ts;
	Event::remainStopCt = remStopCt;
}

int Event::getTimestamp()
{
	return timestamp;
}

int Event::getRemainStopCount()
{
	return remainStopCt;
}
