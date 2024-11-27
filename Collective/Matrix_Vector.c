#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define GLOBAL MPI_COMM_WORLD

int main(int argc, char** argv) {
    double *A, *x, *y;
    
    //local variables
    double *local_a, *local_y;

    int rank,size, N = 4; //can be dynamic as well

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(GLOBAL, &rank);
    MPI_Comm_size(GLOBAL, &size);

    //Master process allocates memory and initializes the global 2D array A and vector x
    if(rank == 0) {
        x = (double*) malloc (N * sizeof(double));
        A = (double*) malloc (N * N * sizeof(double));

        for(int i = 0; i < N; i++) {
            x[i] = i  +1;
            for(int j = 0; j < N; j++) {
                A[i * N + j] = i * N + j + 1;
            }
        }

        printf("Matrix A:\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                printf("%.1f ", A[i * N + j]);
            }
            printf("\n");
        }
        printf("Vector x:\n");
        for (int i = 0; i < N; i++) {
            printf("%.1f ", x[i]);
        }
        printf("\n");
    }

    int rows_per_proc = N / size; //assume equally divisible

    //allocate memory for local variables
    local_a = (double*) malloc (rows_per_proc * N * sizeof(double));
    local_y = (double*) malloc (rows_per_proc * sizeof(double));

    //allocate vector on all processes
    if (rank != 0) {
        x = (double*)malloc(N * sizeof(double));
    }

    //scatter the Matrix A to all processes
    MPI_Scatter(A, rows_per_proc * N, MPI_DOUBLE, local_a, rows_per_proc * N, MPI_DOUBLE, 0, GLOBAL);

    //broadcast vector x to all processes
    MPI_Bcast(x, N, MPI_DOUBLE, 0, GLOBAL);
    
    //compute local sum
    for(int i = 0; i < rows_per_proc; i++) {
        local_y[i] = 0.0;
        for (int j = 0; j < N; j++)
        {
            local_y[i] += local_a[i * N + j] * x[j];
        }
    }

    if(rank == 0) {
        y = (double*) malloc (N * sizeof(double));
    }

    MPI_Gather(local_y, rows_per_proc, MPI_DOUBLE, y, rows_per_proc, MPI_DOUBLE, 0, GLOBAL);

    //print the result
    if (rank == 0) {
        printf("Result vector y:\n");
        for (int i = 0; i < N; i++) {
            printf("%.1f ", y[i]);
        }
        printf("\n");
    }

    if(rank == 0) {
        free(A);
        free(y);
    }

    free(local_a);
    free(local_y);
    free(x);

    MPI_Finalize();

    return 0;
}