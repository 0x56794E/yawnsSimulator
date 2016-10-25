#ifndef SE_H
#define SE_H

#include "Event.h"
#include "LP.h"

#include <string> 

using namespace std;

/**
 * Manage the LPs on this proc
 * Receiving messages from other proc and distribute to appropriate LP
 */
class SE
{
	//Fields
	int lpCount;
	int rank;
	vector<int *> outbox; //List of outgoing msgs
	int* msgCount; //Number of msgs sent to ea proc in current epoch

	//LPs
	LPMap lpMap; 
	int minRow;
	int maxRow;
	int minCol;
	int maxCol;

	//Methods
	int compLBTS(); //Determine LBTS for current iteration/epoch
	void receiveMsgs();
	int done(); //true if all LPs on this proc are done

  public:
  	SE(int lpCount, int rank, int gridSize);
  	void run(); //start event processing
};

#endif
