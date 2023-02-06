#!/bin/bash
#SBATCH --time=00:01:00
#SBATCH --cpus-per-task=4

### Compiles and runs a given c program.
gcc -fopenmp a2q2.c -lm

### Runs 14 times
for i in {1..14}
do
	./a.out
	sleep 1
done

