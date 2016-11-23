#ifndef TRAFFTUILS_H
#define TRAFFUTIL_H

#include <map>

//own files
#include "LP.h"
#include "EventComparator.h"

using namespace std;

void loadTrafficForGrid(int intendedPCount, int gridSize, map<int, LP*> &lpMap); //used for single-proc run
void loadTrafficForGrid(int rank, int p, int gridSize, map<int, LP*> &lpMap);   //used for mult-proc run

#endif
