#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <math.h>

#define ROOT 0

void splitAndCorrect(int N)
{
    int p,m,i,j;
    double b[N];
    double r1,r2,xi,yi,delta,omega;

    //Define as doubles for precision
    double alpha = 2;
    double d = 10;
    double rStar;
    double t;

    double localFirst,localLast;


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
            b[i]= alpha*(i-1) + d*(i) + (i+1);  // first and last need fixing
            //b[i] = 1;
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

        //printf("D is positive - r2: %f - r1: %f\n",r2,r1);
    }
    else
    {
        //  d is negative for quadratic formula
        r2 = (d - sqrt((d * d) - 4 * alpha)) / (2 * alpha);
        r1 = alpha / r2;
        
        //printf("D is negative - r2: %f - r1: %f\n",r2,r1);
    }
    MPI_Barrier(comm);

    //  Scatter and print RHS vector b across all processes
    MPI_Scatter(b, N/m, MPI_DOUBLE, localB, N/m, MPI_DOUBLE, ROOT, comm);
    /*
    printf("P%d:", p);
    for(i=0; i < (N/m); i++)
        printf("[%.1f]", localB[i]);
    printf("\n");
    MPI_Barrier(comm);
    */

    //  (3).    Calc Yi for each process's mini-system
    yVector[0] = localB[0];
    //printf("P%d At 0, yi is:%f\n", p, yVector[0]);
    for(i = 1; i < (N/m); i++)
    {
        //  Set Yi = Bi - (r1 * Yi-1)
        yVector[i] = localB[i] - (r1 * yVector[i-1]);
        //printf("P%d At %d, yi is:%f\n", p, i, yVector[i]);
    }

    //  (4).    Calc Xi for each process's mini-system by Work back through values backwards
    xVector[N/m-1] = yVector[N/m-1]/r2;
    //printf("P%d At %d, xi is:%f\n", p,(N/m-1), xVector[N/m-1]);
    for(i = (N/m) - 2; i >= 0; i--)
    {
        //  Set xi = (Yi - Xi+1) / r2
        xVector[i] = (yVector[i] - xVector[i+1]) / r2;          
        //printf("P%d At %d, xi is:%f\n", p, i, xVector[i]);
    }

    //  (5).    Apply correction to each xi value in solution vector

    //Calc rstar = max{|r1|,|1/r2|}
    rStar = fmax(fabs(r1),fabs(1/r2));
    printf("rstar:%f\n",rStar);

    //  calc t = (ln(epsilon) + ln(|d| - |alpha| - 1) - ln(MAX{|r2 / (r2 - r1)|}, 1)) / ln(rStar)
    //  Where epsilon => Arbitrarily small #
    double epsilon = 0.001;
    t = (log(epsilon) + log(fabs(d) - fabs(alpha) - 1) - log(fmax(fabs(r2 / (r2 - r1)),1))) / log(rStar);
    t = floor(t);       //Round off t
    printf("t:%f\n",t);
    
    MPI_Barrier(comm);
    
    if(p == 0)  //First Processor
    {
        // Tag first as 50, last as 99 
        
        // Send last to next, Recv first from next
        MPI_Send(&xVector[N/m - 1], 1, MPI_DOUBLE, p + 1, 99, comm);
        MPI_Recv(&localFirst, 1, MPI_DOUBLE, p + 1, 50, comm, MPI_STATUS_IGNORE);

        //Apply q up
        for(i = 0; i < t; i++)
        {
            delta = localFirst;
            //  Compute correction (omega) and apply to this xi value
            omega = (alpha / (alpha - (r1*r1))) * delta * (-1 * r1);
            xVector[i] = xVector[i] + omega;
        }

        // Apply Special P down with updated coefficient
        
        for(i = (N/m - 1); i > t; i--)
        {
            omega = xVector[0] / (alpha - r1*r1);
        }
    
    }
    else if(p == (m - 1))   // Last processor
    {
        // Tag first as 50, last as 99 
        
        // Send first to previous, Recieve last from previous
        MPI_Send(&xVector[0], 1, MPI_DOUBLE, p-1, 50, comm);
        MPI_Recv(&localLast, 1, MPI_DOUBLE, p - 1, 99, comm, MPI_STATUS_IGNORE);
       
        // Apply p down
        for(i = (N/m - 1); i > t; i--)
        {
            delta = localLast;
            //  Compute correction (omega) and apply to this xi value
            omega = (alpha / (alpha - (r1*r1))) * delta * (-1 * r1);
            xVector[i] = xVector[i] + omega;
        }
    }
    else    // All other processors p(1 : m-1)
    {
        // Tag first as 50, last as 99 
        
        // Send last to next, first to previous
        MPI_Send(&xVector[N/m - 1], 1, MPI_DOUBLE, p + 1, 99, comm);
        MPI_Send(&xVector[0], 1, MPI_DOUBLE, p - 1, 50, comm);
        
        // Recv last from previous, first from next
        MPI_Recv(&localLast, 1, MPI_DOUBLE, p - 1, 99, comm, MPI_STATUS_IGNORE);
        MPI_Recv(&localFirst, 1, MPI_DOUBLE, p + 1, 50, comm, MPI_STATUS_IGNORE);
        
        //Apply q up
        for(i = 0; i < t; i++)
        {
            delta = localFirst;
            //  Compute correction (omega) and apply to this xi value
            omega = (alpha / (alpha - (r1*r1))) * delta * (-1 * r1);
            xVector[i] = xVector[i] + omega;
        }
        // Apply p down
        for(i = (N/m - 1); i > t; i--)
        {
            delta = localLast;
            //  Compute correction (omega) and apply to this xi value
            omega = (alpha / (alpha - (r1*r1))) * delta * (-1 * r1);
            xVector[i] = xVector[i] + omega;
        }
    }
    

    // TODO - GATHER CORRECTED MINI-SYSTEMS
    // TODO - DISPLAY

    MPI_Finalize();
}

void main()
{
    splitAndCorrect(16);
}