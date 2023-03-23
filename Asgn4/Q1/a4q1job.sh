#!/bin/bash
#SBATCH --time=00:05:00

### Compiles and runs a given c program.
mpicc a4q1.c

### Runs program 14 times for each process count
for nthreads in 1 2 4 8
do
	for i in {1..14}
	do
		mpirun -np $nthreads ./a.out >> output&nthreads.txt
	done
done