#include "Event.h"

using namespace std;

Event::Event(int t)
{
	Event::timestamp = t;
}

int Event::getTimestamp()
{
	return timestamp;
}
