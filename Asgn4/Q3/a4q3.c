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
    //double arr2[N][N];
    //double arr3[N][N];
    
    double tempRow[N];
    double otherTemp[N];
    int p,m;

    

    //  Setup MPI
    MPI_Init(NULL,NULL);
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Comm_rank(comm, &p);
    MPI_Comm_size(comm, &m);

    MPI_Status status;

    // Calculates start and endpoints of each process
    int start = (p * (N / m));
    int end = ((p + 1) * (N / m) -1);
    MPI_Barrier(comm);

    srand(time(NULL) + p);

    // for range of rows per process
    for(int i = start; i <= end; i++)
    {   
        printf("P%d ",p);
        //make input vectors  
        for(int j = 0; j < N; j++)
        {
            tempRow[j] = (double)(rand() % 10);
            printf("%.1f ",tempRow[j]);
        }
        MPI_Barrier(comm);

        
        MPI_Gather(&tempRow,N,MPI_DOUBLE,&(arr1[i]),N,MPI_DOUBLE,ROOT,comm);
        
        /*
        MPI_Send(&tempRow, N, MPI_DOUBLE, ROOT, 1, comm);

        if(p == ROOT)
        {
            MPI_Recv(arr1, N, MPI_DOUBLE, ROOT, MPI_ANY_TAG,comm,&status);
           
        }
        */
        
        printf("\n");
        //printf("p%d updated row %d\n",p,i);
        

        //Send the row of data we made to the root matrix somehow
        
        //printf("p%d iter%d sending to row %d\n",p,i);
        MPI_Barrier(comm);
        
        
    }
    printf("\n");
    MPI_Barrier(comm);

    
    // Get root to print result matrix
   if(p == ROOT)
   {
        //for(int x=0;x<N;x++)
        //{
        //    arr1[7][x] = 999;
        //}
        printf("Back on 0, printing arr...\n");
        for(int i = 0; i < N; i++)
        {

            printf("Row%d:",i);
            for(int j = 0; j < N; j++)
            {
                printf("[%.0f]",arr1[i][j]);
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
    matrixAddition(8);
}