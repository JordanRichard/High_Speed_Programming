#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <math.h>

/******************************************************************************
 * 	
 * Driver program to calculate a summation for question 2.
 *
 *  
 * @author	Jordan Alexander Richard
 * @version CS 3123 - Assignment 3 Question 2
******************************************************************************/

/******************************************************************************
 *  Method: summation: Given a value N, calculates a summation in parallel. 
 *          Updated to use OMP commands.
 * 
 *  Input:  N - Upper limit of the summation to be calculated.
 * 
 *  Output: Nil
 * 
 * ****************************************************************************/
void summation(int N)
{

    double timeDifference;
    double xi = 0;
    double totalSum = 0;

    int input[N];
    
    srand(time(NULL));
    #pragma omp parallel reduction(max: timeDifference)
    {
        double before = clock();
        double after = 0;

        //Generates randomized input from 1-10 for Xi
        #pragma omp for
            for(int i = 0; i <= N; i++)
            {
                int r = (rand() % (10-1 +1)) + 1;
                input[i] = r;
            }

        //Calculates summation for this thread's values
        #pragma omp parallel for reduction(+ : totalSum)
            for(int j = 0; j <= N; j++)
            {
                xi = input[j];
                totalSum += (xi / (xi + 1)) * (pow(-1, j));
            }

        after = clock();
        timeDifference = (after - before) / CLOCKS_PER_SEC;
    }
    printf("Total: %f\n",totalSum);
    printf("Processed %i Values in %lf (s)\n",N,timeDifference);
}

int main()
{
    summation(10000);
}