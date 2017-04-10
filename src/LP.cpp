#include "LP.h"
#include "Communicator.h"

#include <limits>
#include <time.h> //for time() used to seed rangen
#include <stdlib.h> //for rand(), srand()

const int MAX_INT = std::numeric_limits<int>::max();

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

//void LP::scheduleEvent(Event* event)
//{
//	fel.push(event);
//}

void LP::incEventCount()
{
	++totalEvent;
}

//int LP::getFELSize()
//{
//	return fel.size();
//}

//int LP::done()
//{
//	return fel.empty();
//}

//int LP::peekNextTimestamp()
//{
//	if (fel.empty())
//		return MAX_INT;
//	else
//		return fel.top()->getTimestamp();
//}

//Event* LP::nextEvent()
//{
//	Event* event = fel.top();
//	fel.pop();
//	return event;
//}

void LP::addNeighbor(int neiId)
{
	neighbors.push_back(neiId);
}

int LP::getRandNeiId()
{
	return neighbors.at(rand() % neighbors.size());
}
