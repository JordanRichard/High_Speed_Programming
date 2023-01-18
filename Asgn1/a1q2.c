#include <stdlib.h>
#include <stdio.h>

// ****************************************************************************
//		Author: 	Jordan Alexander Richard
//		SN: 		3606213
//
//		Name: 		a1q2.c 	
//		Purpose: 
//
//		Takes an input matrix from a provided file.
//
// ****************************************************************************

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