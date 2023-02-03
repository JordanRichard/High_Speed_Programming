#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>

/*
	Task:
			
		pre-parallel: 	create vector of 1,000,000 ints
		random from 0 -> 100

	parallel:	count how many 2's
*/

int main(int a, char **b)
{
	int thread_id;
	int m;
	int twoCount = 0;
	
	int before = clock();
	int after;
	int difference;
	
	srand(time(NULL));
	
	int vector[1000000];
	
	for(int i = 0; i <= 100000; i++)
	{
		vector[i] = rand() % 100; 
	}
	
	#pragma omp parallel private(thread_id)
	{
		thread_id = omp_get_thread_num();
		m = omp_get_num_threads();

		//printf("Hello World! From thread %i of %i\n",thread_id,m);	
	
		for(int i = 0; i <= 100000; i++)
		{
			if(vector[i] == 2)
				twoCount++; 
		}
	}
	
	after = clock();
	difference = after - before;
	
	printf("Total Twos: %i\n",twoCount);
	printf("Elapsed time (ms): %i\n",difference);	
}
