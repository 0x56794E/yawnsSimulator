#PBS -q class
#PBS -l nodes=1:fourcore
#PBS -l walltime=00:05:00
#PBS -N yawnsSimulator OMPI_MCA_mpi_yield_when_idle=0 /opt/openmpi-1.4.3-gcc44/bin/mpirun --hostfile $PBS_NODEFILE -np 4 sim
