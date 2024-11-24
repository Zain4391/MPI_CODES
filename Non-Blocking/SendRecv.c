#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
    int rank, size;
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int send_data = rank; // Each process sends its rank
    int recv_data;        // To store received data

    MPI_Request send_request, recv_request;

    // Non-blocking send to the next process (circular)
    MPI_Isend(&send_data, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD, &send_request);

    // Non-blocking receive from the previous process (circular)
    MPI_Irecv(&recv_data, 1, MPI_INT, (rank - 1 + size) % size, 0, MPI_COMM_WORLD, &recv_request);

    printf("Process %d: Sent data = %d\n", rank, send_data);

    // Wait for both operations to complete
    MPI_Wait(&send_request, MPI_STATUS_IGNORE);
    MPI_Wait(&recv_request, MPI_STATUS_IGNORE);

    // After receiving the message
    printf("Process %d: Received data = %d\n", rank, recv_data);

    MPI_Finalize();
    return 0;
}
