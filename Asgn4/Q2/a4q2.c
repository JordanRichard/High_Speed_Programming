#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <time.h>
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
    //double timeDifference;
    double xi = 0;
    double localSum = 0;
    double totalSum = 0;
    int p,m;

    int input[N];

    //  Setup MPI
    MPI_Init(NULL,NULL);
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Comm_rank(comm, &p);
    MPI_Comm_size(comm, &m);

    //  Get root node to generate input values
    if(p == ROOT)
    {
        srand(time(NULL)); 

        for(int i = 0; i < N; i++)
        {
            input[i] = (rand() % (10-1 +1)) + 1;
            printf("[%d]",input[i]);
        }
        printf("\n");
    }

    // Send input array to worker nodes
    MPI_Bcast(&input,N,MPI_INT,ROOT,MPI_COMM_WORLD);


    //  Get worker nodes to make their own summations
    int start = (p * (N / m));
	int end = ((p + 1) * (N / m) -1);

    //  Calculate summation
    printf("P%d:",p);
    for(int j = start; j <= end; j++)
    {
        xi = input[j];
        localSum += (xi / (xi + 1)) * (pow(-1, j));
        printf("[%d]",input[j]);
    }
    printf("Localsum on P%d:%f\n",p,localSum);

    //  Combine results and send to root
    MPI_Reduce(&localSum,&totalSum,1,MPI_DOUBLE,MPI_SUM,ROOT,comm);
    
    //  Show result from root
    if(p == ROOT)
    {
        printf("Total: %f\n",totalSum);
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
    summation(20);
}