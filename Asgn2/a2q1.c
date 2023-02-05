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
 *  Method: randomIntFrequency: Given a value N, this function randomly 
 * 			generates N values from 1-10, then counts the frequency of each 
 * 			value in parallel.
 * 
 *  Input:	N - The number of random integers to be generated.
 * 
 *  Output: Nil
 * 
 * ****************************************************************************/
 void randomIntFrequency(int N)
 {
 	int before = clock();
 	int after;
 	int timeDifference;

 	int m;
	int thread_id;

	int randomIntegers[N];
	int frequencyTable[10];

	srand(time(NULL));


	// Generates random integers and calulates frequencies in parallel
	#pragma omp parallel private(thread_id)
	{
		thread_id = omp_get_thread_num();
		m = omp_get_num_threads();
	
		// Calculates start and endpoints of each thread
		int start = (thread_id * (N / m));
		int end = ((thread_id + 1) * (N / m) -1);

		//Generates randomized value between 1-10 and adds to array
		for(int i = start; i <= end; i++)
		{
			int r = (rand() % (10-1 +1)) + 1;
			randomIntegers[i] = r;
		}

		// Creates frequency table of each value generated
		for(int j = start; j <= end; j++)
		{
			int value = randomIntegers[j];
			frequencyTable[value -1] = frequencyTable[value -1] + 1;
		}
	}


	// Serially builds and prints the frequency table 
	printf("Value:\tFrequency:\tRelative:\n");
	for(int i=0; i < 10; i++)
	{
		float relativeFrequency = (float)frequencyTable[i] / (float)N;
		printf("[%i] \t%i \t\t%.4f\n", i+1,frequencyTable[i],relativeFrequency);
	}


	// Calculates and displays elapsed time
	after = clock();
	timeDifference = after- before;

	printf("Processed %i Values in %i (ms).\n",N,timeDifference);
 }

int main()
{
	randomIntFrequency(500);
}