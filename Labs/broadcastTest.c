#include <stdio.h>
#include <mpi.h>
#include <time.h>

main()
{
    int node;
    int root = 0;
    int arr[10];
    int p;


    MPI_Init(NULL,NULL);
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Comm_rank(comm, &p);

    if(p == root)
    {
        srand(time(NULL));
        for(int i = 0; i < 10; i++)
        {
            arr[i] = rand()%25;
        }
        MPI_Bcast(&arr,1,MPI_INT,root,MPI_COMM_WORLD);

        printf("Hello World BIG BOSS NODE %d, i have values:", p);
        for(int i = 0; i < 10; i++)
        {
            printf("%i,", arr[i]);
        }
        printf("\n");
    }
    else
    {
        printf("Hello World from Node %d, i have values:", p);
        for(int i = 0; i < 10; i++)
        {
            printf("%i,", arr[i]);
        }
        printf("\n");
    }

    MPI_Finalize();
}