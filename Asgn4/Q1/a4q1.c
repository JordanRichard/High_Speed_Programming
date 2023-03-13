#include <stdio.h>
#include <mpi.h>
#include <time.h>



/******************************************************************************
 *  Method: randomIntFrequency: Given a value N, generates an array of N random 
 *          integers in parallel and creates a frequency table, updated to use
 * 			OMP commands.
 * 
 *  Input:  N - The number of values to be processed.
 * 
 *  Output: Nil
 * 
 * ****************************************************************************/
void randomIntFrequency(int N)
{
    int randomIntegers[N];
	int frequencyTable[10];

	srand(time(NULL));

    int p;
    int m;

    /*  Open the parallel machine   */
    MPI_Init(NULL,NULL);
    MPI_Comm comm = MPI_COMM_WORLD; //Shorthand

    MPI_Comm_rank(comm, &p);
    MPI_Comm_size(comm, &m);

	// Calculates start and endpoints of each process
	int start = (p * (N / m));
	int end = ((p + 1) * (N / m) -1);

    printf("P%d S:%d E:%d\n",p,start,end);

    //Generates randomized value between 1-10 and adds to array
	for(int i = start; i <= end; i++)
	{
		int r = (rand() % (10-1 +1)) + 1;
		randomIntegers[i] = r;
        printf("Process %d making value %d and placing in randarray index %d\n",p,r,i);
	}

    // Creates frequency table of each value generated
	for(int j = start; j <= end; j++)
	{
		int value = randomIntegers[j];
		frequencyTable[value -1] = frequencyTable[value -1] + 1;
        printf("P%d got value %d and put in box %d\n",p,value,value-1);
	}


    if(p == 0)
    {
        //Wipe frequency table
        for(int i = 0; i < 10; i++)
            frequencyTable[i] = 0;

        printf("Value:\tFrequency:\tRelative:\n");
        for(int i = 0; i < 10; i++)
        {
            float relativeFrequency = (float)frequencyTable[i] / (float)N;
            printf("[%i] \t%i \t\t%.4f\n", i + 1,frequencyTable[i],relativeFrequency);
        }
    }

    MPI_Finalize();
    /*  Close the parallel machine  */

}


/******************************************************************************
 *  Method: main: Calls the method required providing an argument N, the number
 *			of elements to process.
 * 
 *  Input:  Nil
 * 
 *  Output: Nil
 * 
 * ****************************************************************************/
int main()
{
    randomIntFrequency(20);
}