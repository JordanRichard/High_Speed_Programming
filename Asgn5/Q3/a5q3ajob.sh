#!/bin/bash
#SBATCH --time=00:30:00
#SBATCH --ntasks=1               # number of MPI processes
#SBATCH --mem-per-cpu=4G
### Compiles and runs a given c program.
gcc -fopenmp a5q3a.c -lm

### Runs program 14 times for each process count
for nthreads in 16
do
	for i in {1..14}
	do
		./a.out >> outputa$nthreads.txt
	done
done