#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>

/******************************************************************************
 * 	
 * Driver program to create a random integer array and frequency table for 
 * question 1.
 *
 *  
 * @author	Jordan Alexander Richard
 * @version CS 3123 - Assignment 3 Question 1
******************************************************************************/

/******************************************************************************
 *  Method: randomIntFrequency: Given a value N, generates an array of N random 
 *          integers in parallel and creates a frequency table, updated to use
 * 			OMP commands.
 * 
 *  Input:  N - The number of values to be processed.
 * 
 *  Output: Nil
 * 
 * ****************************************************************************/
 void randomIntFrequency(int N)
 {
 	double timeDifference;

	int randomIntegers[N];
	int frequencyTable[10];

	srand(time(NULL));

	#pragma omp parallel reduction(max: timeDifference)
	{
		double before = clock();
		srand(time(NULL));

		// Generates random integers and calulates frequencies in parallel
		#pragma omp for
			for(int i = 0; i < N; i++)
			{
				randomIntegers[i] = (rand() % (10-1 +1)) + 1;
			}

		// Creates frequency table of each value generated
		#pragma omp for
			for(int j = 0; j < N; j++)
			{
				int value = randomIntegers[j];
				#pragma omp critical
				{
					frequencyTable[value -1] = frequencyTable[value -1] + 1;
				}
			}

		// Calculates and displays elapsed time
		double after = clock();
		timeDifference = (after - before) / CLOCKS_PER_SEC;
	}

	/*
	// Serially prints the frequency table 
	printf("Value:\tFrequency:\tRelative:\n");
	for(int i = 0; i < 10; i++)
	{
		float relativeFrequency = (float)frequencyTable[i] / (float)N;
		printf("[%i] \t%i \t\t%.4f\n", i + 1,frequencyTable[i],relativeFrequency);
	}
	*/

	printf("%lf\n",timeDifference);
 }

int main()
{
	randomIntFrequency(10000);
}