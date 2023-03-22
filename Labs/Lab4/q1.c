#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include <math.h>

#define ROOT 0
#define N 100000

int main()
{
    int p,m;
    int arr[N];
    int localArr[N];
    double start,end,globalStart,globalEnd,diff;

    for(int i = 0; i < N; i++)
    {
        arr[i] = i;
    }

    MPI_Init(NULL,NULL);
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Comm_rank(comm, &p);
    MPI_Comm_size(comm, &m);

    if(p == ROOT)
    {
        //Begin timing the send
        start = MPI_Wtime();
        MPI_Send(&arr,N,MPI_INT,1,99,comm);
        end = MPI_Wtime();
    }
    else
    {
        start = MPI_Wtime();
        MPI_Recv(&localArr,N,MPI_INT,ROOT,99,comm,MPI_STATUS_IGNORE);
        end = MPI_Wtime();
        //Display recieved array
        /*
        printf("P%d:",p);
        for(int i = 0; i < N; i++)
        {
            printf("[%d]",localArr[i]);
        }
        printf("\n");
        */
    }

    MPI_Reduce(&start,&globalStart,1,MPI_DOUBLE,MPI_MIN,ROOT,comm);
    MPI_Reduce(&end,&globalEnd,1,MPI_DOUBLE,MPI_MAX,ROOT,comm);

    if(p == ROOT)
    { 
        diff = (globalEnd - globalStart) * 1000;
        printf("%f\n",diff);
    }
}