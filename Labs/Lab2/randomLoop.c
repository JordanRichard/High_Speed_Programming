#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>

#define m 8

/******************************************************************************
 * 	
 * Driver program for lab q1.
 *
 *  
 * @author	Jordan Alexander Richard
 * @version CS 3123 - Lab2 Q1
******************************************************************************/

/******************************************************************************
 *  Method: randomLoop: generates 2 random values, adds them, and stores the 
 * 			result.
 * 
 *  Input:  N - The number of values to be processed.
 * 
 *  Output: Nil
 * 
 * ****************************************************************************/

 void randomLoop(int N)
 {
 	int a,b,c;
 	float percentage;
	
	srand(time(NULL));

	double diff;

	#pragma omp parallel private(a,b,c,percentage) reduction(max: diff)
	{
		int iterations = 0;

		double start = omp_get_wtime();

		#pragma omp for schedule(dynamic)
			for(int i = 0; i < N; i++)
			{
				a = rand();
				b = rand();
				c = a + b;
				iterations ++;
			}
		int p = omp_get_thread_num();
		
		percentage = 100*(iterations / (float)N);

		double end = omp_get_wtime();
		diff = end - start;
		

		printf("Thread %i ran %i times. (%.2f%%)\n",p,iterations,percentage);
	}

	printf("%f\n",diff);
}

int main()
{
	omp_set_num_threads(m);
	randomLoop(25000000);
}