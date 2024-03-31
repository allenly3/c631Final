#include "stdio.h"
#include "stdlib.h"
#include "mpi.h"
#include "time.h"
#define N 16

int main(int argc, char* argv[]) {
    int p;
    int my_rank;


    // Only 0 and 1, so use int

    // To be periodic, originally
    /// we generate an N+2 x N+2 matrix

    int A[N+2][N+2];
    int T[N+2][N+2];
    MPI_Status status;
    MPI_Datatype column_mpi_t;
    int i, j;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);


    // 2x2 matrix, distance: N
    MPI_Type_vector(4, 4, N+1, MPI_INT, &column_mpi_t);
    MPI_Type_commit(&column_mpi_t);
    srand(time(NULL));

    if (my_rank == 0) {

      for (i = 0; i < N+2; i++){
        for (j = 0; j < N+2; j++){
          // The edge value init to 8
          if(i == 0 || j == 0 || i == N+1 || j == N+1){
            A[i][j] = 8;
          }else{
            A[i][j] =  A[i][j] =  rand() % 2;;
          }   
        }
      }

      for (i = 0; i < N+2; i++){
        for (j = 0; j < N+2; j++){
           printf("%d ", A[i][j]);
        }
        printf("\n");
      }
            
        //MPI_Send(&(A[0][4]), 1, column_mpi_t, 1, 0,MPI_COMM_WORLD);
    
    } else { 
        // for (i = 0; i < N; i++)
        //     for (j = 0; j < N; j++)
        //         T[i][j] = 0;

        // MPI_Recv(&(T[0][4]), 1, column_mpi_t, 0, 0,MPI_COMM_WORLD, &status);
        // for (i = 0; i < N; i++) {
        //   for (j = 0; j < N; j++){
        //     printf("%d ", T[i][j]);
        //   }
        //   printf("\n");
        // }
        // printf("\n");
    }

    MPI_Finalize();
    return 0 ; 
}  /* main */
	