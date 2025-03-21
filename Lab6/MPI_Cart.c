#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int dims[2] = {4, 4};
    int periods[2] = {1, 1};
    MPI_Comm cart_comm;

    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &cart_comm);

    int coords[2]; 
    int cart_rank;
    MPI_Comm_rank(cart_comm, &cart_rank);
    MPI_Cart_coords(cart_comm, cart_rank, 2, coords);

    // Calculate the neighboor
    int n, s, e, w;
    MPI_Cart_shift(cart_comm, 0, 1, &n, &s);
    MPI_Cart_shift(cart_comm, 1, 1, &w, &e);
    double avg = (cart_rank + n + s + e + w) / 5.0;

    for (int i = 0; i < size; i++) {
        MPI_Barrier(MPI_COMM_WORLD);
        if (rank == i) {
            printf("Rank %2d, Coords: (%d,%d), N: %2d, S: %2d, E: %2d, W: %2d, Avg: %.2f\n",
                   cart_rank, coords[0], coords[1], n, s, e, w, avg);
        }
    }

    MPI_Finalize();
    return 0;
}
