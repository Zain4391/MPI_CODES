#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int sendbuf[2]; // Each process has 2 elements
    for (int i = 0; i < 2; i++) {
        sendbuf[i] = rank * 2 + i + 1; // Example: Process 0 -> [1, 2], Process 1 -> [3, 4], etc.
    }

    int *recvbuf = NULL; // Buffer to gather all data on the root process
    if (rank == 0) {
        recvbuf = (int *)malloc(size * 2 * sizeof(int));
    }

    // Gather the data
    MPI_Gather(sendbuf, 2, MPI_INT, recvbuf, 2, MPI_INT, 0, MPI_COMM_WORLD);

    // Root process prints the gathered array
    if (rank == 0) {
        printf("Root process gathered array: ");
        for (int i = 0; i < size * 2; i++) {
            printf("%d ", recvbuf[i]);
        }
        printf("\n");
        free(recvbuf);
    }

    MPI_Finalize();
    return 0;
}
