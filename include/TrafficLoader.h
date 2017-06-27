#ifndef TRAFFUTILS_H
#define TRAFFUTILS_H

#include <map>

//own files
#include "SE.h"
#include "Event.h"

using namespace std;

void loadScalefreeTraffic(int rank, int p, string graph_file_name, SE* se, int startEventType, string modelType);

#endif
