#ifndef SE_H
#define SE_H

#include "Event.h"

#include <map>
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
	map<int, LP*> lpMap; 
	int minRow;
	int maxRow;
	int minCol;
	int maxCol;

	//Methods
	void receiveMsg(int);
	void genPackets(); //gen initial packets. Diff with ea run
	void loadPackets(); //load packets. Same with ea run
	void loadLPs(); //load LPs from file

  public:
  	SE(int lpCount, int rank);
  	void run(); //start event processing
	int getTotalEvent();
	void sendMessage(); //send messages; will determine if inter-processor communication is needed; if yes, will be put in outbox; else schedule directly on lp's fel
};

#endif
