#include "Event.h"

using namespace std;

Event::Event(int ts, int stop_count, int lpId)
{
	Event::timestamp = ts;
	Event::stop_count = stop_count;
	Event::lpId = lpId;
}

int Event::getTimestamp()
{
	return timestamp;
}

int Event::getStopCount()
{
	return stop_count;
}

int Event::getCurrentStopIdx()
{
	return cur_stop_idx;
}

int Event::getCurrentStopId()
{
	return lpId;
}

void Event::handled()
{
	cur_stop_idx++;
}

void Event::setTimestamp(int ts)
{
	timestamp = ts;
}
