#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <math.h>

/******************************************************************************
 * 	
 * Driver program to carry out gauss-jacobi iterations to solve a system AX = b
 * in parallel using openmp.
 *
 *  
 * @author	Jordan Alexander Richard
 * @version CS 3123 - Assignment 5 Question 3a
******************************************************************************/

/******************************************************************************
 *  Method: jacobi: Randomly generates a diagonally-dominant NxN system a and
 *              solves in parallel with openmp using the gauss-jacobi iterative
 *              method. Stops iterating and displays the approximate solution
 *              after a precision of  < 0.000001 is reached.
 * 
 *  Input:  N - The size of system to generate and solve
 * 
 *  Output: Nil
 * 
 * ****************************************************************************/
void jacobi(int N)
{
    double before,after,timeDifference;
    double rowSum = 0,xParts = 0;
    int i,j,flag = 0,iters = 0;
    double epsilon = 0.000001;

    double a[N][N];
    double x[N];
    double b[N];
    double newX[N];

    omp_set_num_threads(16);

    srand(time(NULL));
    
    //  Randomly generate a NxN matrix and ensure it to be diagonally dominant,
    //  which is required to ensure our answer converges using the jacobi method
    for (i = 0; i < N; i++) 
    {
        rowSum = 0;
        for (j = 0; j < N; j++) 
        {
            //  Generates a random number between -10 and 10
            a[i][j] = (double)(rand() % 21 - 10); 
            rowSum += fabs(a[i][j]);
        }
        //  Add 1:10 to ensure this row is diagonally dominant after filling it
        a[i][i] = rowSum + (double)(rand() % 10 + 1); 
    }
    
    //  Randomly initialize the vector b, and set the initial "guess" for vector X to 0's
    for (i = 0; i < N; i++) 
    {
        b[i] = (double)(rand() % 21 - 10);
        x[i] = 0;
    }

    //  Start timing
    before = clock();

    //  Continue carrying out jacobi iterations until exit condition is met
    while(flag != 1)
    {
        // Carry out one iteration of the gauss-jacobi method in parallel
        #pragma omp parallel
        {
            #pragma omp for
                for(i = 0; i < N; i++)
                {
                    xParts = b[i];
                    for(j = 0; j < N; j++)
                    {
                        //  If we're NOT on the diagonal
                        if(j != i)
                        {  
                            xParts -= a[i][j] * x[j];
                        }
                    }
                    newX[i] = xParts / a[i][i];
                }            
        }

        //  Check difference in calculated values and stop iterating if small enough
        //  Cannot be parallelized due to break statement
        flag = 1;
        for(i = 0; i < N; i++)
        {
            //Stop testing if any larger than minimum precision
            if(fabs(x[i] - newX[i]) >= epsilon)    
            {
                //Set flag back to 0 and continue on
                flag = 0;    
                break;
            }
        }

        // Update x vector in parallel
        #pragma omp parallel 
        {
            #pragma omp for
                for(i = 0; i < N; i++)
                {
                    x[i] = newX[i];
                }
        }
        iters++;
        
    }
    after = clock();

    //  Displays calculated results if desired
    /*    
    for(i = 0; i < N; i ++)
    {
        printf("x%d: %f after %d iterations\n",i,newX[i],iters);
    }
    */

    // Displays elapsed runtime    
    timeDifference = (after - before) / CLOCKS_PER_SEC;
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
    jacobi(192);
}