#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define ROWS 2
#define COLS 6

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 3) {
        printf("This program requires exactly 3 processes.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    MPI_Datatype column_mpi_t;
    MPI_Type_vector(2, 2, COLS, MPI_INT, &column_mpi_t);
    MPI_Type_commit(&column_mpi_t);

    int root = 0;
    int sendbuf[ROWS][COLS] = {
        {1, 2, 3, 4, 5, 6},
        {7, 7, 8, 8, 9, 9}
    };

    int recvbuf[2][2];

    // Scatter the data from the root process to all other processes
    MPI_Scatter(sendbuf, 4, MPI_INT, recvbuf, 4, MPI_INT, root, MPI_COMM_WORLD);

    // Print out the received data on each process
    printf("Process %d received:\n", rank);
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < 2; j++) {
            printf("%d ", recvbuf[i][j]);
        }
        printf("\n");
    }

    MPI_Type_free(&column_mpi_t);
    MPI_Finalize();
    return 0;
}
