#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/******************************************************************************
 * 	
 * Driver program to execute function for question 2.
 *
 *  
 * @author	Jordan Alexander Richard
 * @version CS 3123 - Assignment 1 Question 2
******************************************************************************/


/******************************************************************************
 *  Method: calcStats: Given an input file and number of data points, 
 * 			calculates the mean and standard deviation of a dataset.
 * 
 * 
 *  Input:	FILE *input - 	File containing our data set.
 * 			int N - 		Number of data points to be processed.
 *  
 * 	Output:	Nil 
 * 
 * ****************************************************************************/
void calcStats(FILE* input, int N)
{
	double mk = 0;
	double xk = 0;
	double sk = 0;

	for (int k = 1; k <= N; k++)
	{
		if(k == 1)
		{
			// Establishes initial values for std. dev. and mean
			fscanf(input,"%lf",&mk); 
			sk = 0;			
		}	
		else
		{
			// Reads value at current position
			fscanf(input,"%lf", &xk); 

			// Save previous Mk before it is updated
			double lastMk = mk; 

			// Calculating std. dev. and mean for current iteration
			mk = mk + ((xk - mk) / k);
			sk = sk + (xk - lastMk)*(xk - mk);	
		}
	}

	// Finish calculating std. dev
	double dev = sqrt(sk / (N - 1));

	printf("Calculated Mean: %lf\n",mk);
	printf("Calculated Std.Dev: %lf\n",dev);
}


void main ()
{
	FILE *infile = fopen("data3.txt","r");

	// Finds the number of elements
	int elems;
	fscanf(infile,"%i", &elems);	

	calcStats(infile, elems * elems);

	fclose(infile);
}