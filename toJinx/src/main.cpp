#include <stdio.h> //For printf, NULL
#include <cstdlib> //For rand gen
#include "mpi.h"   //For MPI stuff
#include <time.h> //for time
#include <chrono>

//My stuff
#include "SE.h"
#include "Communicator.h"

using namespace std::chrono;

/**
 * Simple simulation using YAWNS:
 * A network of p processors - fully connected
 * Ea proc when receive a msg, randomly select te next proc to send the package to.
 * Initially, ea proc has some x number of packets
 *
 * IMPORTANT: input - graph file name (at argv[1])
 */
int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		printf("Usage: mpirun -np <p> ./sim <graph_file_name>\n");
		exit(1);
	}

	//Init MPI
	MPI_Init(&argc, &argv);
	int rank, p;

	//Total procs and own rank
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);


	//Set up ran
	srand(time(NULL) + rank);

	//Create the exec
	SE se(p, rank, argv[1]);
	//printf("Rank %d finished setting up\n", rank);

	//Start timer
	MPI_Barrier(MPI_COMM_WORLD);
	auto begin = std::chrono::high_resolution_clock::now();
	printf("Rank %d start running\n", rank);
	se.run();

	MPI_Barrier(MPI_COMM_WORLD);
	double time_ms;
	if (rank ==0)
	{
		auto end = std::chrono::high_resolution_clock::now();
    	time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count(); 
	}

	//Count total events/msgs
	int l_total = se.getTotalProcessedEvent();
	int gl_total;
	MPI_Reduce(&l_total, &gl_total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0)
		printf("\n***SUMMARY:\nTime = %.5f ms; EventCount = %d (local=%d); Concurrency ~= %.5f event/second\n",
				time_ms, gl_total, l_total,  gl_total * 1000.0 / time_ms);

	//Count total msg sent and percentage of interproc comm
	summarizeMsgCount(rank);


	MPI_Finalize();
	return 0; //safe and sound
}
