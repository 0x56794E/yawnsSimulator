#include "Event.h"

using namespace std;

Event::Event(int ts, int stop_count, int lpId, int stop_passed, int type, int last_node_id)
{
	Event::timestamp = ts;
	Event::stop_count = stop_count;
	Event::lpId = lpId;
	Event::stop_passed = stop_passed;
	Event::type = type;
	Event::last_node_id = last_node_id;	
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

int Event::getType()
{
	return type;
}

int Event::getLastNodeId()
{
	return last_node_id;
}

void Event::setCurrentStopId(int id)
{
	lpId = id;
}

void Event::incStopPassed()
{
	stop_passed++;
}

void Event::setTimestamp(int ts)
{
	timestamp = ts;
}

void Event::setLastNodeId(int id)
{
	last_node_id = id;
}

void Event::setType(int newType)
{
	type = newType;
}

