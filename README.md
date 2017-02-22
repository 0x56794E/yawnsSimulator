# C++ impl of YAWNS simulator.


# Notes on Graph Loader ##

## Link model:
- Part of graph data for each proc is pre-generated in python => must do this before start the sim
- This is to avoi out of mem error since the num of links can be very large
- Links know of their neighbors via the node they share

## Node model:
- Graph data for each proc is loaded in setup phase

# Notes on Utils ##

## Java 8 REQUIRED!!!