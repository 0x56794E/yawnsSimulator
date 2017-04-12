#!/bin/sh

# load MPI module and compile
module load openmpi-x86_64

pbsnodes | grep -B 1 "state = free" | grep jinx | while read nodename; do
    echo "#!/bin/sh" > myscript.sh
    echo "#PBS -q class" >> myscript.sh
    echo "#PBS -l nodes=$nodename" >> myscript.sh
    echo "#PBS -N mpi-test" >> myscript.sh
    echo "#PBS -o output/${nodename}.txt" >> myscript.sh
    cat >>myscript.sh <<EOF
'EOF'
module load openmpi-x86_64
echo "====================="
cat $PBS_NODEFILE
mpicc --version
#echo "ERROR: mpicc is not available"
#echo "Nodefile is: $SLURM_JOB_NODELIST"
#MPIRUN=$(which mpirun)
#echo "mpirun is $MPIRUN"
#echo "MPI Version"
mpirun --version
#cd $HOME/mpi-test
cd $HOME/mpi-test
#mpicxx -o test test.cpp
mpirun -np 4 ./test
EOF
    qsub myscript.sh
    # wait for result to be there
    echo "waiting of job on $nodename to complete..."
    while [[ ! -e "output/${nodename}.txt" ]]; do
        sleep 1
    done
    echo "completed"
done


#echo $REST > myscript.sh
EOF
