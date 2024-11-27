/*
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
    int N = 100;
    double result[N];

    // Parallel loop for computing the square root of numbers
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        result[i] = sqrt(i);
    }

    printf("Computed square roots:\n");
    for (int i = 0; i < N; i++) {
        printf("sqrt(%d) = %.2f\n", i, result[i]);
    }

    return 0;
}
    CODE BELOW is the implementation for MPI
*/

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char** argv) {
    int rank, size, N;
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    double* data = NULL;

    // Master process takes input and initializes the array
    if (rank == 0) {
        printf("Enter size of vector: ");
        scanf("%d", &N);
        data = (double*)malloc(N * sizeof(double));

        for (int i = 0; i < N; i++) {
            data[i] = i;
        }

        printf("Original Vector:\n");
        for (int i = 0; i < N; i++) {
            printf("%.2f ", data[i]);
        }
        printf("\n");
    }

    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int local_N = N /size;
    double *local_data = (double*) malloc (local_N * sizeof(double));

    MPI_Scatter(data, local_N, MPI_DOUBLE, local_data, local_N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for(int i = 0; i < local_N; i++) {
        local_data[i] = sqrt(local_data[i]);
    }

    MPI_Gather(local_data, local_N, MPI_DOUBLE, data, local_N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if(rank == 0) {
        printf("Radical vector: ");
        for (int i = 0; i < N; i++)
        {
            printf("%.2f ", data[i]);
        }
        printf("\n");
        free(data);
    }

    free(local_data);
    MPI_Finalize();

    return 0;
}