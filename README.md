# C++ impl of YAWNS simulator.
Usage:
(1) Gen input: exec gen_input.sh
(2) Run the sim: exec sim with files gen'ed from above

e.g.,
./gen_input.sh 1000 5 4 //Gen graph + traffic for 1000-node scale-free graph with min deg == 5 and the sim is to be run on 4 procs

mpirun -np 4 ./sim g1000_5 //Run the sim (aka cross ur fingers and wait)

# Notes on Graph Loader ##

## Link model:
- Part of graph data for each proc is pre-generated in python => must do this before start the sim
- This is to avoi out of mem error since the num of links can be very large
- Links know of their neighbors via the node they share

## Node model:
- Graph data for each proc is loaded in setup phase

# Notes on Utils ##

## Java 8 REQUIRED!!!