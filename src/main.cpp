#include <stdio.h> //For printf, NULL
#include <cstdlib> //For rand gen
#include "mpi.h"   //For MPI stuff
#include <time.h> //for time
#include <chrono>
#include <string> //for string
#include <sstream> // for std::stringstream

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
 *		    - model type at argv[2] => if not spec, default to node
 */
int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		printf("Usage: mpirun -np <p> ./sim <graph_file_name> <model type: 0 = node, 1 = link>\n");
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
	MODEL_TYPE type;
	int tmp = 0;

	//If type is specifiled
	if (argc == 3)
	{
		std::stringstream convert(argv[2]);
		if (!(convert >> tmp)) // do the conversion
			tmp = 0; //if fail, default to 0 == node
	}
	printf ("Running %s model...\n", 
			tmp ? "LINK" : "NODE");
	type = tmp ? LINK : NODE;
	SE se(p, rank, argv[1], type);
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

	
	//************************
	//PER-EPOCH measurement    
    // this will affect parallelism of link model.
    //DO NOT leave this on when measuring parallelism
	//************************
    
    se.outputPerEpochStats();
	//************************
	//CONCURRENCY measurement
	//************************
	//Count total events/msgs
/*
	int l_total = se.getTotalProcessedEvent();
	int gl_total;
	MPI_Reduce(&l_total, &gl_total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0)
		printf("\n***SUMMARY:\nEventCount = %d (local=%d); Time = %.5f ms; Concurrency ~= %.5f event/second\n",
				gl_total, l_total,  time_ms, gl_total * 1000.0 / time_ms);
*/
	//Count total msg sent and percentage of interproc comm
	//summarizeMsgCount(rank);
	//************************

	MPI_Finalize();
	return 0; //safe and sound
}
