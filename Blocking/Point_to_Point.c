#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int rank;

    //initialize MPI
    MPI_Init(&argc,&argv);

    //get rank of process
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // MPI_COMM_WORLD is common communicator

    if(rank == 0) {
        int data = 42;
        MPI_Send(&data,1,MPI_INT,1,0,MPI_COMM_WORLD); // send data to process 1
    }

    if(rank == 1) {
        int recv_data;
        MPI_Recv(&recv_data,1,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        printf("message received from master by process %d with value: %d\n",rank,recv_data);
    }

    MPI_Finalize(); // end of MPI program

    return 0;
}