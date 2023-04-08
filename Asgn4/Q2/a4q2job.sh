#!/bin/bash
#SBATCH --time=00:10:00
#SBATCH --ntasks=8               # number of MPI processes
### Compiles and runs a given c program.
mpicc a4q2.c -lm

### Runs program 14 times for each process count
for nthreads in 1 2 4 8
do
	for i in {1..14}
	do
		mpirun -np $nthreads ./a.out >> output$nthreads.txt
	done
done