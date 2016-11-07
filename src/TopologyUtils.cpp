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

#include "TopologyUtils.h"
#include <iostream> //for file ops
#include <fstream> //file ops

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


int coorToId(int row, int col, int gridSize)
{
	return row * gridSize + col;
}

void idToCoor(int id, int gridSize, int &row, int &col)
{
	row = id / gridSize;
	col = id % gridSize;
}

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
