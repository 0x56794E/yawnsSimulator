/***
 * For loading traffic
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

//own files
#include "TrafficLoader.h"
#include "TopologyUtils.h"
#include "Event.h"
#include "LP.h"

using namespace std;

//parallelPCount: the num of proc used when traffic was gen
void loadTrafficForGrid(int intendedPCount, int gridSize, map<int, LP*> &lpMap)
{
	for (int rank = 0; rank < intendedPCount; ++rank)
		loadTrafficForGrid(rank, intendedPCount, gridSize, lpMap);
}

void loadTrafficForGrid(int rank, int p, int gridSize, map<int, LP*> &lpMap)
{
	ifstream infile (to_string(rank) + "_" + to_string(p) + "_traffic_" + to_string(gridSize));
	int stopCount, startTime, row, col, eCount = 1;
	
	//line by line; ea line is an event for an LP
	string line;
	while (getline(infile, line))
	{
		istringstream iss(line);
		iss >> stopCount >> startTime;
		Event* event = new Event(startTime);
		
		for (int stop = 0; stop < stopCount; ++stop)
		{
			iss >> row >> col;
			event->addStop(coorToId(row, col, gridSize));
		}

		
		//schedule event on appropriate LP
		int nextStop = event->peekNextStop();
		lpMap[nextStop]->scheduleEvent(event);
		++eCount;
	}

	printf("Rank %d finished loading traffic\n", rank);
	infile.close();
}
