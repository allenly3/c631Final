/*
    Read Me
  - Only 0 and 1, so use INT array
  - 0 stands for dead, 1 stands for alive
  - N for matrix edge size
  - We assume that  N is divisible by p 
  - Ansume that size  = p * p  
*/
#include "stdio.h"
#include "stdlib.h"
#include "mpi.h"
#include "time.h"
#include "unistd.h"

#define N 8

void toPeriodic(int arr[N+2][N+2] , int n) {
  int i , j;

    for (i = 1; i < n+1; i++){
      for (j = 1; j < n+1; j++){
 

        // assign edge and corner values
        if(i == 1){
          //First row value to 
          if(j == 1){
            arr[n+1][n+1] = arr[i][j];
          }
          if(j == n){
            arr[n+1][0] = arr[i][j];
          }
          arr[n+1][j] =  arr[i][j];
          
        }

        if( i == n ){

          if(j == 1){
            arr[0][n+1] = arr[i][j];
          }
          if(j == n){
            arr[0][0] = arr[i][j];
          }
          arr[0][j] =  arr[i][j];

        }

        if(j == 1){
          arr[i][n+1] = arr[i][j];
        }

        if(j == n){
          arr[i][0] = arr[i][j];
        }

            
      }
    }
}


int main(int argc, char* argv[]) {

  int size;
  int my_rank;

  // To be periodic
  /*
    P P P P P P
    P 1 0 1 0 P
    P 1 0 1 0 P
    P 0 1 0 1 P
    P P P P P P
  So the array initialized size is N+2
  Same as in submatrix
  */

  int A[N+2][N+2];
  int T[N+2][N+2];
  int S[N+2][N+2];
  MPI_Status status;
  MPI_Datatype column_mpi_t;
  int i, j, k, q, counter, row , col;
  int G = 1; // Number of Generation we want to see
  int subSize; 
  counter = 10;



  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if( N % size != 0 ){
    printf("Program Ends. Number of Process should be a factor of N(%d), size(%d) \n", N, size); 
    MPI_Finalize();
    return 0;
  }

  row = size;
  col = size;
  
  // Split Original matrix A according to size P
  // And create a vector of sending blocks

  subSize = N/size;  // number of element in submatrix

  MPI_Type_vector(subSize+2, subSize+2, N+2, MPI_INT, &column_mpi_t);
  MPI_Type_commit(&column_mpi_t);

  
  srand(time(NULL));


  for (i = 0; i < N+2; i++){
    for (j = 0; j < N+2; j++){ 
      A[i][j] = 99;
    }
  }

  if (my_rank == 0) {

    // Initialize matrix in P0
    for (i = 1; i < N+1; i++){
      for (j = 1; j < N+1; j++){
        //A[i][j] =   rand() % 2;
        A[i][j] =  A[i][j] =  counter;
        counter++;
 
      }
    }

    toPeriodic(A,N); 

    for (i = 0; i < N+2; i++){
      for (j = 0; j < N+2; j++){
          printf("%d ", A[i][j]);
      }
      printf("\n");
    }

    printf("************ Initialization Done ************\n");      
   
    // Sending submatrix

    for (i = 0 ; i < size ; i ++){
      for(j = 0; j < size; j ++){
          if(j == 0 ){
            continue; //  Skip p0 
          }
          MPI_Send(&(A[i*subSize][j*subSize]), 1, column_mpi_t, j, 0, MPI_COMM_WORLD);

      }
    }




  
  
  } else { 
      for (i = 0; i < N+2; i++)
          for (j = 0; j < N+2; j++)
              T[i][j] = 0;
      
      for( k =0; k < size; k++){
        MPI_Recv(&(T[0][0]), 1, column_mpi_t, 0, 0,MPI_COMM_WORLD, &status);
        usleep(100000);
        for (i = 0; i < subSize+2; i++) {
          for (j = 0; j < subSize+2; j++){
            printf("%d ", T[i][j]);
          }
          printf("\n");
        }
        printf("\n");
      }
      

  }

  MPI_Finalize();
  return 0 ; 
}  /* main */
	


