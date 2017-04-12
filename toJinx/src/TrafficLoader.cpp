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

/**
 * For ea LP on this proc,
 * schedule the initial event -
 * the packets that originate from the local LP.
 * The file contains ONLY traffic originate from local LPs
 */
void loadScalefreeTraffic(int rank, int p, string graph_file_name, SE* se)
{
	//file name format: <graph_file_name>_traffic_p_rank
	string file_name = graph_file_name + "_traffic_" + to_string(static_cast<long long>(p)) + "_" + to_string(static_cast<long long>(rank));
	ifstream infile (file_name);

	if (infile.is_open())
	{
//		printf("Rank %d start loading traffic \n", rank);
		int eCount = 0;

		//Line format: Start LP, start time, stop count
		int startLP, startTime, stopCount;

		//line by line; ea line is an event for an LP
		string line;
		while (getline(infile, line))
		{
			istringstream iss(line);
			iss >> startLP >> startTime >> stopCount;

			//Initial event
			Event* event = new Event(startTime, stopCount, startLP, 1);

			//schedule event on appropriate LP
			se->scheduleEvent(event);
			//lp_map[startLP]->scheduleEvent(event);
			++eCount;
		}

		infile.close();
//		printf("Finishing loading traffic for rank %d; Total init event = %d\n", rank, eCount);
	}
	else
	{
		cout << "Unable to open file " << file_name << endl;
		exit (4);
	}
}
