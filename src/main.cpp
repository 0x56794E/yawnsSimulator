#include <stdio.h> //For printf
#include <cstdlib> //For rand gen
#include "mpi.h"   //For MPI stuff

//My stuff
#include "Event.h"
#include "SimExec.h"

using namespace std;

int main(int argc, char* argv[])
{
	//Init MPI
	MPI_Init(&argc, &argv);
	int rank, p;
	
	//Total procs and own rank
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	
	printf("\nRank %d successfully set up system of %d procs", rank, p);
	
	//Set up ran
	int seed = rank; //TODO: ea proc has own seed?
	srand(seed);
	
	//Create the exec
	SimExec se (p, rank);
	se.run();
	
	printf("\nRank %d successfully finished.\n", rank	);
	
	MPI_Finalize();
	return 0; //safe and sound
}
