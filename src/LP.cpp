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

void LP::incEventCount()
{
	++totalEvent;
}

void LP::addNeighbor(int neiId)
{
	neighbors.push_back(neiId);
}

int LP::getRandNeiId()
{
	return neighbors.at(rand() % neighbors.size());
}
