#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

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
    int p, m, sliceSize, i, j;
    double **matrix1, **matrix2, **matrix3, *localRow1, *localRow2, *localRow3;
    double startTime, endTime, globalStartTime, globalEndTime;

    MPI_Init(NULL,NULL);
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Comm_rank(comm, &p);
    MPI_Comm_size(comm, &m);

    // Start timing each process
    MPI_Barrier(comm);
    startTime = MPI_Wtime();

    // Allocate memory for pointers to array columns of size N
    matrix1 = (double **) malloc(N * sizeof(double *));
    matrix2 = (double **) malloc(N * sizeof(double *));
    matrix3 = (double **) malloc(N * sizeof(double *));

    // Allocate memory for NxN 2d aarray
    matrix1[0] = (double *) malloc(N * N * sizeof(double));
    matrix2[0] = (double *) malloc(N * N * sizeof(double));
    matrix3[0] = (double *) malloc(N * N * sizeof(double));

    // Assigns offset for each row's starting addresses using pointer arithmetic
    for (i = 1; i < N; i++) 
    {
        matrix1[i] = matrix1[0] + i * N;
        matrix2[i] = matrix2[0] + i * N;
        matrix3[i] = matrix3[0] + i * N;
    }

    // Allocate slices of matrix locally and distribute among processes
    sliceSize = N / m;
    localRow1 = (double *) malloc(sliceSize * N * sizeof(double));
    localRow2 = (double *) malloc(sliceSize * N * sizeof(double));
    localRow3 = (double *) malloc(sliceSize * N * sizeof(double));
    MPI_Scatter(matrix1[0], sliceSize * N, MPI_DOUBLE,localRow1, sliceSize * N, MPI_DOUBLE, ROOT, comm);
    MPI_Scatter(matrix2[0], sliceSize * N, MPI_DOUBLE,localRow2, sliceSize * N, MPI_DOUBLE, ROOT, comm);

    // Seed random number generator and offset by pid to ensure unique output
    srand(MPI_Wtime() + p);

    // Have each process initialize its local array slice with random double values
    for (i = 0; i < sliceSize; i++) 
    {
        for (j = 0; j < N; j++) 
        {
            localRow1[i * N + j] = (double)(rand() % 10);
            localRow2[i * N + j] = (double)(rand() % 10);
        }
    }

    // Gather all initialized slices into root (input) matrices
    MPI_Gather(localRow1, sliceSize * N, MPI_DOUBLE,matrix1[0], sliceSize * N, MPI_DOUBLE, ROOT, comm);
    MPI_Gather(localRow2, sliceSize * N, MPI_DOUBLE,matrix2[0], sliceSize * N, MPI_DOUBLE, ROOT, comm);
    
    // Store sum of local slices in Result slice and gather into result matrix
    for (i = 0; i < sliceSize * N; i++) 
    {
        localRow3[i] = localRow1[i] + localRow2[i];
    }
    MPI_Gather(localRow3, sliceSize * N, MPI_DOUBLE,matrix3[0], sliceSize * N, MPI_DOUBLE, ROOT, comm);

    endTime = MPI_Wtime();

    //  Calculate elapsed runtime over all processes
    MPI_Reduce(&startTime, &globalStartTime, 1, MPI_DOUBLE, MPI_MIN, ROOT, comm);
    MPI_Reduce(&endTime, &globalEndTime, 1, MPI_DOUBLE, MPI_MIN, ROOT, comm);

    //  Gets root node to display results
    if (p == ROOT) 
    {
        //  Display parallel execution time in seconds
        printf("%f\n", (globalEndTime - globalStartTime));

        //  Display All Matrices if desired
        /*
        printf("Randomly generated %dx%d array1:\n", N, N);
        for (i = 0; i < N; i++) 
        {
            for (j = 0; j < N; j++) 
            {
                printf("[%.0f] ", matrix1[i][j]);
            }
            printf("\n");
        }
        printf("\n");

        printf("Randomly generated %dx%d array2:\n", N, N);
        for (i = 0; i < N; i++) 
        {
            for (j = 0; j < N; j++) 
            {
                printf("[%.0f] ", matrix2[i][j]);
            }
            printf("\n");
        }
        printf("\n");

        printf("Result %dx%d array3:\n", N, N);
        for (i = 0; i < N; i++) 
        {
            for (j = 0; j < N; j++) 
            {
                printf("[%.0f] ", matrix3[i][j]);
            }
            printf("\n");
        }
        */
    }
    

    // De-allocate memory reserved for matrices
    free(localRow1);
    free(matrix1[0]);
    free(matrix1);

    free(localRow2);
    free(matrix2[0]);
    free(matrix2);

    free(localRow3);
    free(matrix3[0]);
    free(matrix3);

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
    matrixAddition(10000);
}