#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <math.h>

#define ROOT 0

/******************************************************************************
 * 	
 * Driver program to calculate a summation for question 2.
 *
 *  
 * @author	Jordan Alexander Richard
 * @version CS 3123 - Assignment 4 Question 2
******************************************************************************/

/******************************************************************************
 *  Method: summation: Given a value N, calculates a summation in parallel. 
 *          Modified to use an MPI distributed memory approach.
 * 
 *  Input:  N - Upper limit of the summation to be calculated.
 * 
 *  Output: Nil
 * 
 * ****************************************************************************/
 void summation(int N)
 {
    int input[N];
    double xi = 0;
    double localSum = 0;
    double totalSum = 0;
    int p, m, i;
    double startTime, endTime, globalStartTime, globalEndTime;

    MPI_Init(NULL,NULL);
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Comm_rank(comm, &p);
    MPI_Comm_size(comm, &m);

    //  Get root node to generate input values
    if(p == ROOT)
    {
        srand(MPI_Wtime()); 

        for(i = 0; i < N; i++)
        {
            input[i] = (rand() % 10) + 1;
        }
    }

    //  Start timing each process
    MPI_Barrier(comm);
    startTime = MPI_Wtime();

    //  Broadcast input array to worker nodes
    MPI_Bcast(&input, N, MPI_INT, ROOT, MPI_COMM_WORLD);

    //  Have each node calculute their portion of array to work on
    int startIndex = (p * (N / m));
	int endIndex = ((p + 1) * (N / m) -1);

    //  Calculate summation of local subarray
    for(i = startIndex; i <= endIndex; i++)
    {
        xi = input[i];
        localSum += (xi / (xi + 1)) * (pow(-1, i));
    }
    
    //  Combine local results and send to root
    MPI_Reduce(&localSum, &totalSum, 1, MPI_DOUBLE, MPI_SUM, ROOT, comm);

    endTime = MPI_Wtime();

    //  Calculate elapsed runtime over all processes
    MPI_Reduce(&startTime, &globalStartTime, 1, MPI_DOUBLE, MPI_MIN, ROOT, comm);
    MPI_Reduce(&endTime, &globalEndTime, 1, MPI_DOUBLE, MPI_MIN, ROOT, comm);
    
    //  Show results from root
    if(p == ROOT)
    {
        //  Display parallel execution time in seconds
        printf("%f\n", (globalEndTime - globalStartTime));

        //  Display calculated sum if desired
        //  printf("Total: %f\n",totalSum);
    }

    MPI_Finalize();
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
int main()
{
    summation(500);
}