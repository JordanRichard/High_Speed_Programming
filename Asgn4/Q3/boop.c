#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define ROOT 0

void gen(int N)
{
    int p, m;
    double **matrix1, **matrix2, **matrix3, *localRow1, *localRow2, *localRow3;
    int sliceSize, i, j;

    MPI_Init(NULL,NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &p);
    MPI_Comm_size(MPI_COMM_WORLD, &m);

    /*  PERFORM MEMORY ALLOCATION       */

    // allocate memory for contiguous 2D array
    matrix1 = (double **) malloc(N * sizeof(double *));
    matrix2 = (double **) malloc(N * sizeof(double *));
    matrix3 = (double **) malloc(N * sizeof(double *));

    //Allocate memory block for whole nxn array
    matrix1[0] = (double *) malloc(N * N * sizeof(double));
    matrix2[0] = (double *) malloc(N * N * sizeof(double));
    matrix3[0] = (double *) malloc(N * N * sizeof(double));
    for (i = 1; i < N; i++) 
    {
        //Assigns offset for next row's starting address using pointer arithmetic
        matrix1[i] = matrix1[0] + i * N;
        matrix2[i] = matrix2[0] + i * N;
        matrix3[i] = matrix3[0] + i * N;
    }

    // Create local slices of matrix and distribute among processes
    sliceSize = N / m;
    localRow1 = (double *) malloc(sliceSize * N * sizeof(double));
    localRow2 = (double *) malloc(sliceSize * N * sizeof(double));
    localRow3 = (double *) malloc(sliceSize * N * sizeof(double));
    MPI_Scatter(matrix1[0], sliceSize * N, MPI_DOUBLE,localRow1, sliceSize * N, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);
    MPI_Scatter(matrix2[0], sliceSize * N, MPI_DOUBLE,localRow2, sliceSize * N, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);

    /*  PERFORM ARRAY INITIALIZATION    */

    // Initialize local array slice(s) with random double values
    srand(time(NULL) + p);
    // For all slices for this process
    for (i = 0; i < sliceSize; i++) 
    {
        // For each index in slice
        for (j = 0; j < N; j++) 
        {
            localRow1[i * N + j] = (double)(rand() % 10);
            localRow2[i * N + j] = (double)(rand() % 10);
        }
    }
    // Gather all initialized slices into root (input) matrices
    MPI_Gather(localRow1, sliceSize * N, MPI_DOUBLE,matrix1[0], sliceSize * N, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);
    MPI_Gather(localRow2, sliceSize * N, MPI_DOUBLE,matrix2[0], sliceSize * N, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);
    
    /*  PERFORM MATRIX ADDITION         */
    
    // Store sum of local slices in Result slice
    for (i = 0; i < sliceSize * N; i++) 
    {
        localRow3[i] = localRow1[i] + localRow2[i];
    }

    // Gather all initialized slices into root matrix
    MPI_Gather(localRow3, sliceSize * N, MPI_DOUBLE,matrix3[0], sliceSize * N, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);

    // Gets root node to display the matrix created
    if (p == 0) 
    {
        printf("Randomly generated %dx%d array1:\n", N, N);
        for (i = 0; i < N; i++) 
        {
            for (j = 0; j < N; j++) 
            {
                printf("[%.1f] ", matrix1[i][j]);
            }
            printf("\n");
        }

        printf("\n\n");

        printf("Randomly generated %dx%d array2:\n", N, N);
        for (i = 0; i < N; i++) 
        {
            for (j = 0; j < N; j++) 
            {
                printf("[%.1f] ", matrix2[i][j]);
            }
            printf("\n");
        }

        printf("Result %dx%d array3:\n", N, N);
        for (i = 0; i < N; i++) 
        {
            for (j = 0; j < N; j++) 
            {
                printf("[%.1f] ", matrix3[i][j]);
            }
            printf("\n");
        }
    }

    // De-allocate heap memory
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

int main() {
    gen(100);
}