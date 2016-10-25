#ifndef TRAFFTUILS_H
#define TRAFFUTIL_H

#include <map>

//own files
#include "LP.h"
#include "EventComparator.h"

using namespace std;

void loadTrafficMultiForGrid(int rank, int p, int gridSize, map<int, LP*> &lpMap);

#endif
