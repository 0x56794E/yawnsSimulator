#ifndef TOPUTILS_H
#define TOPUTILS_H

#include <map>
#include <string>

#include "LP.h" 

using namespace std;

/**
 * Given the rank and p count, load the LPs for the proc.
 * as well as the coors limits.
 * input: rank, p
 * output: lpMap, minRow, maxRow, minCol, maxCol
 */
void loadLP(int rank, int p, int gridSize, LPMap &lpMap, int &minRow, int &maxRow, int &minCol, int &maxCol);

/**
 * Given the row and col pos of a node,
 * construct the ID of the LP repping this node.
 */
string makeLPID(int row, int col);

int coorToId(int row, int col, int gridSize);

void idToCoor(int id, int gridSize);

int getRank(int lpId);

void doLoadLink(int rank, int p, string fileName, LPMap &lpMap);

void doLoadNode();

#endif
