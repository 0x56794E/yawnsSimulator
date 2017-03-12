/***
 * Utils for generate topologies or to convert that from SNAPPY into desired format.
 * (1) Convert SNAPPY output graph file into desired format:
 *     - Input: file from SNAPPY, number of processors p
 *     - Output: p files named <rank>_graph specifying the portion of the graph
 *     processor with rank is responsible for.
 * (2) Generate a square grid topology
 *     - Input: size of the grid; number of processors p
 *     - Output: p files named <rank>_graph for the ea proc
 * (3) Load file for the proc and construct LPs
 *    
 * When used, ea proc load its own file and distribute the packets among the LPs
 * Each line in this file has the following format:
 * <startTime> <numStops> <stop0> <stop1> ... <stopn>
 */

#include <iostream> //for file ops
#include <fstream> //file ops
#include <stdio.h>      /* printf, fopen */
#include <stdlib.h>     /* exit, EXIT_FAILURE */
#include <string>
#include <mpi.h> //for mpi stuff
#include <math.h> //for floor stuff

//My stuff
#include "TopologyUtils.h"

/**
 * IDEALLY, this should be called ONCE at the beginning.
 * Keep a map or sth
 * @param: id of lp rep link
 * @param: total number of links
 * @param: total number of procs
 * @return rank the link (lp) is on
 */
int getLinkRank(int lpId, int n, int p)
{
	int rem = n % p;
	int counts[p]; //number of links for ea proc
	int count = (int) floor(n/p);

	for (int i = 0; i < p; ++i)
		counts[i] = count;

	//Additionally, first "rem" proc has 1 more
	for (int i = 0; i < rem; ++i)
		counts[i] += 1;

}

/**
 * Load scale-free topology
 * @param type: model type
 * @param rank: the rank of the proc
 * @param p: number of procs
 * @param fileName: name of the file containing the graph's edge list
 * @param lpMap: the lpMap for this proc (ONLY one instance per proc)
 * TODO: spec network size and stuff
 */
void loadScalefreeLP(MODEL_TYPE type, int rank, int p, string fileName, LPMap &lpMap, map<int, pair<int, int>> &rankMap)
{
	switch (type)
	{
		case LINK:
			doLoadLink(rank, p, fileName, lpMap, rankMap);
			break;

		case NODE:
			doLoadNode();
			break;

		default:
			printf("Unknown node type: %d. Exiting\n", type);
			exit(1);
	}
}

//INDEPENDENT of the number of procs being used!!!
string getNeighborFileName(int lpId, string graphFileName)
{
	string ret = graphFileName + "_link_nei/" + to_string(lpId) + ".txt";
	return ret;
}

void loadNeighbors(LP* lp, string graph_file_name)
{
	string nei_file = getNeighborFileName(lp->getId(), graph_file_name);
	ifstream inFile (nei_file);
	int neiId;

	if (inFile.is_open())
	{
		while (inFile >> neiId)
		{
			lp->addNeighbor(neiId);
		}

		inFile.close();
	}
	else
	{
		cout << "ERROR: cannot open file: " << nei_file << endl;
		exit(1);
	}
}

/**
 * TODO: currently assuming SPACE delim!!!!!
 * fileName: name of the graph file
 * 1a => file containing LPs for proc rank: fileName_p_rank
 */
void doLoadLink(int rank, int p, string fileName, LPMap &lpMap, map<int, pair<int, int>> &rankMap)
{
	//(1) eac proc reads from its own graph file
	//1a. Construct LPMap for the proc
	//1b. Load the set of neighbors for ea LP
	ifstream inFile (fileName + "_"
			+ to_string(p) + "_"
			+ to_string(rank));

	vector<int> linkIds;

	//Each line: <link ID> <src node ID> <dst node ID>
	int linkId, srcId, dstId;
	if (inFile.is_open())
	{
		while (inFile >> linkId >> srcId >> dstId)
		{
			//Construct LP
			lpMap[linkId] = new LP(linkId);
			linkIds.push_back(linkId);

			//Load neighbors for ea link
			loadNeighbors(lpMap[linkId], fileName);
		}
		inFile.close();
	}

	//(2) Load the rank map so all procs know which LP on which proc?
	//Do all gather on the size
	int l_size = linkIds.size(); //local
	int g_sizes[p]; //global
	int min, max = -1;
	MPI_Allgather(&l_size, 1, MPI_INT, &g_sizes, 1, MPI_INT, MPI_COMM_WORLD);

	//Load the rank map
	for (int i = 0; i < p; ++i)
	{
		min = max + 1;
		max = min + g_sizes[i] - 1;
		rankMap[i] = pair<int,int>(min, max);
	}
}

void doLoadNode()
{

}

