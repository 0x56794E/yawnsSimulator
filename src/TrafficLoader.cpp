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
//	for (int rank = 0; rank < intendedPCount; ++rank)
//		loadTrafficForGrid(rank, intendedPCount, gridSize, lpMap);
}
//
//void loadTrafficForGrid(int rank, int p, int gridSize, LPMap &lpMap)
//{
//	ifstream infile (to_string(rank) + "_" + to_string(p) + "_traffic_" + to_string(gridSize));
//	int stopCount, startTime, row, col, eCount = 1;
//
//	//line by line; ea line is an event for an LP
//	string line;
//	while (getline(infile, line))
//	{
//		istringstream iss(line);
//		iss >> stopCount >> startTime;
//		Event* event = new Event(startTime);
//
//		for (int stop = 0; stop < stopCount; ++stop)
//		{
//			iss >> row >> col;
//			event->addStop(coorToId(row, col, gridSize));
//		}
//
//
//		//schedule event on appropriate LP
//		int nextStop = event->peekNextStop();
//		lpMap[nextStop]->scheduleEvent(event);
//		++eCount;
//	}
//
//	printf("Rank %d finished loading traffic\n", rank);
//	infile.close();
//}

void loadScalefreeTraffic(int rank, int p, string graph_file_name, LPMap &lp_map)
{
	//file name format: <graph_file_name>_traffic_p_rank
	string file_name = graph_file_name + "_traffic_" + to_string(p) + "_" + to_string(rank);
	ifstream infile (file_name);

	//Start LP, start time, stop count
	int startLP, startTime, stopCount;

	//line by line; ea line is an event for an LP
	string line;
	while (getline(infile, line))
	{
		istringstream iss(line);
		iss >> startLP >> startTime >> stopCount;

		//Initial event
		Event* event = new Event(startTime, stopCount, startLP);


		//schedule event on appropriate LP
//		int nextStop = 0; //TODO: rand gen!!!
//		lpMap[nextStop]->scheduleEvent(event);
//		++eCount;
	}


	printf("Finishing loading traffic for rank %d\n", rank);
}
