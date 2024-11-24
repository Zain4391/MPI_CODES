#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int rank, size;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if(rank == 0) {
        int n1,n2;
        n1 = 0;
        n2 = 1;
        MPI_Send(&n1,1,MPI_INT,1,0,MPI_COMM_WORLD);
        MPI_Send(&n2,1,MPI_INT,1,0,MPI_COMM_WORLD);

        printf("Master generated %d and %d\n", n1, n2);

        int recv;
        MPI_Recv(&recv, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Master received the final computed number: %d\n", recv);

    } else {
        int first,last;
        MPI_Recv(&first,1,MPI_INT,rank - 1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        MPI_Recv(&last,1,MPI_INT,rank - 1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        int next = first + last;

        printf("process %d received %d and %d, computed %d\n",rank,first,last,next);

        if(rank < size - 1) {
            MPI_Send(&last,1,MPI_INT,rank + 1,0,MPI_COMM_WORLD);
            MPI_Send(&next,1,MPI_INT,rank + 1,0,MPI_COMM_WORLD);
        } else {
            MPI_Send(&next, 1, MPI_INT, 0, 0, MPI_COMM_WORLD); // send last computed one to master
        }
    }


    MPI_Finalize();

}