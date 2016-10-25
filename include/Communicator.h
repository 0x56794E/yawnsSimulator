/**
 * Responsible for sending msgs across LPs.
 * If LPs are on the same proc, simply add the msgs to corresponding LP's fel.
 * Otherwise, send it across proc
 */
#include "mpi.h"

#include "LP.h"

//Tag for "regular" msgs
const int MSG_TAG = 0;

void newEpoch();

void sendMsg(Event* event, LPMap lpMap);

/**
 * Receive msg and schedule it on appropriate LP
 */
void receiveMsg(MPI_Status, LPMap lpMap);

int done(MPI_Request req);
