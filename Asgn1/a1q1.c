#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
#		#Author: Jordan Alexander RIchard
#		#SN: 3606213
#
#		Name: a1q1.c 	
#		Purpose: 
#
#		This program randomly generates two NxN matrices based on a size specified by the 
#		user. Outputs are saved to a file.
#
*/


void matrixMult(int matrixSize)
{
	printf("Creating the matrices...\n");
	
	// Seeds the random number generator
	srand(time(NULL));

	//Allocates heap memory for the matrix
	int *arr1 = (int *)malloc(matrixSize * matrixSize * sizeof(int));	
	int *arr2 = (int *)malloc(matrixSize * matrixSize * sizeof(int));
	int *arr3 = (int *)malloc(matrixSize * matrixSize * sizeof(int));
	

	// Populates each array
	for(int i = 0; i < matrixSize; i++)
	{
		for(int j = 0; j< matrixSize; j++)
		{
			*(arr1 + i*matrixSize + j) = rand() % 10;
			*(arr2 + i*matrixSize + j) = rand() % 10;
		}
	}
	

	//Pretty-prints a matrix to the screen
	printf("----------Matrix A---------------\n");
	for(int i = 0; i < matrixSize; i++)
	{
		for(int j = 0; j< matrixSize; j++)
		{
			int val = *(arr1 + i*matrixSize + j);	//Finds next entry based on current ptr posn
			printf("[%i] ",val);
		}
		printf("\n");
	}


	printf("----------Matrix B---------------\n");
	for(int i = 0; i < matrixSize; i++)
	{
		for(int j = 0; j< matrixSize; j++)
		{
			int val = *(arr2 + i*matrixSize + j);	//Finds next entry based on current ptr posn
			printf("[%i] ",val);
		}
		printf("\n");
	}


	//Multiplies the two matrices together via dot-product
	printf("-----------------------------\nMultiplying matrices...\n");
	for(int i = 0; i < matrixSize; i++)
	{
		for(int j = 0; j< matrixSize; j++)
		{
			int total = 0;

			//Calculates the dot product for each entry of the new matrix
			for(int k = 0; k < matrixSize;k++)
			{
				int dotProduct = *(arr1 + i*matrixSize+k) * *(arr2 + k*matrixSize+j);
				total += dotProduct;
			}

			*(arr3 + i * matrixSize + j) = total;	//Assigns calculated dot-product to matrix entry
		}
		printf("\n");
	}

	// Pretty-Print Resultant matrix
	printf("----------Matrix AB--------------\n");
	for(int i = 0; i < matrixSize; i++)
	{
		for(int j = 0; j< matrixSize; j++)
		{
			int val = *(arr3 + i*matrixSize + j);	//Finds next entry based on current ptr posn
			printf("[%i] ",val);
		}
		printf("\n");
	}
	printf("-------------------------\n");

	free(arr1);
	free(arr2);
	free(arr3);
	printf("Memory de-allocated.\n");

	// TODO- Pipe original matrixes to file then the result

}

int main()
{
	// Takes user input to call function
	int n;
	printf("Please input a number\n");
	scanf("%i", &n);				

	matrixMult(n);

	printf("All done!\n");
}