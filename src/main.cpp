#include <stdio.h> //For printf, NULL
#include <cstdlib> //For rand gen
#include "mpi.h"   //For MPI stuff
#include <time.h> //for time
#include <chrono>

//My stuff
#include "Event.h"
#include "SimExec.h"

using namespace std::chrono;

/**
 * Simple simulation using YAWNS:
 * A network of p processors - fully connected
 * Ea proc when receive a msg, randomly select te next proc to send the package to.
 * Initially, ea proc has some x number of packets
 */
int main(int argc, char* argv[])
{
	//Init MPI
	MPI_Init(&argc, &argv);
	int rank, p;
	
	//Total procs and own rank
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	
	//Set up ran
	//TODO: ea proc has own seed?
	srand(time(NULL) + rank);
	
	//Start timer
	MPI_Barrier(MPI_COMM_WORLD);
	auto begin = std::chrono::high_resolution_clock::now();

	//Create the exec
	SimExec se(p, rank);
	se.run();

	int l_total = se.getTotalEvent();
	int gl_total;
	MPI_Reduce(&l_total, &gl_total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	//printf("\nRank %d successfully finished with %d.\n", rank, l_total);

	if (rank ==0)
	{
		auto end = std::chrono::high_resolution_clock::now();
    	double time_secs = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count(); 
		printf("\n*** TOTAL events: %d; time = %.5f ms; parallel = %.5f event/ms\n", 
				gl_total, time_secs, gl_total / time_secs);
	}

	MPI_Finalize();
	return 0; //safe and sound
}
