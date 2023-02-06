#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>

/******************************************************************************
 * 	
 * Driver program to carry out parallel matrix addition for question 3.
 *
 *  
 * @author	Jordan Alexander Richard
 * @version CS 3123 - Assignment 2 Question 3
******************************************************************************/

/******************************************************************************
 *  Method: summation: Given a value N, generates two NxN matrices of randomly
 *          generated double values and adds them together in parallel.
 * 
 *  Input:	N - The size (order) of matrices to be created.
 * 
 *  Output: Nil
 * 
 * ****************************************************************************/
void matrixAddition(int N)
{
    double before = clock();
    double after;
    double timeDifference;

    double arr1[N][N];
    double arr2[N][N];
    double arr3[N][N];

    int p;
    int m;

    srand(time(NULL));

    #pragma omp parallel private(p)
    {
        p = omp_get_thread_num();
        m = omp_get_num_threads();
        
        // Calculates start and endpoints of each thread
        int start = (p * (N / m));
        int end = ((p + 1) * (N / m) -1);

        double r;
        double r2;
        
        // Populates each matrix with random double values
        for(int i = start; i <= end; i++)
        {
            for(int j = 0; j <= N; j++)
            {
                r = (double)(rand());
                r2 =(double)(rand());

                arr1[i][j] = r;
                arr2[i][j] = r2;
            }  
        }

        // Adds the two matrices together
        for(int x = start; x <= end; x++)
        {
            for(int y = 0; y <= N; y++)
            {
                double arrSum = arr1[x][y] + arr2[x][y];
                arr3[x][y] = arrSum;
            }
        }
    }

    after = clock();
    timeDifference = (after - before) / CLOCKS_PER_SEC;
    printf("Added two %iX%i matrices in %lf (s).\n",N,N,timeDifference);
}


int main()
{
    matrixAddition(10000);
}