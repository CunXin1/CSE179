
#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, x;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        x = 10000;
        printf("Rank 0: x = %d\n", x);
    }
    x = x * x + 114514;

    double start = MPI_Wtime();
    MPI_Bcast(&x, 1, MPI_INT, 0, MPI_COMM_WORLD);
    double end = MPI_Wtime();

    printf("Rank %d: x = %d, time = %f seconds\n", rank, x, end - start);

    MPI_Finalize();
    return 0;
}
