#ifndef TRAFFTUILS_H
#define TRAFFUTIL_H

using namespace std;

/**
 * Given the rank and p count, load the LPs for the proc.
 * as well as the coors limits.
 * input: rank, p
 * output: lpMap, minRow, maxRow, minCol, maxCol
 */
void loadTraffic(int rank, int p, map<string, LP*> &lpMap, int &minRow, int &maxRow, int &minCol, int &maxCol);



#endif
