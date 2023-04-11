#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <math.h>

#define ROOT 0

/******************************************************************************
 * 	
 * Driver program to solve a tridiagonal toeplitz system for question 6.
 *
 *  
 * @author	Jordan Alexander Richard
 * @version CS 3123 - Assignment 4 Question 6
******************************************************************************/

/******************************************************************************
 *  Method: splitAndCorrect: Given a value N, solves a tridiagonal toeplitz
 *              system AX = B of the form [2,10,1] for x using McNally's m-split
 *              and correct algorithm in MPI.
 * 
 *  Input:  N - Size of the NxN system to be solved.
 * 
 *  Output: Nil
 * 
 * ****************************************************************************/
void splitAndCorrect(int N)
{
    int p,m,i;
    double b[N];
    double r1,r2,omega,rStar,t;
    double startTime,endTime,globalStartTime,globalEndTime;

    double alpha = 2;
    double d = 10;
    double epsilon = pow(2,-12);

    MPI_Init(NULL,NULL);
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Comm_rank(comm, &p);
    MPI_Comm_size(comm, &m);

    double localB[N/m];
    double xVector[N/m], yVector[N/m];
    double finalXVector[N];
    double localFirst,localLast;

    //  Initialize result vector b
    if(p == 0)
    {
        for(i = 0; i < N; i ++)
        {
            b[i]= alpha * (i-1) + d * (i) + (i + 1);  //  first and last need fixing
            //b[i] = 1;
        }
    }

    // Start timing each process
    MPI_Barrier(comm);
    startTime = MPI_Wtime();

    //  Calculate values r1, r2
    if(d >= 0)  //  d is positive for quadratic formula
        r2 = (d + sqrt((d*d) - 4 * alpha)) / (2 * alpha);
    else        //  d is negative for quadratic formula
        r2 = (d - sqrt((d*d) - 4 * alpha)) / (2 * alpha);

    r1 = alpha / r2;

    //  Scatter RHS vector b across all processes
    MPI_Scatter(b, N/m, MPI_DOUBLE, localB, N/m, MPI_DOUBLE, ROOT, comm);

    //  Calculate Yi for each process's mini-system
    yVector[0] = localB[0];
    for(i = 1; i < (N/m); i++)
        yVector[i] = localB[i] - (r1 * yVector[i - 1]);

    //  Calculate Xi for each process's mini-system by working through values backwards
    xVector[N/m - 1] = yVector[N/m - 1] / r2;
    for(i = (N/m) - 2; i >= 0; i--)
        xVector[i] = (yVector[i] - xVector[i + 1]) / r2;



    //  Apply correction to each xi value in solution vector

    //  Calculate values rStar, t to limit number of corrections to perform
    rStar = fmax(fabs(r1), fabs(1 / r2));
    t = ceil((log(epsilon) + log(fabs(d) - fabs(alpha) - 1) 
        - log(fmax(fabs(r2 / (r2 - r1)), 1))) / log(rStar));
    
    MPI_Barrier(comm);
    
    //  Exchange first,last values between processors and perform p,q corrections 
    if(p == 0)  //  First Processor
    {
        //  Send last value to next process, Recv first value from next process
        MPI_Send(&xVector[N/m - 1], 1, MPI_DOUBLE, p + 1, 99, comm);
        MPI_Recv(&localFirst, 1, MPI_DOUBLE, p + 1, 50, comm, MPI_STATUS_IGNORE);

        //  Apply q correction upwards
        omega = (((r1*r1) - alpha) * xVector[N/m - 1] + r1 * localFirst) 
            / (alpha - r1*r1) * (-1 / r2);
        for(i = (N/m - 1); i > t; i--)
        {
            xVector[i] = xVector[i] + omega;
            omega = omega * (-1 / r1);  //  Update omega
        }
        //  Apply Special p correction downwards with different coefficient
        omega = -r1 * (xVector[0] / (alpha - r1*r1));
        for(i = 0; i < t; i++)
        {
            xVector[i] = xVector[i] + omega;
            omega = omega * (r1 * -1);  //  Update omega
        }
    }
    else if(p == (m - 1))   //  Last processor
    {
        //  Send first value to previous process, Recieve last value from previous process
        MPI_Send(&xVector[0], 1, MPI_DOUBLE, p - 1, 50, comm);
        MPI_Recv(&localLast, 1, MPI_DOUBLE, p - 1, 99, comm, MPI_STATUS_IGNORE);
       
        //  Apply p correction downwards
        omega = (alpha / (alpha - (r1*r1))) * localLast * (-1 * r1);
        for(i = 0; i < t; i++)
        {
            xVector[i] = xVector[i] + omega;
            omega = omega * (r1 * -1);  //  Update omega
        }
    }
    else    //  All other processors p(1 : m-1)
    {
        //  Send last value to next process, first to previous process
        MPI_Send(&xVector[N/m - 1], 1, MPI_DOUBLE, p + 1, 99, comm);
        MPI_Send(&xVector[0], 1, MPI_DOUBLE, p - 1, 50, comm);
        
        //  Recv last value from previous process, first from next process
        MPI_Recv(&localLast, 1, MPI_DOUBLE, p - 1, 99, comm, MPI_STATUS_IGNORE);
        MPI_Recv(&localFirst, 1, MPI_DOUBLE, p + 1, 50, comm, MPI_STATUS_IGNORE);
        
        //  Apply q correction upwards
        omega = (((r1*r1) - alpha) * xVector[N/m - 1] + r1 * localFirst) 
            / (alpha - r1*r1) * (-1 / r2);
        for(i = (N/m - 1); i > t; i--)
        {
            xVector[i] = xVector[i] + omega;
            omega = omega * (-1 / r1);  //  Update omega
        }
        //  Apply p correction downwards
        omega = (alpha / (alpha - (r1*r1))) * localLast * (-1 * r1);
        for(i = 0; i < t; i++)
        {
            xVector[i] = xVector[i] + omega;
            omega = omega * (r1 * -1);  //  Update omega
        }
    }
    
    //  Gather calculated Mini-systems into result vector
    MPI_Gather(&xVector, N/m, MPI_DOUBLE, &finalXVector, N/m, MPI_DOUBLE, ROOT, comm);
    
    endTime = MPI_Wtime();

    //  Calculate elapsed runtime over all processes
    MPI_Reduce(&startTime, &globalStartTime, 1, MPI_DOUBLE, MPI_MIN, ROOT, comm);
    MPI_Reduce(&endTime, &globalEndTime, 1, MPI_DOUBLE, MPI_MIN, ROOT, comm);

    if(p == 0)
    {   
        //  Display gathered result vector if desired
        /*
        for(i = 0; i < N; i++)
            printf("X%d--[%f]\n",i,finalXVector[i]);
        */

        //  Display parallel execution time in seconds
        printf("%f\n", (globalEndTime - globalStartTime));
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
    splitAndCorrect(pow(32,4));
}