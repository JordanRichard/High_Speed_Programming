#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <time.h>

#define ROOT 0

/******************************************************************************
 * 	
 * Driver program to carry out parallel matrix addition for question 3. 
 * Modified to use an MPI distributed memory approach.
 *
 *  
 * @author	Jordan Alexander Richard
 * @version CS 3123 - Assignment 4 Question 3
******************************************************************************/

/******************************************************************************
 *  Method: matrixAddition: Given a value N, generates two NxN matrices of 
 *          randomly generated double values and adds them together in 
 *          parallel.
 * 
 *  Input:  N - The size (order) of matrices to be created.
 * 
 *  Output: Nil
 * 
 * ****************************************************************************/
void matrixAddition(int N)
{
    //double timeDifference;

    double arr1[N][N];
    double arr2[N][N];
    double arr3[N][N];
    int p,m;

    double tempRow[N];

    //  Setup MPI
    MPI_Init(NULL,NULL);
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Comm_rank(comm, &p);
    MPI_Comm_size(comm, &m);

    // Calculates start and endpoints of each process
    int start = (p * (N / m));
    int end = ((p + 1) * (N / m) -1);


    for(int i = start; i < end; i++)
    {
        printf("HI\n");
        for(int j = 0; j < N; j++)
        {
            //arr1[i][j] = (double)(rand() % 10);
            //arr2[i][j] = (double)(rand() % 10);

            //Generate row of data
            tempRow[j] = (double)(rand() % 10);
            
        }
        //Send the row of data we made to the root matrix somehow
        MPI_Gather(&tempRow,N,MPI_DOUBLE,&arr1[i],N,MPI_DOUBLE,ROOT,comm);
        printf("[%f]",tempRow[i]);
    }
    printf("\n");

    // Get root to print result matrix
   if(p == ROOT)
   {
        for(int i = 0; i < N; i++)
        {

            printf("P%d:",p);
            for(int j = 0; j < N; j++)
            {
                printf("[%f]",arr1[i][j]);
            }
            printf("\n");  
        }

   }

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
    matrixAddition(4);
}