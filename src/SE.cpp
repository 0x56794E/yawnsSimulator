
//own includes
#include "SE.h"
#include "util/TopologyUtils.h"

/******************
 * PUBLIC members *
 ******************/
SE::SE(int lpCount, int rank)
{
	this->lpCount = lpCount;
	this->rank = rank;

	//Load LPs
	int gridSize = 100;
	loadLP(rank, lpCount, gridSize, lpMap, minRow, maxRow, minCol, maxCol);

	//Init msg count to all 0 for BEFORE the beginning of ea epoch
	for (int i = 0; i < lpC; ++i)
		msgCount[i] = 0;		

	//TODO: gen/load initial packets/msgs here
}

void SE::run()
{
}

int SE::getTotalEvents()
{
	//TODO: get all from all LPs
}

/*******************
 * PRIVATE members *
 *******************/
/**
 * Randomly select a neighbor - i.e., cell with +/- 1 in coors
 */
void getNei(int row, int col, int &nrow, int ncol)
{
	int
}
