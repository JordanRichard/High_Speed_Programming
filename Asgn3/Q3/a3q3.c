#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>

/******************************************************************************
 * 	
 * Driver program to carry out parallel matrix addition for question 3. 
 * Modified to use OMP commands.
 *
 *  
 * @author	Jordan Alexander Richard
 * @version CS 3123 - Assignment 3 Question 3
******************************************************************************/

/******************************************************************************
 *  Method: matrixAddition: Given a value N, generates two NxN matrices of 
 *          randomly generated double values and adds them together in 
 *          parallel.
 * 
 *  Input:  N - The size (order) of matrices to be created.
 * 
 *  Output: Nil
 * 
 * ****************************************************************************/
void matrixAddition(int N)
{

    double timeDifference;

    double arr1[N][N];
    double arr2[N][N];
    double arr3[N][N];

    
    #pragma omp parallel reduction(max: timeDifference)
    {
        double before = clock();
        srand(time(NULL));

        // Populates each matrix with random double values
        #pragma omp for
            for(int i = 0; i < N; i++)
            {
                for(int j = 0; j < N; j++)
                {
                    arr1[i][j] = (double)(rand());
                    arr2[i][j] = (double)(rand());
                }  
            }

        // Adds the two matrices together
        #pragma omp for
            for(int x = 0; x <= N; x++)
            {
                for(int y = 0; y <= N; y++)
                {
                    arr3[x][y] = arr1[x][y] + arr2[x][y];
                }
            }
        
        double after = clock();
        timeDifference = (after - before) / CLOCKS_PER_SEC;
    }

    // Displays elapsed runtime
    printf("%lf\n",timeDifference);
}


/******************************************************************************
 *  Method: main: Calls the method required providing an argument N, the order
 *			of matrix elements to process.
 * 
 *  Input:  Nil
 * 
 *  Output: Nil
 * 
 * ****************************************************************************/
int main()
{
    matrixAddition(10000);
}