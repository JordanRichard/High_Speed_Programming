#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <time.h>
#include <math.h>

#define ROOT 0
/******************************************************************************
 * 	
 * Driver program to carry out gauss-jacobi iterations to solve a system AX = b
 * in parallel using MPI.
 *
 *  
 * @author	Jordan Alexander Richard
 * @version CS 3123 - Assignment 5 Question 3b
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
    int i,j,p,m,sliceSize,startRow,endRow,flag = 0,iters = 0;
    double epsilon = 0.000001;

    double a[N][N];
    double x[N];
    double b[N];
    double newX[N];

    MPI_Init(NULL,NULL);
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Comm_rank(comm, &p);
    MPI_Comm_size(comm, &m);

    sliceSize = N/m;
    double localSlice[sliceSize][N];
    double localB[sliceSize];
    double localX[sliceSize];
    double localNewX[sliceSize];

    srand(MPI_Wtime());

    if(p == ROOT)
    {
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
    }


    printf("HI\n");
    //  Scatter matrix a across processes
    MPI_Scatter(a,sliceSize * N, MPI_DOUBLE,localSlice, sliceSize * N, MPI_DOUBLE, ROOT, comm);
    MPI_Scatter(b,sliceSize, MPI_DOUBLE,localB, sliceSize, MPI_DOUBLE, ROOT, comm);
    MPI_Scatter(x,sliceSize, MPI_DOUBLE,localX, sliceSize, MPI_DOUBLE, ROOT, comm);




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

    before = MPI_Wtime();

    // Determine the number of rows each process will compute
    startRow = p * sliceSize;
    if (p == m - 1) 
        endRow = N;
    else 
        endRow = startRow + sliceSize;

    printf("P:%d start %d end %d ---\n",p,startRow,endRow);
    //  Continue carrying out jacobi iterations until exit condition is met
    //while(flag != 1)
    //{
        printf("--- Iteration %d ---\n",iters+1);

        // Carry out one iteration of the gauss-jacobi method
        for(i = startRow; i < endRow; i++)
        {
            xParts = localB[i];
            for(j = 0; j < N; j++)
            {
                //  If we're NOT on the diagonal
                if(j != i)
                {  
                    xParts -= (localSlice[i][j] * localX[j]);
                }
            }
            localNewX[i] = xParts / localSlice[i][i];
            printf("p%d: x%i: %f\n",p,i,localNewX[i]);

        }            

        /*
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
        */
        iters++;
    //}



    after = MPI_Wtime();
    timeDifference = after - before;

    // Displays elapsed runtime
    printf("%f\n",timeDifference);
    
    MPI_Finalize();
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
    jacobi(4);
}