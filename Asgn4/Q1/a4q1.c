#include <stdio.h>
#include <mpi.h>
#include <time.h>

#define ROOT 0
/******************************************************************************
 * 	
 * Driver program to create a random integer array and frequency table for 
 * question 1.
 *
 *  
 * @author	Jordan Alexander Richard
 * @version CS 3123 - Assignment 4 Question 1
******************************************************************************/

/******************************************************************************
 *  Method: randomIntFrequency: Given a value N, generates an array of N random 
 *          integers in parallel and creates a frequency table, implementation
 *          modified to use an MPI distributed memory approach.
 * 
 *  Input:  N - The number of values to be processed.
 * 
 *  Output: Nil
 * 
 * ****************************************************************************/
void randomIntFrequency(int N)
{
    int randomIntegers[N];
    int p,m;

    int globalSum[10];
    int localSum[10];

    //  Initialize MPI
    MPI_Init(NULL,NULL);
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Comm_rank(comm, &p);
    MPI_Comm_size(comm, &m);

    // Get root node to initialize input array
    /*  -- TODO: Parallelize    */
    if(p == ROOT)
    {
        srand(time(NULL));

        //Initializes blank frequency table
        for(int i = 0; i < 10; i++)
            globalSum[i] = 0;

        //Generates randomized value between 1-10 and adds to array
        for(int i = 0; i < N; i++)
        {
            randomIntegers[i] = (rand() % (10-1 +1)) + 1;
            //printf("[%d]",randomIntegers[i]);
        }
        //printf("\n");
    }
    // Share input array with worker nodes
    MPI_Bcast(&randomIntegers,N,MPI_INT,ROOT,MPI_COMM_WORLD);


	// Calculates start and endpoints of each process's subarray
	int start = (p * (N / m));
	int end = ((p + 1) * (N / m) -1);

    /*  Show subarrays  */
    /*
    printf("P%d: ",p);
    for(int i = start; i <= end; i++)
    {
        printf("[%d]",randomIntegers[i]);
    }
    printf("\n");
    MPI_Barrier(comm);
    */

    // Loop through each value in freq table and tally value totals by process
    for(int i = 0; i < 10; i++)
    {
        int tally = 0;

        // Foreach value in process subarray, increment value counter if match
        for(int j = start; j <= end; j++)
        {
            // 
            if(randomIntegers[j] == i+1)
                tally++;
        }

        //Reduce all found value tallys to frequency table
        localSum[i] = tally;
        MPI_Barrier(comm);
        MPI_Reduce(&localSum[i],&globalSum[i],1,MPI_INT,MPI_SUM,ROOT,comm);
    }
    MPI_Barrier(comm);

    // Gets root node to display frequency table
    if(p == ROOT)
    {
        printf("Value:\tFrequency:\tRelative:\n");
        for(int i = 0; i < 10; i++)
        {
            float relativeFrequency = (float)globalSum[i] / (float)N;
            printf("[%i] \t%i \t\t%.4f\n", i + 1,globalSum[i],relativeFrequency);
        }
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
    randomIntFrequency(20);
}