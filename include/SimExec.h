#ifndef SIMEXEC_H
#define SIMEXEC_H

#include "Event.h"

class SimExec
{
	//Fields
	int lpCount;
	int rank;
	//TODO: FEL here => find q dt
	
	//Methods
	void handleEvent(Event);
	
  public:
  	SimExec(int lpCount, int rank);
  	void run();
};

#endif
