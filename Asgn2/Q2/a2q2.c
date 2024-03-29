#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <math.h>

/******************************************************************************
 * 	
 * Driver program to calculate a cummation for question 2.
 *
 *  
 * @author	Jordan Alexander Richard
 * @version CS 3123 - Assignment 2 Question 2
******************************************************************************/

/******************************************************************************
 *  Method: summation: Given a value N, calculates a summation in parallel.
 * 
 *  Input:  N - Upper limit of the summation to be calculated.
 * 
 *  Output: Nil
 * 
 * ****************************************************************************/
void summation(int N)
{
    double before = clock();
    double after;
    double timeDifference;

    int m;
    int p;

    int input[N];
    float total;

    srand(time(NULL));

    // Pre-allocating resultant array based on number of running threads
    #pragma omp parallel private(p)
    {
        p = omp_get_thread_num();
        m = omp_get_num_threads();
    }
    float results[m];
    
    #pragma omp parallel private(p)
    {
        p = omp_get_thread_num();
        m = omp_get_num_threads();
        float xi;
        float currentSum;

        // Calculates start and endpoints of each thread
        int start = (p * (N / m));
        int end = ((p + 1) * (N / m) -1);

        //Generates randomized input from 1-10 for Xi
        for(int i = start; i <= end; i++)
        {
            int r = (rand() % (10-1 +1)) + 1;
            input[i] = r;
        }

        //Calculates summation for this thread's values
        for(int j = start; j <= end; j++)
        {
            xi = input[j];
            currentSum += (xi / (xi + 1)) * (pow(-1, j));
        }
        results[p] = currentSum;
    }

    // Combines results and displays summation total
    for(int w = 0; w < m; w++)
    {
        total += results[w];
    }
    printf("Total: %f\n",total);

    after = clock();
    timeDifference = (after - before) / CLOCKS_PER_SEC;
    printf("Processed %i Values in %lf (s)\n",N,timeDifference);
}

int main()
{
    summation(10000);
}