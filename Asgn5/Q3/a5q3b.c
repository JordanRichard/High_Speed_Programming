#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
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
 *              solves in parallel with MPI using the gauss-jacobi iterative
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
    double startTime,endTime,globalStartTime,globalEndTime;
    double rowSum = 0,xParts = 0;
    int i,j,p,m,sliceSize,offset,flag = 0,iters = 0;
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
        
        //  Randomly initialize the vector b
        for (i = 0; i < N; i++) 
        {
            b[i] = (double)(rand() % 21 - 10);
        }
    }

    //  Create "Guess" vector x,xnew on each process and fill with 0's to start
    for (i = 0; i < N; i++) 
    {
        newX[i] = 0;
        x[i] = 0;
    }

    //  Scatter matrix a and vector b across processes
    MPI_Scatter(a,sliceSize * N, MPI_DOUBLE,localSlice, sliceSize * N, MPI_DOUBLE, ROOT, comm);
    MPI_Scatter(b,sliceSize, MPI_DOUBLE,localB, sliceSize, MPI_DOUBLE, ROOT, comm);

    // Start timing each process
    startTime = MPI_Wtime();

    //  Calc column offset to apply to row diagonals
    offset = p * (N/m);

    //  Continue carrying out jacobi iterations until exit condition is met
    while(flag != 1)
    {
        // Carry out one iteration of the gauss-jacobi method on this process
        for(i = 0; i < N/m; i++)
        {
            xParts = localB[i];
            for(j = 0; j < N; j++)
            {
                //  If we're NOT on the diagonal
                if(j != i + offset)
                {  
                    xParts -= (localSlice[i][j] * x[j]);
                }
            }
            localNewX[i] = xParts / localSlice[i][i + offset];
        }   

        //  Gather newly calculated x values to all processors
        MPI_Allgather(localNewX, N/m, MPI_DOUBLE, newX, N/m, MPI_DOUBLE, comm);
        
        //  Check difference in calculated values and stop iterating if small enough
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

        //  Update x vector
        for(i = 0; i < N; i ++)
        {
            x[i] = newX[i];
        }        
        
        iters++;
    }
    endTime = MPI_Wtime();

    //  Calculate elapsed runtime over all processes
    MPI_Reduce(&startTime, &globalStartTime, 1, MPI_DOUBLE, MPI_MIN, ROOT, comm);
    MPI_Reduce(&endTime, &globalEndTime, 1, MPI_DOUBLE, MPI_MIN, ROOT, comm);

    // Have root display results
    if(p == 0)
    {
        // Display calculated vector x if desired
        /*
        printf("After %d iterations:\n",iters+1);
        for(i = 0; i < N; i++)
        {
            printf("x%i: %f\n",i,x[i]);
        }
        */
        
        // Displays elapsed runtime
        printf("%f\n", endTime - startTime);
    }

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
    jacobi(256);
}