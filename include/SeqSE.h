#ifndef SEQSE_H
#define SEQSE_H

#include "Event.h"
#include <queue> //For priority queue

class SeqSE
{
	//Fields
	int lpCount;
	std::vector<LP*> lps; //List of LPs on this proc
	int totalEvent; //Total event processed
	
	int compLBTS(); //Determine LBTS for current iteration/epoch

  public:
  	SeqSE(int lpCount);
  	void run();
};

#endif
