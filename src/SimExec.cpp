#include "stdio.h"

#include "SimExec.h"

SimExec::SimExec(int lpC, int r)
{
	lpCount = lpC;
	rank = r;
}

/**
 * Start event processing
 */
void SimExec::run()
{
	for (int i = 0; i < rank; ++i)
		printf("\nRank %d is processing event %d", rank, i);
}


//Private stuff
void SimExec::handleEvent(Event e)
{
	printf("Handling events");
}
