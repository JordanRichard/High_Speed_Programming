#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/******************************************************************************
 * 	
 * Driver program to carry out matrix addition for question 3. 
 *
 *  
 * @author	Jordan Alexander Richard
 * @version CS 3123 - Assignment 3 Question 3 - Sequential Variant
******************************************************************************/

/******************************************************************************
 *  Method: matrixAddition: Given a value N, generates two NxN matrices of randomly
 *          generated double values and adds them together.
 * 
 *  Input:  N - The size (order) of matrices to be created.
 * 
 *  Output: Nil
 * 
 * ****************************************************************************/
void matrixAddition(int N)
{

    double timeDifference;

    double arr1[N][N];
    double arr2[N][N];
    double arr3[N][N];

    srand(time(NULL));

    double before = clock();
      
    // Populates each matrix with random double values
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            double r = (double)(rand());
            double r2 =(double)(rand());

            arr1[i][j] = r;
            arr2[i][j] = r2;
        }  
    }

    // Adds the two matrices together
    for(int x = 0; x <= N; x++)
    {
        for(int y = 0; y <= N; y++)
        {
            double arrSum = arr1[x][y] + arr2[x][y];
            arr3[x][y] = arrSum;
        }
    }
        
    double after = clock();
    timeDifference = (after - before) / CLOCKS_PER_SEC;


    printf("Added two %iX%i matrices in %lf (s).\n",N,N,timeDifference);

    //Print resultant matrix for verification

    printf("----------Result-------------\n");
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            printf("[%.2f]",arr3[i][j]);
        }
        printf("\n");
    }

}


int main()
{
    matrixAddition(4);
}