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

//My stuff
#include "TopologyUtils.h"

enum MODEL_TYPE {NODE = 0, LINK = 1};

/** 
 * Given the rank, find appropriate input file to load all lps that are supposed to be on this proc.
 * Assume the topology is a grid and the boundaries of the portion a proc is to rep are defined in the file.
 * File has the following format: (inclusive)
 * <min row> <max row> <min col> <max col>
 *
 * @param rank: rank of the proc
 * @param p: number of processors
 */
void loadLP(int rank, int p, int gridSize, map<int, LP*> &lpMap, int &minRow, int &maxRow, int &minCol, int &maxCol)
{
	int id;
	if (p == 1) //If there's only ONE proc => no need to read from lp file. all lps on same proc
	{
		minRow = 0;
		minCol = 0;
		maxRow = gridSize - 1;
		maxCol = gridSize - 1;

		for (int row = 0; row < gridSize; ++row)
		{
			for (int col = 0; col < gridSize; ++col)
			{
				id = coorToId(row, col, gridSize); 
				lpMap[id] = new LP(id);
			}
		}
	}
	else
	{
		ifstream inFile (to_string(rank) + "_" + to_string(p) + "_lps_" + to_string(gridSize));
	
		if (inFile.is_open())
		{
			while (inFile >> minRow >> maxRow >> minCol >> maxCol)
			{
				//Construct all LPs with coors in given range
				for (int row = minRow; row <= maxRow; ++row)
				{
					for (int col = minCol; col <= maxCol; ++col)
					{
						id = coorToId(row, col, gridSize); 
						lpMap[id] = new LP(id);
					}
				}
			}	

			inFile.close();
		}
	}	
}

int coorToId(int row, int col, int gridSize)
{
	return row * gridSize + col;
}

void idToCoor(int id, int gridSize, int &row, int &col)
{
	row = id / gridSize;
	col = id % gridSize;
}

/**
 * TODO: VERY HACKY!!!
 * FIX THIS to handle scale-free case!!
 */
int getRank(int lpId)
{
	int row, col;
	idToCoor(lpId, 100, row, col);
	
	if (row < 50) //rank 0 or 1
	{
		return col < 50 ? 0 : 1;
 	}
	else //rank 2 or 3
	{
		return col < 50 ? 3 : 2;
	}
}

/**
 * Load scale-free topology
 * @param type: model type
 * @param p: number of procs
 * TODO: spec network size and stuff
 */
void loadScalefreeLP(MODEL_TYPE type, int rank)
{
	switch (type)
	{
		case LINK:
			doLoadLink();
			break;

		case NODE:
			doLoadNode();
			break;

		default:
			printf("Unknown node type: %d. Exiting\n", type);
			exit(1);
	}
}

/**
 * Given the IDs of the source and target nodes,
 * return the unique ID for the link
 * Using Cantor Pairing Function
 */
int getLinkId(int srcId, int destId)
{
	return getCode(srcId, destId);
}

//INDEPENDENT of the number of procs being used!!!
string getNeighborFileName(int lpId, string graphFileName)
{
	return graphFileName + "_links/" + lpId + ".txt";
}

void loadNeighbors(LP* lp, string fileName)
{
	ifstream inFile (fileName);
	int neiId;

	if (inFile.is_open())
	{
		while (inFile >> neiId)
		{
			lp->addNeighbor(neiId);
		}

		inFile.close();
	}
}

/**
 * TODO: currently assuming SPACE delim!!!!!
 * fileName: name of the graph file
 * 1a => file containing LPs for proc rank: fileName_p_rank
 */
void doLoadLink(int rank, int p, string fileName, LPMap &lpMap)
{
	//(1) eac proc reads from its own graph file
	//1a. Construct LPMap for the proc
	//1b. Load the set of neighbors for ea LP
	ifstream inFile (fileName + "_"
			+ to_string(p) + "_"
			+ to_string(rank));

	//Each line: <link ID> <src node ID> <dst node ID>
	int linkId, srcId, dstId;
	if (inFile.is_open())
	{
		while (inFile >> linkId >> srcId >> dstId)
		{
			//Construct LP
			lpMap[linkId] = new LP(linkId);

			//Load neighbors
			loadNeighbors(lpMap[linkid]);
		}

		inFile.close();
	}

	//(2) do all-to-all bcast so all procs know which LP on which proc?

}

void doLoadNode()
{

}

