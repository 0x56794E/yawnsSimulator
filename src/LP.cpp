#include "LP.h"
#include "Communicator.h"

#include <limits>

const int MAX_INT = std::numeric_limits<int>::max();
const int LA = 10; 

LP::LP(int id)
{
	LP::id = id;
	LP::totalEvent = 0;
}

int LP::getId()
{
	return id;
}

int LP::getTotalProcessedEvent()
{
	return totalEvent;
}

void LP::scheduleEvent(Event* event)
{
	fel.push(event);
}

void LP::handleEvent(Event* event, LPMap lpMap)
{
	totalEvent++;

	//me - the one who's processing this event;
	//Don't need this value. Call to remove it from queue. TODO: better name?
	//event->nextStop();

	if (event->getCurrentStopIdx() < event->getStopCount())
	{
		//Send msg to nextStop
		event->setTimestamp(event->getTimestamp() + LA);
		sendMsg(event, lpMap);
	}

	event->handled();
}

int LP::getFELSize()
{
	return fel.size();
}

int LP::done()
{
	return fel.empty();
}

int LP::peekNextTimestamp()
{
	if (fel.empty())
		return MAX_INT;
	else
		return fel.top()->getTimestamp();	
}

Event* LP::nextEvent()
{
	Event* event = fel.top();
	fel.pop();
	return event;
}

void LP::addNeighbor(int neiId)
{
	neighbors.insert(neiId);
}
