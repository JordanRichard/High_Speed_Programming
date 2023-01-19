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
 *  Method: sumSquares: 
 * 
 * 
 *  Input:	
 * 
 *  Output: 
 * 
 * ****************************************************************************/
void sumSquares()
{
	FILE *infile = fopen("data3.txt","r");
	int elems;

	fscanf(infile,"%i", &elems);	//Finds the number of elements

	int *X = (int *)malloc(elems * elems * sizeof(int));
	
	for(int i = 0; i < elems; i++)
	{
		for(int j = 0; j < elems; j++)
		{
			int Xi;
			fscanf(infile,"%i", &Xi);
			*(X + i * elems + j) = Xi;
			printf("[%i]", Xi);	
		}
	}

	free(X);
	fclose(infile);
}


void main ()
{
	sumSquares();
}