#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>

#define ROOT 0

/******************************************************************************
 * 	
 * Driver program to estimate pi using a unit circle monte-carlo method for 
 * question 1.
 *
 *  
 * @author	Jordan Alexander Richard
 * @version CS 3123 - Assignment 5 Question 1
******************************************************************************/

/******************************************************************************
 *  Method: approximatePi: Given a value N, have each processor generate N 
 *          random x/y coordinates in parallel and determine the count of how
 *          many fall within the unit square. Combines totals and calculates an
 *          estimate of pi.
 *
 *  Input:  N - The number of values to be processed.
 * 
 *  Output: Nil
 * 
 * ****************************************************************************/
void approximatePi(int N)
{
    int p,m,i;
    double xCoord,yCoord,z,piApproximation;
    int insideCount = 0;
    int totalInsideCount = 0;

    MPI_Init(NULL,NULL);
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Comm_rank(comm, &p);
    MPI_Comm_size(comm, &m);

    srand(MPI_Wtime() + p);
    
    //  Generate N random coordinates between 0,1 and check if in the unit circle
    for(i = 0; i < N; i++)
    {
        xCoord = ((double)rand())/RAND_MAX;
        yCoord = ((double)rand())/RAND_MAX;
        z = sqrt((xCoord * xCoord) + (yCoord * yCoord));

        //  Keep a Count of all coordinates that fall within the unit circle
        if(z <= 1)
            insideCount++;
    }

    //  Combine inside totals
    MPI_Reduce(&insideCount, &totalInsideCount, 1, MPI_INT, MPI_SUM, ROOT, comm);
    
    //  Use totals to calculate estimate of pi, display results
    if(p == ROOT)
    {
        piApproximation = 4.0 * ((double)totalInsideCount / (double)(N * m));

        printf("TOTAL POINTS TESTED -- %d\nTOTAL POINTS INSIDE CIRCLE-- %d\n",
            (N * m),totalInsideCount);
        printf("PI: %f\n",piApproximation);
    }
}

/******************************************************************************
 *  Method: main: Calls the method required providing an argument N, the number
 *			of elements to process.
 * 
 *  Input:  Nil
 * 
 *  Output: Nil
 * 
 * ****************************************************************************/
void main()
{
    approximatePi(100000000);
}