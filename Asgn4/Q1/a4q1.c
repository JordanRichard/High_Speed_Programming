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
    int frequencyTable[10];
    int localSum[10];
    int p,m;

    // Setup MPI
    MPI_Init(NULL,NULL);
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Comm_rank(comm, &p);
    MPI_Comm_size(comm, &m);

    int localRandoms[N/m];

    // Get root to initialize blank frequency table
    if(p == ROOT)      
        for(int i = 0; i < 10; i++)
            frequencyTable[i] = 0;
 
    //Seed RNG based on time and node # to ensure unique results
    srand(time(NULL) + p);
    
    //printf("P%d:",p);
    for(int i = 0; i < N/m; i++)
    {
        localRandoms[i] = (rand() % (10-1 +1)) + 1;
        //printf("[%d]",localRandoms[i]);
    }
    //printf("\n");
    
    // Collect generated subarrays into root array
    MPI_Gather(localRandoms,N/m,MPI_INT,randomIntegers,N/m,MPI_INT,ROOT,comm);
    if(p == 0)
    {
        printf("Gathering into root...\n");
        //Show gathered arr at root node
        for(int i = 0; i < N; i++)
            printf("[%d]",randomIntegers[i]);
        printf("\n");
    }

    // Share input array with worker nodes
    MPI_Bcast(&randomIntegers,N,MPI_INT,ROOT,MPI_COMM_WORLD);
    
    // Calculates start and endpoints of each process's subarray
	int start = (p * (N / m));
	int end = ((p + 1) * (N / m) -1);
    MPI_Barrier(comm);

    /*
    //Show subarrays
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
            if(randomIntegers[j] == i+1)
                tally++;

        //Reduce all found value tallys to frequency table
        localSum[i] = tally;
        MPI_Reduce(&localSum[i],&frequencyTable[i],1,MPI_INT,MPI_SUM,ROOT,comm);
    }
    
    // Gets root node to display frequency table
    if(p == ROOT)
    {
        printf("Value:\tFrequency:\tRelative:\n");
        for(int i = 0; i < 10; i++)
        {
            float relativeFrequency = (float)frequencyTable[i] / (float)N;
            printf("[%i] \t%i \t\t%.4f\n", i + 1,frequencyTable[i],relativeFrequency);
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
    randomIntFrequency(8);
}