#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define GLOBAL MPI_COMM_WORLD

int main(int argc, char**argv) {
    int rank,size,N;
    MPI_Init(&argc,&argv);
    int *data;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(rank == 0) {
        printf("Enter size of vector: ");
        scanf("%d", &N);

        data = (int*) malloc (N * sizeof(int));
        srand(time(NULL) + rank);
        for(int i = 0; i < N; i++) {
            data[i] = rand() % 101;
        }

        printf("Original Vector: ");
        for (int i = 0; i < N; i++)
        {
            printf("%d ", data[i]);
        }
        printf("\n");
    }

    MPI_Bcast(&N, 1, MPI_INT, 0, GLOBAL);

    int local_N = N / size;
    int *local_data = (int*) malloc (local_N * sizeof(int));

    MPI_Scatter(data, local_N, MPI_INT, local_data, local_N, MPI_INT, 0 ,GLOBAL);

    for(int i = 0; i < local_N; i++) {
        local_data[i] = local_data[i] * local_data[i];
    }

    MPI_Gather(local_data, local_N, MPI_INT, data, local_N, MPI_INT, 0, GLOBAL);

    if(rank == 0) {
        printf("Square of original vector: ");
        for (int i = 0; i < N; i++)
        {   
            printf("%d ", data[i]);
        }
        printf("\n");
        free(data);
    }

    free(local_data);
    MPI_Finalize();
    return 0;
}