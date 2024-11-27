/*
Equivalent OpenMP Code:

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    int N;
    printf("Enter size of vector: ");
    scanf("%d", &N);
    int* data = (int*)malloc(N * sizeof(int));

    // Initialize the array with random numbers
    for (int i = 0; i < N; i++) {
        data[i] = rand() % 101;
    }

    printf("Array:\n");
    for (int i = 0; i < N; i++) {
        printf("%d ", data[i]);
    }
    printf("\n");

    int sum = 0;

    // Parallel reduction to compute the sum
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < N; i++) {
        sum += data[i];
    }

    printf("Sum of array = %d\n", sum);

    free(data);
    return 0;
}

The following MPI code is an implementation of the above OpenMP code.
*/

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    int rank, size, N;
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int* data = NULL;

    // Master process takes input and initializes the array
    if (rank == 0) {
        printf("Enter size of vector: ");
        scanf("%d", &N);
        data = (int*)malloc(N * sizeof(int));

        for (int i = 0; i < N; i++) {
            data[i] = rand() % 101;
        }

        printf("Array being broadcasted:\n");
        for (int i = 0; i < N; i++) {
            printf("%d ", data[i]);
        }
        printf("\n");
    }

    // Broadcast the size of the array
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Allocate memory for other processes
    if (rank != 0) {
        data = (int*)malloc(N * sizeof(int));
    }

    // Broadcast the entire array
    MPI_Bcast(data, N, MPI_INT, 0, MPI_COMM_WORLD);

    // Debugging step
    printf("Process %d received array: ", rank);
    for (int i = 0; i < N; i++) {
        printf("%d ", data[i]);
    }
    printf("\n");

    // Compute the local sum
    int chunk_size = N / size;
    int s = rank * chunk_size;
    int e = (rank == size - 1) ? N : s + chunk_size;

    int local_sum = 0;
    for (int i = s; i < e; i++) {
        local_sum += data[i];
    }

    // Reduce the local sums to calculate the global sum
    int global_sum;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Print the result from the root process
    if (rank == 0) {
        printf("Sum of array = %d\n", global_sum);
    }

    // Clean up
    MPI_Finalize();
    free(data);
    return 0;
}
