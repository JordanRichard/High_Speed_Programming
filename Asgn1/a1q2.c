#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/******************************************************************************
 * 	
 * Driver program to execute method for question 2.
 *
 *  
 * @author	Jordan Alexander Richard
 * @version CS 3123 - Assignment 1 Question 2
******************************************************************************/


/******************************************************************************
 *  Method: findMean: 
 * 
 * 
 *  Input:	FILE *input - 	File containing our data set.
 * 			int N - 		Size of the data set to be processed.
 *  
 * Output:	double mk - 	Calculated mean of our data set. 
 * 
 * ****************************************************************************/
double mean(FILE* input,int N)
{
	double mk = 0;
	double xk = 0;

	for (int k = 1; k <= N; k++)
	{

		if(k == 1)
		{
			fscanf(input,"%lf",&mk);	//M1 
			printf("M1: %lf\n",mk);
		}	
		else
		{
			fscanf(input,"%lf", &xk); // Reads value at posn k
			printf("On value %lf\n",xk);

			mk = mk + ((xk - mk) / k);	
			printf("Mk: %lf\n",mk);
		}
	}

	return mk;
}


void main ()
{
	FILE *infile = fopen("data3.txt","r");

	int elems;
	fscanf(infile,"%i", &elems);	//Finds the number of elements

	double r = mean(infile,elems);

	printf("Calculated Mean: %f\n",r);

	fclose(infile);
}