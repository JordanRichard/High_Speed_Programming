#!/bin/bash
#SBATCH --time=00:05:00

### Compiles and runs a given c program.
gcc -fopenmp a3q2.c -lm

### Runs program 14 times for each threadcount
for nthreads in 1 2 3 4
do
	for i in {1..14}
	do
		OMP_NUM_THREADS=$nthreads ./a.out >> output$nthreads.txt 
	done
done