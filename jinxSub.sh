#!/bin/sh
#PBS -q research
#PBS -l nodes=jinx24
#PBS -N yawnsSim
#PBS -o jinx24.txt

module load openmpi-x86_64
cd $HOME/toJinx
mpirun -np 4 ./sim g2000_5