#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    const int MAX = 8;
    int rank, size;
    int *arr = (int*) malloc(MAX * sizeof(int));

    for(int i = 0; i < MAX; i++) {
        arr[i] = i+1;
    }

    MPI_Init(&argc,&argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    //portion size for each process
    int portion_size = MAX / size;

    //array for each process
    int *portion = (int*) malloc(portion_size * sizeof(int));

    if(rank == 0) {
        for(int i = 1; i < size; i++) {
            MPI_Send(&arr[i * portion_size], portion_size, MPI_INT, i, 0, MPI_COMM_WORLD);
        }

        // master's portion
        for(int i = 0; i < portion_size; i++) {
            portion[i] = arr[i];
        }
    } else {
        // receive the portion
        MPI_Recv(portion, portion_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    int partial_sum = 0;
    for(int i = 0; i < portion_size; i++) {
        partial_sum += portion[i];
    }

    if(rank == 0) {
        int total_sum = partial_sum;
        for(int i = 1; i < size; i++) {
            int worker_sum;
            MPI_Recv(&worker_sum,1,MPI_INT,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            total_sum += worker_sum;
        }

        printf("Master computed total sum: %d\n",total_sum);
    } else {
        // workers send their sums to master
        MPI_Send(&partial_sum,1,MPI_INT,0,0,MPI_COMM_WORLD);
        printf("Process %d: Computed partial sum = %d\n", rank, partial_sum);
    }

    MPI_Finalize();
    free(arr);
    free(portion);
    return 0;
}