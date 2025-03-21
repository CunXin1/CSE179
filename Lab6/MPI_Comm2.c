#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    const int chunk_size = 10;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int total_size = chunk_size * size;
    int *data = NULL;

    if (rank == 0) {
        data = malloc(total_size * sizeof(int));
        for (int i = 0; i < total_size; i++) {
            data[i] = i + 1;
        }
        printf("Initial array:\n");
        for (int i = 0; i < total_size; i++) {
            printf("%d ", data[i]);
        }
        printf("\n");
    }

    int *sub = malloc(chunk_size * sizeof(int));

    double start = MPI_Wtime();
    MPI_Scatter(data, chunk_size, MPI_INT, sub, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    //Update Data
    for (int i = 0; i < chunk_size; i++) {
        sub[i] = sub[i] * 1000;
        sub[i] += rank;
    }

    MPI_Gather(sub, chunk_size, MPI_INT, data, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);
    double end = MPI_Wtime();

    if (rank == 0) {
        printf("Final array:\n");
        for (int i = 0; i < total_size; i++) {
            printf("%d ", data[i]);
        }
        printf("\n");
        printf("Time = %f seconds\n", end - start);
        free(data);
    }

    free(sub);
    MPI_Finalize();
    return 0;
}
