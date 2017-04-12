#ifndef TRAFFUTILS_H
#define TRAFFUTILS_H

#include <map>

//own files
#include "SE.h"
#include "EventComparator.h"

using namespace std;

void loadScalefreeTraffic(int rank, int p, string graph_file_name, SE* se); //Used for mult-proc, scalefree graph

#endif
