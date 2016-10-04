#ifndef SIMEXEC_H
#define SIMEXEC_H

#include "Event.h"
#include <queue> //For priority queue

class SimExec
{
	//Fields
	int lpCount;
	int rank;
	std::vector<int *> outbox;
	int* msgCount; //Number of msgs sent to ea proc in current epoch
	
	//Methods
	void handleEvent(Event*);
	void receiveMsg(int);
	
  public:
  	SimExec(int lpCount, int rank);
  	void run();
};

#endif
