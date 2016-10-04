#include <stdio.h> //For printf
#include <queue>   //For priority queue
#include "mpi.h"   //For MPI stuff

//My stuff
#include "Event.h"
#include "util/EventComparator.h"

int main(int argc, char** argv)
{
	Event* e1 = new Event(1,1);
	Event* e2 = new Event(2,2);
	Event* e3 = new Event(3,3);
	Event* e4 = new Event(4,4);
	
	std::priority_queue<Event*, std::vector<Event*>, EventComparator> fel;
	fel.push(new Event(1,1));
	fel.push(e2);
	fel.push(e3);
	fel.push(e4);
	
	while (!fel.empty())
	{
		Event* e = fel.top();
		printf("Popping e ts = %d\n", e->getTimestamp());
		fel.pop();
	}
}
