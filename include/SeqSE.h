#ifndef SE_H
#define SE_H

#include "Event.h"
#include <queue> //For priority queue

class SE
{
	//Fields
	int lpCount;
	int rank;
	std::vector<int *> outbox;
	std::vector<LP*> lps; //List of LPs on this proc
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
