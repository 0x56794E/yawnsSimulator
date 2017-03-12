/**
 * Responsible for sending msgs across LPs.
 * If LPs are on the same proc, simply add the msgs to corresponding LP's fel.
 * Otherwise, send it across proc
 */
#ifndef COMM_H
#define COMM_H

#include "mpi.h"
#include <vector>

#include "LP.h"

//Tag for "regular" msgs
const int MSG_TAG = 0;
const int MSG_SIZE = 4;

//allow inbox to reach up to 25 msgs.
const int MAX_SIZE = 25;

//Index for data array
const int TIMESTAMP = 0;
const int STOP_COUNT = 1;
const int STOP_PASSED = 2;
const int HANDLER_ID = 3;

void newEpoch();

void sendMsg(Event* event, LPMap &lpMap, int nextStopId, map<int, pair<int, int>> &rankMap);

/**
 * Receive msg and schedule it on appropriate LP
 */
void receiveMsg(MPI_Status, LPMap lpMap);

int done(MPI_Request req);

#endif
