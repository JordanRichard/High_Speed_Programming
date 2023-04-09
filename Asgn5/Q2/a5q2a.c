#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#define ROOT 0


/******************************************************************************
 * 	
 * Driver program to carry out parallel matrix multiplication using MPI for 
 * question 2.
 *
 *  
 * @author	Jordan Alexander Richard
 * @version CS 3123 - Assignment 5 Question 2a
******************************************************************************/

/******************************************************************************
 *  Method: matrixMult: Given a value N, randomly generate 2 NxN matrices and
 *          multiply them together in parallel using MPI. Carrys out matrix 
 *          multiplication by scattering rows of one of the matrices and
 *          having each process multiply the rows by the second matrix
 *          in order to compute each row of the result matrix
 *
 *  Input:  N - The number of values to be processed.
 * 
 *  Output: Nil
 * 
 * ****************************************************************************/
void matrixMult(int N) 
{
    
    int p,m,i,j,k,sliceSize,startRow,endRow,sum;
    
    MPI_Init(NULL,NULL);
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Comm_rank(comm, &p);
    MPI_Comm_size(comm, &m);

    int matrix1[N][N], matrix2[N][N], result[N][N];

    // Initialize random seed
    srand(time(NULL));

    // Randomly generate matrixes matrix1,matrix2
    for (i = 0; i < N; i++) 
    {
        for (j = 0; j < N; j++) 
        {
            matrix1[i][j] = rand() % 10 + 1;
            matrix2[i][j] = rand() % 10 + 1;
        }
    }


    // Determine the number of rows each process will compute
    sliceSize = N / m;
    startRow = p * sliceSize;
    if (p == m - 1) 
        endRow = N;
    else 
        endRow = startRow + sliceSize;

    // Scatter rows of matrix1 and the entire matrix2 matrix to each process
    int localSlice[sliceSize * N];
    MPI_Scatter(matrix1, sliceSize * N, MPI_INT, localSlice, sliceSize * N, MPI_INT, ROOT, comm);
    MPI_Bcast(matrix2, N * N, MPI_INT, ROOT, comm);

    // Compute the product of the scattered rows and the entire matrix2 matrix
    for (i = startRow; i < endRow; i++) 
    {
        for (j = 0; j < N; j++) 
        {
            sum = 0;
            for (k = 0; k < N; k++) 
            {
                sum += localSlice[(i - startRow) * N + k] * matrix2[k][j];
            }
            result[i][j] = sum;
        }
    }

    // Gather the results from each process
    MPI_Gather(result[startRow], sliceSize * N, MPI_INT, result, sliceSize * N, MPI_INT, ROOT, comm);

    // Print the result
    if (p == ROOT) 
    {
        printf("Matrix matrix1:\n");
        for (i = 0; i < N; i++) 
        {
            for (j = 0; j < N; j++) 
            {
                printf("%d ", matrix1[i][j]);
            }
            printf("\n");
        }

        printf("\nMatrix matrix2:\n");
        for (i = 0; i < N; i++) 
        {
            for (j = 0; j < N; j++) 
            {
                printf("%d ", matrix2[i][j]);
            }
            printf("\n");
        }

        printf("\nResult:\n");
        for (i = 0; i < N; i++) 
        {
            for (j = 0; j < N; j++) 
            {
                printf("%d ", result[i][j]);
            }
            printf("\n");
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
void main()
{
    matrixMult(32);
}