#!/bin/bash
#SBATCH --time=00:30:00
#SBATCH --ntasks=1               # number of MPI processes
#SBATCH --mem-per-cpu=4G
### Compiles and runs a given c program.
mpicc a5q1.c -lm

### Runs program 14 times for each process count
for nthreads in 16
do
	for i in {1}
	do
		mpirun -np $nthreads ./a.out >> output$nthreads.txt
	done
done