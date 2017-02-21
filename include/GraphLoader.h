#ifndef GRAPH_H
#define GRAPH_H

#include <string> //for string
#include <vector> //for vector

using namespace std;

/**
 * Read from the given file (containing edge list of the graph)
 * Compute the ID for ea of these edge \
 * => Do NOT use Cantor => just assign ID as the edges are read
 * => batch processing
 * Return the list of those IDs
 */
vector<int> loadEdgeList(string filename);

/**
 * Distribute//bcast the list of edge also the mapping
 */
void distributeEdgeIds(int p);

#endif
