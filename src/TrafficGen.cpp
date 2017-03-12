/***
 * Generate traffic and save it to file such that each rank has its own file named as follows:
 * <rank>_input
 * When used, ea proc load its own file and distribute the packets among the LPs
 * Each line in this file has the following format:
 * <startTime> <numStops> <stop0> <stop1> ... <stopn>
 */
#include <iostream> //for reading form file
#include <fstream>  //ditto
#include <string>   //ditto
#include <cstdlib> //for rand
#include <stdio.h> //For printf, NULL
#include <sstream> //for line by line parsign

using namespace std;

const int MAX_PACKET_CT = 3; //max num of packets
const int MAX_STOP_CT = 3; //max num of stops

/**
 * Generate traffic for grid topology
 *
 * input: p the number of processors and grid size
 */
void genTrafficMultiForGrid(int p, int gridSize)
{
	int minRow, maxRow, minCol, maxCol;
	srand(time(NULL));

	int numPacket, numStop, startTime, crow, ccol, ncol, nrow;

	for (int rank = 0; rank < p; ++rank)
	{
		//For each proc, red the topology file to get the limits
		ifstream infile (to_string(rank) + "_" + to_string(p) + "_lps_" + to_string(gridSize));
	
		//Output file for ea proc
		//Format ea line: <stop count> <start time> <src row> <src col> <stop 1 row> <stop 1 col> ...
		ofstream outfile (to_string(rank) + "_" + to_string(p) + "_traffic_" + to_string(gridSize));
		if (infile.is_open() && outfile.is_open())
		{
			infile >> minRow >> maxRow >> minCol >> maxCol;
			
			//Generate traffic for all LPs on this proc
			for (int row = minRow; row <= maxRow; ++row)
			{
				for (int col = minCol; col <= maxCol; ++col)
				{
					//Gen num of packets
					numPacket = rand() % MAX_PACKET_CT;

					for (int i = 0; i < numPacket; ++i)
					{
						numStop = rand() % MAX_STOP_CT + 2;
						crow = row;
						ccol = col;
				
						outfile << numStop << " " << rand() << " " << crow << " " << ccol;
						for (int j = 1; j < numStop; ++j)
						{
							//Making sure the next stop is one of the neighbors
							//And that the next stop != current stop
							nrow = rand() % gridSize;
							while (nrow - crow > 1 || nrow - crow < -1)
								nrow = rand() % gridSize;
						
							ncol = rand() % gridSize;
							while (ncol - ccol > 1 || ncol - ccol < -1 || nrow == crow && ncol == ccol)
								ncol = rand() % gridSize;

							outfile << " " << nrow << " " << ncol;

							//update crow and ccol
							crow = nrow;
							ccol = ncol;
						}
						outfile << '\n';
					}
				}
			}

			infile.close();
			outfile.close();
		}
	}
}
	
//OBSOLETE!!! See java version
//Generate traffic for scale-free topology
void genTrafficForScalefree(string graphFileName, int p)
{
	//General idea:
	//Gen start stop, start time, stop count
	//At ea stop, the handler will randomly gen the next stop
	// The next stop is randomly chosen among the neighbors
	// of the current stop

	//Output format:
	//Each line:
	//<start_stop/LP's ID> <start_time> <stop_count>
	//File name format: <graph_file_name>_p_rank
	//where p: num of proc to be used
	//rank: the rank of the proc this file is for
	//

	srand(time(NULL));

	int numPacket, numStop, startTime;

	for (int rank = 0; rank < p; ++rank)
	{
		//ofstream outfile (to_string(rank) + "_" + to_string(p) + "_traffic_" + to_string(gridSize));
		ofstream outfile (graphFileName + "_traffic_" + to_string(p) + "_" + to_string(rank) + ".txt" );

		//Get all LPs on this proc
		//lp list file name: <graphFileName>_p_rank
		ifstream infile (graphFileName + "_" + to_string(p) + "_" + to_string(rank));


		//Gen the traffic for ea LP
		//TODO:

		outfile.close();
		infile.close();

	}
}

int main(int argc, char* argv[])
{	
	genTrafficMultiForGrid(4, 20);
	return 0;	
}
