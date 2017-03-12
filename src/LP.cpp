#include "LP.h"
#include "Communicator.h"

#include <limits>
#include <time.h> //for time() used to seed rangen
#include <stdlib.h> //for rand(), srand()

const int MAX_INT = std::numeric_limits<int>::max();
const int LA = 10; 

LP::LP(int id)
{
	LP::id = id;
	LP::totalEvent = 0;

	srand(time(NULL));
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

/**
 * TODO: FIX THIS!!!!
 * Gen next stop on fly
 */
void LP::handleEvent(Event* event, LPMap &lpMap, map<int, pair<int, int>> &rankMap)
{
	totalEvent++;
	event->handled(); //to inc num of stops passed

	//THIS is the last stop
	if (event->getStopPassed() == event->getStopCount())
	{
		//TODO: what?
	}
	//TODO: no need for if. Just 2b safe :D => will check
	else if (event->getStopPassed() < event->getStopCount())
	{
		//Send msg to nextStop
		event->setTimestamp(event->getTimestamp() + LA);
		sendMsg(event, lpMap, getRandNeiId(), rankMap);
	}
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
	neighbors.push_back(neiId);
}

int LP::getRandNeiId()
{
	return neighbors.at(rand() % neighbors.size());
}
