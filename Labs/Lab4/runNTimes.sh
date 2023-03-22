#!/bin/bash
#SBATCH --time=00:15:00

### Compiles and runs a given MPI c program.
mpicc q1.c

### Runs program 14 times for each threadcount
for i in {1..14}
do
	mpirun -np 2 ./a.out
done
