#ifndef TRAFFUTILS_H
#define TRAFFUTILS_H

#include <map>

//own files
#include "LP.h"
#include "EventComparator.h"

using namespace std;

void loadScalefreeTraffic(int rank, int p, string graph_file_name, LPMap &lp_map); //Used for mult-proc, scalefree graph

#endif
