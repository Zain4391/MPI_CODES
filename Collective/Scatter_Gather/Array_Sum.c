#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

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
        for(int i = 0; i < N; i++) {
            data[i] = rand() % 101;
        }

        printf("Original Vector:\n");
        for(int i = 0; i < N; i++) {
            printf("%d ", data[i]);
        }
        printf("\n");
    }

    int local_N = N / size;

    MPI_Finalize();
    return 0;
}