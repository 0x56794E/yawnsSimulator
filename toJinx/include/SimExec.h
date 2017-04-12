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
	int totalEvent; //Total event processed
	
	//Methods
	void handleEvent(Event*);
	void receiveMsg(int);
	void genPackets(); //gen initial packets. Diff with ea run
	void loadPackets(); //load packets. Same with ea run
	
  public:
  	SimExec(int lpCount, int rank);
  	void run();
	int getTotalEvent();
};

#endif
