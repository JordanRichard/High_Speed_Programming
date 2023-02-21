#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/******************************************************************************
 * 	
 * Driver program to execute function for question 3.
 *
 *  
 * @author	Jordan Alexander Richard
 * @version CS 3123 - Assignment 1 Question 3
******************************************************************************/

/******************************************************************************
 *  Method: reimannSum: 
 * 
 * 
 *  Input:  double A - Value to determine the probability of P(Z =< A).
 *  
 *  Output: Nil.
 * 
 * ****************************************************************************/
void reimannSum(double A)
{
    double sum = 0;
    double fx = 0;

    printf("Calculating P(Z <= %lf)\n",A);

    // Splits interval into n blocks of size h - Smaller h is more precise
    double h = 0.01;
    double n = (A - (-10) / h);

    // Iterates over each trapezoid under the curve
    for (int x = 1; x <= n; x++)
    {
        // Calculates f(x) and f(x + 1) to calculate the trapezoid's area 
        double fx = (1 / (sqrt(2 * M_PI))) * exp(-0.5 *(fx * fx)); 
        double nextFx = (1 / (sqrt(2 * M_PI))) * exp(-0.5 *((fx+1) * (fx+1)));

        sum += h * ((fx + nextFx) / 2);
    }   

    printf("Z-Score: %lf\n",sum);
}

void main()
{
    reimannSum(3.00);
}