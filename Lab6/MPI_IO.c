#include <mpi.h>
#include <stdio.h>

#define FILENAME "data.bin"

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_File file;
    MPI_Datatype filetype;
    MPI_Offset offset;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Generate data
    int my_data[10];
    for (int i = 0; i < 10; i++)
        my_data[i] = rank * 10 + i;

    // Only Rank 0 deletes the file
    if (rank == 0) MPI_File_delete(FILENAME, MPI_INFO_NULL);
    MPI_Barrier(MPI_COMM_WORLD);

    // Print data
    printf("Rank %d data:", rank);
    for (int i = 0; i < 10; i++)
    {
        printf(" %d", my_data[i]);
    }
    printf("\n");

    MPI_File_open(MPI_COMM_WORLD, FILENAME, MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &file);

    // Define storage pattern using MPI_Type_vector()
    MPI_Type_vector(5, 2, size * 2, MPI_INT, &filetype);
    MPI_Type_commit(&filetype);
    offset = rank * 2 * sizeof(int);
    MPI_File_set_view(file, offset, MPI_INT, filetype, "native", MPI_INFO_NULL);

    // Perform collective write operation
    MPI_File_write_all(file, my_data, 10, MPI_INT, MPI_STATUS_IGNORE);

    // Close the file
    MPI_File_close(&file);
    MPI_Type_free(&filetype);
    
    MPI_Finalize();
    return 0;
}
