#include "Event.h"

using namespace std;

Event::Event(int ts, int stop_count, int lpId)
{
	Event::timestamp = ts;
	Event::stop_count = stop_count;
	Event::lpId = lpId;
	Event::stop_passed = 1;
}

int Event::getTimestamp()
{
	return timestamp;
}

int Event::getStopCount()
{
	return stop_count;
}

int Event::getStopPassed()
{
	return stop_passed;
}

int Event::getCurrentStopId()
{
	return lpId;
}

void Event::handled()
{
	stop_passed++;
}

void Event::setTimestamp(int ts)
{
	timestamp = ts;
}
