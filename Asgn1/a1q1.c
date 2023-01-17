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

	int arr1[matrixSize][matrixSize];
	int arr2[matrixSize][matrixSize];

	// Populates each array
	for(int i = 0; i < matrixSize; i++)
	{
		for(int j = 0; j< matrixSize; j++)
		{
			arr1[i][j] = rand();
			arr2[i][j] = rand();
		}
	}

	// Prints out the array
	/*
	int c=0;
	for(int i = 0; i < matrixSize; i++)
	{
		printf("Value %i:",i);
		printf("%i\n",arr1[i]);
		c++;
	}
	*/
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