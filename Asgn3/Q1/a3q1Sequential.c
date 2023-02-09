#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/******************************************************************************
 * 	
 * Driver program to create a random integer array and frequency table for 
 * the sequential variant question 1.
 *
 *  
 * @author	Jordan Alexander Richard
 * @version CS 3123 - Assignment 3 Question 1 (Sequential Variant)
******************************************************************************/

/******************************************************************************
 *  Method: randomIntFrequency: Given a value N, generates an array of N random 
 *          integers sequentially and creates a frequency table.
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

	int randomIntegers[N];
	int frequencyTable[10];

	srand(time(NULL));

	// Sequentially generates random integers and calulates relative frequencies
	for(int i = 0; i < N; i++)
	{
		int r = (rand() % (10-1 +1)) + 1;
		randomIntegers[i] = r;
	}

	// Creates frequency table of each value generated
	for(int j = 0; j < N; j++)
	{
		int value = randomIntegers[j];
		frequencyTable[value -1] = frequencyTable[value -1] + 1;
	}

	// Calculates and displays elapsed time
	after = clock();
	timeDifference = (after - before) / CLOCKS_PER_SEC;

	// Displays the frequency table 
	printf("Value:\tFrequency:\tRelative:\n");
	for(int i = 0; i < 10; i++)
	{
		float relativeFrequency = (float)frequencyTable[i] / (float)N;
		printf("[%i] \t%i \t\t%.4f\n", i + 1,frequencyTable[i],relativeFrequency);
	}

	printf("Processed %i Values in %lf (s).\n",N,timeDifference);
 }

int main()
{
	randomIntFrequency(10000);
}