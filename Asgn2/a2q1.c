#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>

/******************************************************************************
 * 	
 * Driver program to generate random integer array in parallel for question 1.
 *
 *  
 * @author	Jordan Alexander Richard
 * @version CS 3123 - Assignment 2 Question 1
******************************************************************************/

/******************************************************************************
 *  Method: randomIntFrequency
 * 
 * 
 *  Input:	N - The number of random integers to be generated.
 * 
 *  Output: Nil
 * 
 * ****************************************************************************/
 void randomIntFrequency(int N)
 {
 	int m;
	int thread_id;
	int randomIntegers[N];

	srand(time(NULL));

	//Parallel section - handle integer generation and frequency counting
	#pragma omp parallel private(thread_id)
	{
		// TODO -- timer

		thread_id = omp_get_thread_num();
		m = omp_get_num_threads();
	
		int start = (thread_id * (N / m));
		int end = ((thread_id + 1) * (N / m) -1);

		for(int i = start; i <= end; i++)
		{
			//printf("I am Thread #%i: making a value for arr[%i]\n",thread_id,i);
			randomIntegers[i] = (rand() % (10-1 +1)) + 1;
		}

		// TODO -- find counts and create frequency table (Store results in arr)	
	}

	/*
	printf("--------Now in Serial--------\n");
	for(int i=0; i < N; i++)
		{
			printf("[%i]: %i\n",i,randomIntegers[i]);
		}
	*/

	// TODO -- calculate elapsed time

	printf("All done.\n");
 }

int main()
{
	randomIntFrequency(100);
}