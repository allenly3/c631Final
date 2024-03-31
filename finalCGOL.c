#include "stdio.h"
#include "mpi.h"
#define N 16

int main(int argc, char* argv[]) {
    int p;
    int my_rank;
    // Only 0 and 1, so use int
    int A[N][N];
    int T[N][N];
    MPI_Status status;
    MPI_Datatype column_mpi_t;
    int i, j;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);


    // 2x2 matrix, distance: N
    MPI_Type_vector(2, 2, N, MPI_INT, &column_mpi_t);
    MPI_Type_commit(&column_mpi_t);

    if (my_rank == 0) {
        for (i = 0; i < N; i++)
            for (j = 0; j < N; j++)
                A[i][j] =  j;
        MPI_Send(&(A[0][2]), 1, column_mpi_t, 1, 0,MPI_COMM_WORLD);
    } else { 
        for (i = 0; i < N; i++)
            for (j = 0; j < N; j++)
                T[i][j] = 0;

        MPI_Recv(&(T[0][2]), 1, column_mpi_t, 0, 0,MPI_COMM_WORLD, &status);
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++)
                printf("%d ", T[i][j]);
            printf("\n");
        }
        printf("\n");
    }

    MPI_Finalize();
    return 0 ; 
}  /* main */
	