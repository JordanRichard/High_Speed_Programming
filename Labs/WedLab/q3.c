#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include <math.h>

#define ROOT 0
#define n 10

int main()
{
    int p,m;

    MPI_Init(NULL,NULL);
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Comm_rank(comm, &p);
    MPI_Comm_size(comm, &m);
    
    int localArray[10];
    for(int i=0; i < 10; i++)
    {
        localArray[i] = p;
    }


    


    

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