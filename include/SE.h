#ifndef SE_H
#define SE_H

#include <queue> //For priority queue
#include <map>
#include <string>

#include "Event.h"
#include "LP.h"
#include "TopologyUtils.h"

using namespace std;

struct Stats
{
    //fields
    int max;
    int min;
    int sum;
    double avg;
    double stdDev;
};

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
	int* msgCount; //Number of msgs sent to ea proc in current epoch
	MODEL_TYPE type;
    vector<Stats> perEpochStats; //count of event proc'ed per epoch
    int curEpoch;

	//LPs
	//HACKY BUT DESPERATE TIMES...!!!
	LinkLPMap linkLPMap;
	NodeLPMap nodeLPMap;

	map<int, pair<int, int>> rankMap; //Map spec range of LP IDs on ea proc; key: rank; val: array of size 2: [minID, maxId]

    //********
	//Methods
    //********
	int compLBTS(); //Determine LBTS for current iteration/epoch
	void receiveMsgs();

	EventQueue fel;	//One FEL for all LPs on this proc
	Event* nextEvent();
	
	//VERY stupid but im desperate right now...
	void runNode();
	void runLink();
    void nextEpoch();

  public:
  	SE(int lpCount, int rank, string graph_file_name, MODEL_TYPE type);
  	void run(); //start event processing
	int getTotalProcessedEvent(); //total number of events processed by all LPs on this SE
	void scheduleEvent(Event*);
	int peekNextTimestamp(); //If queue is empty, return MAX_INT
    void outputPerEpochStats();
};

#endif
