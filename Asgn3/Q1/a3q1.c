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
 	double before = clock();
 	double after;
 	double timeDifference;

 	int m;
	int p;

	int randomIntegers[N];
	int frequencyTable[10];

	srand(time(NULL));


	// Generates random integers and calulates frequencies in parallel
	#pragma omp parallel for
		for(int i = 0; i < N; i++)
		{
			int r = (rand() % (10-1 +1)) + 1;
			randomIntegers[i] = r;
		}

	// Creates frequency table of each value generated
	#pragma omp parallel for
		for(int j = 0; j < N; j++)
		{
			int value = randomIntegers[j];
			#pragma omp critical
			{
				frequencyTable[value -1] = frequencyTable[value -1] + 1;
			}
		}

	// Serially prints the frequency table 
	printf("Value:\tFrequency:\tRelative:\n");
	for(int i = 0; i < 10; i++)
	{
		float relativeFrequency = (float)frequencyTable[i] / (float)N; //Parallelize?
		printf("[%i] \t%i \t\t%.4f\n", i + 1,frequencyTable[i],relativeFrequency);
	}

	// Calculates and displays elapsed time
	after = clock();
	timeDifference = (after - before) / CLOCKS_PER_SEC;

	printf("Processed %i Values in %lf (s).\n",N,timeDifference);
 }

int main()
{
	randomIntFrequency(10000);
}