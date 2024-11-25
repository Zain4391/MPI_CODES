#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char**argv) {
    int rank,size,N;
    MPI_Init(&argc,&argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int *data = NULL;

    //master takes input
    if(rank == 0) {
        printf("Enter size of vector: ");
        scanf("%d", &N);
        data = (int*)malloc(N * sizeof(int));

        for(int i = 0; i < N; i++) {
            data[i] = rand() % 101;
        }

        printf("Array being broadcasted:\n");
        for(int i = 0; i < N; i++) {
            printf("%d ", data[i]);
        }
        printf("\n");
    }

    //first tell processes the maount of data being send
    MPI_Bcast(&N,1,MPI_INT,0,MPI_COMM_WORLD);

    //allocate memory to othe rprocesses
    if(rank != 0) {
        data = (int*) malloc (N * sizeof(int));
    }

    //now broadcast the entire array
    MPI_Bcast(data,N,MPI_INT,0,MPI_COMM_WORLD);

    //debugging step
    printf("process %d received array: ", rank);
    for(int i = 0; i < N; i++) {
        printf("%d ", data[i]);
    }
    printf("\n");

    int chunk_size = N / size;
    int s = rank * chunk_size;
    int e = (rank == size - 1) ? N : s + chunk_size;

    int local_sum = 0;
    for(int i = s; i < e; i++) {
        local_sum += data[i];
    }

    int global_sum;
    MPI_Reduce(&local_sum,&global_sum,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);

    if(rank == 0) {
        double avg = (double) global_sum / N;
        printf("Average of array = %.2f\n", avg);
    }

    MPI_Finalize();
    free(data);

    return 0;

}