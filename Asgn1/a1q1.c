#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/******************************************************************************
 * 	
 * Driver program to execute matrix multiplication method for question 1.
 *
 *  
 * @author	Jordan Alexander Richard
 * @version CS 3123 - Assignment 1 Question 1
******************************************************************************/


/******************************************************************************
 *  Method: matrixMult: Given a square matrix order N, this method randomly 
 * 			generates two NxN matrices based on a size specified by the user. 
 * 			Multiplies these matrices together and stores resultant matrix to 
 * 			a file.
 * 
 * 
 *  Input:	int matrixSize - the size of nxn matrices to be created.
 * 
 *  Output: Nil
 * 
 * ****************************************************************************/
void matrixMult(int matrixSize)
{
	printf("Creating the matrices...\n");
	
	// Seeds the random number generator
	srand(time(NULL));

	//Allocates heap memory for the matrices
	int *arr1 = (int *)malloc(matrixSize * matrixSize * sizeof(int));	
	int *arr2 = (int *)malloc(matrixSize * matrixSize * sizeof(int));
	int *arr3 = (int *)malloc(matrixSize * matrixSize * sizeof(int));
	

	// Populates each array
	for(int i = 0; i < matrixSize; i++)
	{
		for(int j = 0; j< matrixSize; j++)
		{
			//Generates integer of max 10
			*(arr1 + i * matrixSize + j) = rand() % 10;	
			*(arr2 + i * matrixSize + j) = rand() % 10;
		}
	}
	

	// Multiplies the two matrices together via dot-product
	printf("Multiplying matrices...\n");

	for(int i = 0; i < matrixSize; i++)
	{
		for(int j = 0; j < matrixSize; j++)
		{
			int total = 0;

			//Calculates the dot product for each entry of the new matrix
			for(int k = 0; k < matrixSize; k++)
			{
				//Finds next entry based on current ptr posn and matrix size
				int dotProduct = *(arr1 + i * matrixSize + k) * *(arr2 + k * matrixSize + j);
				total += dotProduct;
			}

			//Assigns calculated dot-product to matrix entry
			*(arr3 + i * matrixSize + j) = total;	
		}
	}
	printf("All done multiplication.\n");

	// Pipes all matrices to output file
	FILE *output = fopen("result.txt","w");
	fprintf(output,"----------Matrix A---------------\n");
	for(int i = 0; i < matrixSize; i++)
	{
		for(int j = 0; j < matrixSize; j++)
		{
			int val = *(arr1 + i * matrixSize + j);	
			fprintf(output,"[%i] ",val);
		}
		fprintf(output,"\n");
	}
	fprintf(output,"\n");
	free(arr1);

	fprintf(output,"----------Matrix B---------------\n");
	for(int i = 0; i < matrixSize; i++)
	{
		for(int j = 0; j < matrixSize; j++)
		{
			int val = *(arr2 + i * matrixSize + j);
			fprintf(output,"[%i] ",val);
		}
		fprintf(output,"\n");
	}
	fprintf(output,"\n");
	free(arr2);

	fprintf(output,"----------Matrix AB--------------\n");
	for(int i = 0; i < matrixSize; i++)
	{
		for(int j = 0; j < matrixSize; j++)
		{
			int val = *(arr3 + i * matrixSize + j);
			fprintf(output,"[%i]",val);
		}
		fprintf(output,"\n");
	}
	fprintf(output,"---------------------------------\n");
	free(arr3);

	fclose(output);
	printf("Results saved in file \"result.txt\".\n");
}

int main()
{
	int n;
	printf("Please the desired matrix size (N).\n");
	scanf("%i", &n);				

	matrixMult(n);
}