#! /bin/bash

# remake and exec the sim
# Script input:
# (1) proc count
# (2) name of fiile containing graph's edge list

if [ $# -lt 2 ]
then
    echo "Usage: ./run.sh <np> <graph file name> <model type>"
else 
    make clean && make sim
    mpirun -np $1 ./sim $2 $3
fi
