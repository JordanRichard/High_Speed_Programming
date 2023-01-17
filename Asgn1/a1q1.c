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
	//int arr2[matrixSize][matrixSize];

	
	// Populates each array
	for(int i = 0; i < matrixSize; i++)
	{
		for(int j = 0; j< matrixSize; j++)
		{
			*(arr1 + i*matrixSize + j) = rand() % 10;
			//arr2[i][j] = rand();
		}
	}
	

	//Pretty-prints a matrix to the screen
	printf("-----------------------------\n");
	for(int i = 0; i < matrixSize; i++)
	{
		for(int j = 0; j< matrixSize; j++)
		{
			int val = *(arr1 + i*matrixSize + j);	//Finds next entry based on pointer aritihmetic
			printf("[%i] ",val);

			//arr2[i][j] = rand();
		}
		printf("\n");
	}
	printf("-----------------------------\n");


	free(arr1);
	printf("Memory de-allocated.\n");
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