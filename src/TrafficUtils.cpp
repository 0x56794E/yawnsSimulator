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

const int MAX_PACKET_CT = 100; //max num of packets
const int MAX_STOP_CT = 10; //max num of stops


int main(int argc, char* argv[])
{	

	return 0;	
}

void loadTrafficMultiForGrid(int p, int rank, int gridSize, map<int, LP*> &lpMap)
{
	ifstream infile (to_string(rank) + "_" + to_string(p) + "_traffic_" + to_string(gridSize));
	int stopCount, startTime, row, col;
	
	//line by line; ea line is an event for an LP
	string line;
	while (getline(infile, line))
	{
		istringstream iss(line);
		if (!(iss >> stopCount >> startTime))
			break;
		
		Event event* = new Event(startTime, stopCount);

		for (int stop = 0; stop < stopCount; ++stop)
		{
			iss >> row >> col;
			event->addStop(coorToId(row, col, gridSize));
		}

		//schedule event on appropriate LP
		int nextStop = event->nextStop();
		lpMap[nextStop]->scheduleEvent(event);
	}
}

/**
 * Generate traffic for grid topology
 *
 * input: p the number of processors
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
		//Format: <stop count> <start time> <src row> <src col> <stop 1 row> <stop 1 col> ...
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
						ccol =col;
				
						outfile << numStop << " " << rand() << " " << crow << " " << ccol;
						for (int j = 1; j < numStop; ++j)
						{
							nrow = rand() % gridSize;
							while (nrow - crow > 1 || nrow - crow < -1)
								nrow = rand() % gridSize;
						
							ncol = rand() % gridSize;
							while (ncol - ccol > 1 || ncol - ccol < -1)
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
	

/**
 * Use for simple case where ea LP rep one node.
 * and fully connected topology is assumed.
 */
void genTrafficSimple()
{
 	int lpCount = 2; //input params
	srand(time(NULL));
	int numPackets, numStops, startTs, curStop, nextStop;

	for (int i = 0; i < lpCount; ++i)
	{
		string fileName = to_string(i) + "_input";
		ofstream myfile (fileName);
	
		if (myfile.is_open())
		{
			numPackets = rand() % MAX_PACKET_CT;

			for (int j = 0; j < numPackets; ++j)
			{
				startTs = rand();
				numStops = rand() % MAX_STOP_CT + 1;

				myfile << startTs << " " << numStops;

				//Generate the stops:
				nextStop = i;
				for (int k = 0; k < numStops; ++k)
				{
					curStop = nextStop;
					nextStop = rand() % lpCount;
					while (nextStop == curStop)
						nextStop = rand() % lpCount;

					myfile << " " << nextStop;
				}
				myfile << "\n";
			}
			myfile.close();
		}
	}
}
