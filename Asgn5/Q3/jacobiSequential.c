#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <math.h>

/******************************************************************************
 * 	
 * Driver program to carry out parallel matrix addition for question 3. 
 * Modified to use OMP commands.
 *
 *  
 * @author	Jordan Alexander Richard
 * @version CS 3123 - Assignment 5 Question 3a
******************************************************************************/

/******************************************************************************
 *  Method: 
 * 
 *  Input:  N - The size (order) of matrices to be created.
 * 
 *  Output: Nil
 * 
 * ****************************************************************************/
void jacobi(int N)
{

    double before,after,timeDifference;
    double rowSum = 0,xParts = 0;
    int i,j,flag = 0,iters = 0;

    double a[N][N];
    double x[N];
    double b[N];
    double newX[N];


    srand(time(NULL));

    //  Randomly generate a NxN matrix and ensure it to be diagonally dominant
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

    /*
    //Print matrix
    for (i = 0; i < N; i++) 
    {
        for (j = 0; j < N; j++) 
        {
            printf("[%.0f]",a[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    //  Print Vectors
    printf("b: ");
    for (i = 0; i < N; i++) 
        printf("[%.0f]",b[i]);
    printf("\n\n");

    printf("x: ");
    for (i = 0; i < N; i++) 
        printf("[%.0f]",x[i]);
    printf("\n\n");

    */

    before = clock();

    //  Continue carrying out jacobi iterations until exit condition is met
    while(flag != 1)
    {
        printf("--- Iteration %d ---\n",iters+1);

        // Carry out one iteration of the gauss-jacobi method
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
            printf("x%i: %f\n",i,newX[i]);

        }            

        //  Check difference in calculated values and stop iterating if small enough
        flag = 1;
        for(i = 0; i < N; i++)
        {
            //Stop testing if any larger than minimum precision
            if(fabs(x[i] - newX[i]) >= 0.000001)    
            {
                //Set flag back to 0 and continue on
                flag = 0;    
                break;
            }
        }

        // Update x vector
        for(i = 0; i < N; i++)
        {
            x[i] = newX[i];
        }

        iters++;
    }



    after = clock();
    timeDifference = (after - before) / CLOCKS_PER_SEC;

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
    jacobi(15);
}