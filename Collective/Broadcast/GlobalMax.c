#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    int rank, size, N, global_max;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int *arr = NULL;

    if (rank == 0) {
        printf("Enter vector size: ");
        scanf("%d", &N);

        arr = (int*) malloc(N * sizeof(int));
        for (int i = 0; i < N; i++) {
            arr[i] = rand() % 100; 
        }

        printf("The array being broadcasted: ");
        for (int i = 0; i < N; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");
    }

    // Broadcast the size of the array to all processes
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Allocate memory for the array in all processes
    if (rank != 0) {
        arr = (int*) malloc(N * sizeof(int));
    }

    // Broadcast the array to all processes
    MPI_Bcast(arr, N, MPI_INT, 0, MPI_COMM_WORLD);

    // Determine chunk size and local boundaries
    int chunk_size = N / size;
    int start = rank * chunk_size;
    int end = (rank == size - 1) ? N : start + chunk_size;

    int local_max = arr[start];
    for (int i = start + 1; i < end; i++) {
        if (arr[i] > local_max) {
            local_max = arr[i];
        }
    }

    // Reduce to find the global maximum
    MPI_Reduce(&local_max, &global_max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Max element in array: %d\n", global_max);
    }

    // Free allocated memory
    free(arr);

    MPI_Finalize();
    return 0;
}
