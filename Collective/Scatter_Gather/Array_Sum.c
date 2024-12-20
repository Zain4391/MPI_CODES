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
    }

    MPI_Bcast(&N, 1, MPI_INT, 0, GLOBAL);

    int local_N = N / size;
    int *local_data = (int*) malloc (local_N * sizeof(int));

    //Scatter the array
    MPI_Scatter(data,local_N,MPI_INT,local_data,local_N,MPI_INT,0,GLOBAL);

    int local_sum = 0;
    for(int i = 0; i < local_N; i++) {
        local_sum += local_data[i];
    }

    int global_sum;
    MPI_Reduce(&local_sum,&global_sum,1,MPI_INT,MPI_SUM,0,GLOBAL);

    if(rank == 0) {
        printf("Computed Sum: %d",global_sum);
    }

    MPI_Finalize();
    return 0;
}