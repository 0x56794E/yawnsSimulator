#ifndef TOPUTILS_H
#define TOPUTILS_H

#include <string>

using namespace std;

/**
 * Given the rank and p count, load the LPs for the proc.
 * as well as the coors limits.
 * input: rank, p
 * output: lpMap, minRow, maxRow, minCol, maxCol
 */
void loadLP(int rank, int p, map<string, LP*> &lpMap, int &minRow, int &maxRow, int &minCol, int &maxCol);

/**
 * Given the row and col pos of a node,
 * construct the ID of the LP repping this node.
 */
string makeLPID(int row, int col);

int coorToId(int row, int col, int gridSize);

void idToCoor(int id, int gridSize);

#endif
