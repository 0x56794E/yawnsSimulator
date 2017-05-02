#ifndef SE_H
#define SE_H

#include <queue> //For priority queue
#include <map>
#include <string>

#include "Event.h"
#include "LP.h"

using namespace std;


/**
 * Manage the LPs on this proc
 * Receiving messages from other proc and distribute to appropriate LP
 */
class SE
{
	//Fields
	int lpCount;
	int p; //num procs
	int rank;
	vector<int *> outbox; //List of outgoing msgs
	int* msgCount; //Number of msgs sent to ea proc in current epoch

	//LPs
	LPMap lpMap; //Map of all LP residing on this proc
	map<int, pair<int, int>> rankMap; //Map spec range of LP IDs on ea proc; key: rank; val: array of size 2: [minID, maxId]

	//Methods
	int compLBTS(); //Determine LBTS for current iteration/epoch
	void receiveMsgs();

	//One FEL for all LPs on this proc
	//std::priority_queue<Event*, std::vector<Event*>, EventComparator> fel;
	EventQueue fel;	
	Event* nextEvent();
	void handleEvent(Event*, LP*);

  public:
  	SE(int lpCount, int rank, string graph_file_name);
  	void run(); //start event processing
	int getTotalProcessedEvent(); //total number of events processed by all LPs on this SE
	void scheduleEvent(Event*);
	int peekNextTimestamp(); //If queue is empty, return MAX_INT
};

#endif
