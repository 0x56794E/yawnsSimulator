#include <stdio.h> //For printf
#include <queue>   //For priority queue
#include "mpi.h"   //For MPI stuff
#include "string"
#include <map>

//My stuff
#include "Event.h"
#include "util/EventComparator.h"


using namespace std;

void loadMap(map<string, Event*> &mymap)
{
	mymap["1, 2"] = new Event(1, 2);
	mymap["3, 4"] = new Event(3, 4);
}

void testStringScope()
{
	map<string, Event*> mymap;
	loadMap(mymap);

	auto res1 = mymap.find("1, 1");
	printf("Found (1, 1)? %d\n", res1 != mymap.end());

	auto res2 = mymap.find("1, 2");
	printf("Found (1, 2)? %d\n", res2 != mymap.end());
	if (res2 != mymap.end())
		printf("\tFound Event at (1, 2): ts=%d, stopCt=%d\n", res2->second->getTimestamp(), res2->second->getRemainStopCount());
}

void testQ()
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



int main(int argc, char** argv)
{

	
	testStringScope();
}
