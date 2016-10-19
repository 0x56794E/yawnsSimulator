#ifndef LP_H
#define LP_H

#include <queue> //For priority queue
#include <string> //for string

//own includes
#include "Event.h"

using namespace std;

class LP
{
	//Fields
	string id;
	std::priority_queue<Event*, std::vector<Event*>, EventComparator> fel;
	int totalEvent; //Total event processed
	
	//Methods
	void genPackets(); //gen initial packets. Diff with ea run
	void loadPackets(); //load packets. Same with ea run
	
  public:
  	LP(int id);
	int getTotalEvent();
	void handleEvent(Event*);
	void scheduleEvent(Event*);
};

#endif
