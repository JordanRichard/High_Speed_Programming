#include <stdio.h>
#include <mpi.h>

#define N 8

int main(int argc, char** argv) {
    int rank, size;
    int i, j;
    int sendbuf[N], recvbuf[N*N];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (i = 0; i < N; i++) {
        sendbuf[i] = rank;
    }

    MPI_Gather(sendbuf, N, MPI_INT, recvbuf, N, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                printf("%d ", recvbuf[i*N+j]);
            }
            printf("\n");
        }
    }

    MPI_Finalize();

    return 0;
}


