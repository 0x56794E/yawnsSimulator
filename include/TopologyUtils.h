#ifndef TOPUTILS_H
#define TOPUTILS_H

#include <map>
#include <string>

#include "LP.h" 

using namespace std;

enum MODEL_TYPE {NODE = 0, LINK = 1};

//void loadScalefreeLP(MODEL_TYPE type, int rank, int p, string fileName, LPMap &lpMap, map<int, pair<int, int>> &rankMap);

void doLoadLink(int rank, int p, string fileName, LinkLPMap &lpMap, map<int, pair<int, int>> &rankMap);
void doLoadNode(int rank, int p, string fileName, NodeLPMap &lpMap, map<int, pair<int, int>> &rankMap);

#endif
