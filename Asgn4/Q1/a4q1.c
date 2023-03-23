#include <stdio.h>
#include <mpi.h>

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
    int p, m, i, j, counter;
    double startTime, endTime, globalStartTime, globalEndTime;

    MPI_Init(NULL,NULL);
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Comm_rank(comm, &p);
    MPI_Comm_size(comm, &m);

    int localRandoms[N/m];

    // Get root to initialize blank frequency table
    if(p == ROOT)
    {  
        for(i = 0; i < 10; i++)
        {
            frequencyTable[i] = 0;
        }
    }
    
    // Start timing each process
    MPI_Barrier(comm);
    startTime = MPI_Wtime();

    // Create local subarray of random ints, seed based on PID for uniqueness
    srand(MPI_Wtime() + p);
    for(i = 0; i < N/m; i++)
    {
        localRandoms[i] = (rand() % 10) + 1;
    }
    
    /*  
        Collect generated subarrays into root array and re-broadcast the whole 
        array to worker nodes. While this communication step is redundant 
        (As frequency counts could just be calculated on the subarrays already 
        in memory for each process), it would allow for later reuse of this 
        entire array if desireable. 
    */
    MPI_Gather(localRandoms, N/m, MPI_INT, randomIntegers, N/m, MPI_INT, ROOT, comm);
    MPI_Bcast(&randomIntegers, N, MPI_INT, ROOT, MPI_COMM_WORLD);
    
    // Calculates start and endpoints for each process to work on
	int startIndex = (p * (N / m));
	int endIndex = ((p + 1) * (N / m) -1);

    // Loop through each value in freq table and tally value totals by process
    for(i = 0; i < 10; i++)
    {
        counter = 0;

        // For each value in process subarray, increment value counter if match
        for(j = startIndex; j <= endIndex; j++)
        {
            if(randomIntegers[j] == i+1)
            {
                counter++;
            }
        }

        // Reduce sum of all found value counts to frequency table
        localSum[i] = counter;
        MPI_Reduce(&localSum[i], &frequencyTable[i], 1, MPI_INT, MPI_SUM, ROOT, comm);
    }

    endTime = MPI_Wtime();

    // Calculate elapsed runtime over all processes
    MPI_Reduce(&startTime, &globalStartTime, 1, MPI_DOUBLE, MPI_MIN, ROOT, comm);
    MPI_Reduce(&endTime, &globalEndTime, 1, MPI_DOUBLE, MPI_MIN, ROOT, comm);
    
    // Gets root node to display Results
    if(p == ROOT)
    {
        // Display parallel execution time in seconds
        printf("%f\n", (globalEndTime - globalStartTime));

        // Display Frequency table if desired
        /*
        printf("Value:\tFrequency:\tRelative:\n");
        for(i = 0; i < 10; i++)
        {
            float relativeFrequency = (float)frequencyTable[i] / (float)N;
            printf("[%i] \t%i \t\t%.4f\n", i + 1, frequencyTable[i], relativeFrequency);
        }
        */
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
    randomIntFrequency(10000);
}