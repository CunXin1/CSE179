#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int num, rank, message;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double start = MPI_Wtime();

    int prev = (rank - 1 + num) % num;
    int next = (rank + 1) % num;

    if (rank == 0) {

        message = 114514;
        MPI_Send(&message, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
        printf("Process %d sent message %d to process %d\n", rank, message, next);

        MPI_Recv(&message, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, &status);
        printf("Process %d received final message %d from process %d\n", rank, message, prev);
    } else {
        MPI_Recv(&message, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, &status);
        printf("Process %d received message %d from process %d\n", rank, message, prev);

        message += 1;

        MPI_Send(&message, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
        printf("Process %d sent message %d to process %d\n", rank, message, next);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    
    if (rank == 0) {
        double end = MPI_Wtime();
        printf("Execution Time: %f seconds\n", end - start);
    }

    MPI_Finalize();
    return 0;
}
