#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    int rank, size;
    int N = 8; // Total size of the array

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int *sendbuf = NULL; // Buffer on the root process
    int recvbuf[2];      // Each process receives 2 elements (N / size)

    if (rank == 0) {
        // Root process initializes the array
        sendbuf = (int *)malloc(N * sizeof(int));
        for (int i = 0; i < N; i++) {
            sendbuf[i] = i + 1; // Example: [1, 2, 3, 4, 5, 6, 7, 8]
        }
        printf("Root process has array: ");
        for (int i = 0; i < N; i++) {
            printf("%d ", sendbuf[i]);
        }
        printf("\n");
    }

    // Scatter the array
    MPI_Scatter(sendbuf, 2, MPI_INT, recvbuf, 2, MPI_INT, 0, MPI_COMM_WORLD);

    // Each process prints its received chunk
    printf("Process %d received: %d %d\n", rank, recvbuf[0], recvbuf[1]);

    // Free memory on root process
    if (rank == 0) {
        free(sendbuf);
    }

    MPI_Finalize();
    return 0;
}
