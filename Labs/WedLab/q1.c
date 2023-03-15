#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include <math.h>

#define ROOT 0
#define n 5

int main()
{
    int p,m;

    MPI_Init(NULL,NULL);
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Comm_rank(comm, &p);
    MPI_Comm_size(comm, &m);
    
    int array[n*m];
    int localArray[n];

    // Get Root to create a random array of values in [0,1)
    if(p == ROOT)
    {
        srand(time(NULL));
        for(int i = 0; i < n*m; i++)
        {
            array[i] = rand() % 2;
            //printf("[%d]",array[i]);
        }
        //printf("\n");
    }

    //Scatter in groups of N
    MPI_Scatter(array,n,MPI_INT,localArray,n,MPI_INT,ROOT,comm);

    //printf("P%d:",p);
    for(int i = 0; i < n; i++)
    {
        localArray[i] = localArray[i] * pow(10,m);
        //printf("[%d]",localArray[i]);
    }
    //printf("\n");

    // Gather local values back to root
    MPI_Gather(localArray,n,MPI_INT,array,n,MPI_INT,ROOT,comm);

    //Get Root to show gathered values
    if(p == ROOT)
    {
        for(int i = 0; i < n*m; i++)
        {
            printf("[%d]",array[i]);
        }
        printf("\n");
    }
    
}