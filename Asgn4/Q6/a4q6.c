#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <math.h>

#define ROOT 0

void splitAndCorrect(int N)
{
    int p,m,i;
    double b[N];
    double r1,r2,omega,rStar,t;

    double alpha = 2;
    double d = 10;
    double epsilon = pow(2,-8);

    MPI_Init(NULL,NULL);
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Comm_rank(comm, &p);
    MPI_Comm_size(comm, &m);

    double localB[N/m];
    double xVector[N/m], yVector[N/m];
    double finalXVector[N];
    double localFirst,localLast;

    //  (0).    Initialization
    if(p == 0)
    {
        //  Set result vector b to all 1's
        for(i = 0; i < N; i ++)
        {
            b[i]= alpha*(i-1) + d*(i) + (i+1);  //  first and last need fixing
            //b[i] = 1;
            printf("b%d:\t[%f]\n",i,b[i]);
        }
    }
    MPI_Barrier(comm);

    //  (1).    Calculating r1, r2
    if(d >= 0)  //  d is positive for quadratic formula
    {
        r2 = (d + sqrt((d * d) - 4 * alpha)) / (2 * alpha);
        r1 = alpha / r2;
    }
    else    //  d is negative for quadratic formula
    {
        r2 = (d - sqrt((d * d) - 4 * alpha)) / (2 * alpha);
        r1 = alpha / r2;
    }

    //  Scatter RHS vector b across all processes
    MPI_Scatter(b, N/m, MPI_DOUBLE, localB, N/m, MPI_DOUBLE, ROOT, comm);

    //  (3).    Calc Yi for each process's mini-system
    yVector[0] = localB[0];
    for(i = 1; i < (N/m); i++)
    {
        //  Set Yi = Bi - (r1 * Yi-1)
        yVector[i] = localB[i] - (r1 * yVector[i-1]);
        //printf("P%d At %d, yi is:%f\n", p, i, yVector[i]);
    }

    //  (4).    Calc Xi for each process's mini-system by Work back through values backwards
    xVector[N/m-1] = yVector[N/m-1]/r2;
    for(i = (N/m) - 2; i >= 0; i--)
    {
        //  Set xi = (Yi - Xi+1) / r2
        xVector[i] = (yVector[i] - xVector[i+1]) / r2;          
        //printf("P%d At %d, xi is:%f\n", p, i, xVector[i]);
    }


    //  (5).    Apply correction to each xi value in solution vector
    
    //  Calc rstar = max{|r1|,|1/r2|}
    rStar = fmax(fabs(r1), fabs(1/r2));
    printf("rstar:%f\n", rStar);

    //  calc t = (ln(epsilon) + ln(|d| - |alpha| - 1) - ln(MAX{|r2 / (r2 - r1)|}, 1)) / ln(rStar)
    t = (log(epsilon) + log(fabs(d) - fabs(alpha) - 1) - log(fmax(fabs(r2 / (r2 - r1)),1))) / log(rStar);
    t = floor(t);       //Round off t
    printf("t:%f\n", t);
    
    MPI_Barrier(comm);
    
    //  Tag first as 50, last as 99 
    if(p == 0)  //  First Processor
    {
        //  Send last to next, Recv first from next
        MPI_Send(&xVector[N/m - 1], 1, MPI_DOUBLE, p + 1, 99, comm);
        MPI_Recv(&localFirst, 1, MPI_DOUBLE, p + 1, 50, comm, MPI_STATUS_IGNORE);

        //  Apply q up
        omega = (alpha / (alpha - (r1*r1))) * localFirst * (-1 * r1);
        for(i = 0; i < t; i++)
        {
            xVector[i] = xVector[i] + omega;
            omega = omega * (r1 * -1);  //  Update omega
        }
        //  Apply Special P down with different coefficient
        omega = xVector[0] / (alpha - r1*r1);
        for(i = (N/m - 1); i > t; i--)
        {
            xVector[i] = xVector[i] + omega;
            omega = omega * (r1 * -1);  //  Update omega
        }
    }
    else if(p == (m - 1))   //  Last processor
    {
        //  Send first to previous, Recieve last from previous
        MPI_Send(&xVector[0], 1, MPI_DOUBLE, p-1, 50, comm);
        MPI_Recv(&localLast, 1, MPI_DOUBLE, p - 1, 99, comm, MPI_STATUS_IGNORE);
       
        //  Apply p down
        omega = (alpha / (alpha - (r1*r1))) * localLast * (-1 * r1);
        for(i = (N/m - 1); i > t; i--)
        {
            xVector[i] = xVector[i] + omega;
            omega = omega * (r1 * -1);  //  Update omega
        }
    }
    else    //  All other processors p(1 : m-1)
    {
        //  Send last to next, first to previous
        MPI_Send(&xVector[N/m - 1], 1, MPI_DOUBLE, p + 1, 99, comm);
        MPI_Send(&xVector[0], 1, MPI_DOUBLE, p - 1, 50, comm);
        
        //  Recv last from previous, first from next
        MPI_Recv(&localLast, 1, MPI_DOUBLE, p - 1, 99, comm, MPI_STATUS_IGNORE);
        MPI_Recv(&localFirst, 1, MPI_DOUBLE, p + 1, 50, comm, MPI_STATUS_IGNORE);
        
        //  Apply q up
        omega = (alpha / (alpha - (r1*r1))) * localFirst * (-1 * r1);
        for(i = 0; i < t; i++)
        {
            xVector[i] = xVector[i] + omega;
            omega = omega * (r1 * -1);  //  Update omega
        }
        //  Apply p down
        omega = (alpha / (alpha - (r1*r1))) * localLast * (-1 * r1);
        for(i = (N/m - 1); i > t; i--)
        {
            xVector[i] = xVector[i] + omega;
            omega = omega * (r1 * -1);  //  Update omega
        }
    }
    
    //  Gather calculated Mini-systems into result vector
    MPI_Gather(&xVector, N/m, MPI_DOUBLE, &finalXVector, N/m, MPI_DOUBLE, ROOT, comm);
    
    //  Display gathered result vector
    if(p == 0)
    {   
        for(i = 0; i < N; i++)
            printf("X%d--[%f]\n",i,finalXVector[i]);
    }

    MPI_Finalize();
}

void main()
{
    splitAndCorrect(64);
}