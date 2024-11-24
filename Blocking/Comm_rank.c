#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int rank, size;

    MPI_Init(&argc, &argv);

    //get the no of processes (value of the number of processes will be returned in size)
    MPI_Comm_size(MPI_COMM_WORLD, &size); 

    //get the rank of calling process (rank returned in the 2nd argument)
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    printf("process %d out of %d says hello!\n", rank, size);

    MPI_Finalize();

    return 0;
}