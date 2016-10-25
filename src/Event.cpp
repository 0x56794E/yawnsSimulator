#include "Event.h"

using namespace std;

Event::Event(int ts)
{
	Event::timestamp = ts;
}

int Event::getTimestamp()
{
	return timestamp;
}

int Event::getRemainStopCount()
{
	return stops.size();
}

void Event::addStop(int i)
{
	stops.push(i);
}

void Event::setTimestamp(int newTs)
{
	timestamp = newTs;
}

int Event::nextStop()
{
	int next = stops.front();
	stops.pop();
	return next;
}

int Event::peekNextStop()
{
	return stops.front();
}
