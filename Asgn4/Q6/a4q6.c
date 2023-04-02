#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <math.h>

#define ROOT 0
#define alpha 2
#define d 10

void splitAndCorrect(int N)
{
    int p,m,i,j;
    double b[N];
    double r1,r2,xi,yi;

    MPI_Init(NULL,NULL);
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Comm_rank(comm, &p);
    MPI_Comm_size(comm, &m);

    double localB[N/m];
    double xVector[N/m], yVector[N/m];

    //  (0).    Initialization
    if(p == 0)
    {
        //  Set result vector b to all 1's
        for(i = 0; i < N; i ++)
        {
            b[i] = 1;
            printf("b%d:\t[%f]\n",i,b[i]);
        }
    }
    MPI_Barrier(comm);

    //  (1).    Calculating r1, r2
    if(d >= 0)
    {
        //  d is positive for quadratic formula
        r2 = (d + sqrt((d * d) - 4 * alpha)) / (2 * alpha);
        r1 = alpha / r2;

        printf("D is positive - r2: %f - r1: %f\n",r2,r1);
    }
    else
    {
        //  d is negative for quadratic formula
        r2 = (d - sqrt((d * d) - 4 * alpha)) / (2 * alpha);
        r1 = d - r2;
        
        printf("D is negative - r2: %f - r1: %f\n",r2,r1);
    }
    MPI_Barrier(comm);

    //  Scatter RHS vector b across all processes
    MPI_Scatter(b, N/m, MPI_DOUBLE, localB, N/m, MPI_DOUBLE, ROOT, comm);
    printf("P%d:", p);
    for(i=0; i < (N/m); i++)
        printf("[%.1f]", localB[i]);
    printf("\n");
    MPI_Barrier(comm);


    //  (3).    Calc Yi for each process's mini-system
    if(p != 5)
    {
        for(i = 0; i < (N/m); i++)
        {
            if(i == 0)
            {
                //  Set Y1 = B1
                yi = localB[i];
                printf("P%d At %d, yi is:%f\n", p, i, yi);

                //  TODO -- Save value at this iteration in a vector??
                yVector[i] = yi;
            }
            else
            {
                //  Set Yi = Bi - (r1 * Yi-1)
                yi = localB[i] - (r1 * yi);
                printf("P%d At %d, yi is:%f\n", p, i, yi);

                //  TODO -- Save value at this iteration in a vector??
                yVector[i] = yi;
            }
        }

        //  (4).    Calc Xi for each process's mini-system by Work back through values backwards
        for(i = (N/m) - 1; i >= 0; i--)
        {
            if(i == (N/m) - 1)
            {
                //  Set Xn = Yn
                xi = yVector[i];
                printf("P%d At %d, xi is:%f\n", p, i, xi);

                //  TODO -- Save value at this iteration in a vector??
                xVector[i] = xi;
            }
            else
            {
                //  Set xi = (Yi - Xi+1) / r2
                xi = (yVector[i] - xi) / r2;          
                printf("P%d At %d, xi is:%f\n", p, i, xi);

                //  TODO -- Save value at this iteration in a vector??
                xVector[i] = xi;
            }
        }
    }







    MPI_Finalize();
}

void main()
{
    splitAndCorrect(48);
}