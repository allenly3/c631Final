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
#include "stdbool.h"


#define N 4

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


/*
0: dead
1: live
Four Rules:
Any live cell with fewer than two live neighbors dies, as if by underpopulation.
Any live cell with two or three live neighbors lives on to the next generation.
Any live cell with more than three live neighbors dies, as if by overpopulation.
Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.
*/
void statusUpdate(int arr[N+2][N+2] , int storeArr[N+2][N+2] ,  int subN, bool p0) {

    printf(">>>>>>>>>>>>>>> Status Updating <<<<<<<<<<<<<<<\n");

    int i , j;
    int liveCounter = 0;
    int row ;

    if(p0){
      row = N + 1;
    }else{
      row = subN + 1;
    }

    for (i = 1; i < row; i++){
      for (j = 1; j < subN+1; j++){

        liveCounter =   arr[i-1][j-1] + arr[i-1][j] + arr[i-1][j+1] + 
                        arr[i][j-1] + arr[i][j+1] + 
                        arr[i+1][j-1] + arr[i+1][j] + arr[i+1][j+1];

        printf("%d", liveCounter);

        storeArr[i][j] = arr[i][j];
        if(arr[i][j] == 1){ // This dot is alive

            if(liveCounter < 2){
              storeArr[i][j] = 0 ;
            } else if (liveCounter == 2 || liveCounter ==3){
              storeArr[i][j] = 1 ;
            } else if ( liveCounter > 3){
              storeArr[i][j] = 0 ;
            }

        
        }else if(arr[i][j] == 0){ // This dot is dead

          if ( liveCounter == 3){
            storeArr[i][j] = 1;
          }

        }
        printf(":%d  ", storeArr[i][j]);
      }
      printf("\n");
    }

  printf(">>>>>>>>>>>>>>> Update Done <<<<<<<<<<<<<<<\n");
    for (i = 0; i < row + 1; i++) {
      for (j = 0; j < subN+2; j++){
        printf("%d ", storeArr[i][j]);
      }
      printf("\n");
    }
    printf("\n");
    //printf("*****************************\n");



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
  int S_U [N+2][N+2]; // S matrix  updated.
  MPI_Status status;
  MPI_Datatype column_mpi_t, column_mpi_origin;
  int i, j, k, q, counter, row , col;
  int Generation = 1; // Number of Generation we want to see
  int g; // for iteration of Generation
  int req_size; // non-block req counter
  int subSize; 
  counter = 10;




  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  //for sending back by non-blocking communication
  MPI_Request *reqs = (MPI_Request *)malloc(size * sizeof(MPI_Request)); 

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

  //square submatrix
  MPI_Type_vector(subSize+2, subSize+2, N+2, MPI_INT, &column_mpi_t);
  MPI_Type_commit(&column_mpi_t);

  //send back to P0 
  MPI_Type_vector(N+2, subSize, N+2, MPI_INT, &column_mpi_origin);
  MPI_Type_commit(&column_mpi_origin);

  
  srand(time(NULL));


  for (i = 0; i < N+2; i++){
    for (j = 0; j < N+2; j++){ 
      A[i][j] =  rand() % 2;  // random 0 or 1 to A
      S[i][j] = 9; // init S using 9, its better to check result 
    }
  }

  for (g = 0; g < Generation; g ++){
  // ----------------------------------P0-------------------------
    if (my_rank == 0) { 

      if(g > 0 ){  // If its not initial generation, use the S matrix which get from last round
        for (i = 0; i < N+2; i++){
          for (j = 0; j < N+2; j++){
            A[i][j] = S[i][j];
          }
        }
      }


      toPeriodic(A,N); 

      for (i = 0; i < N+2; i++){
        for (j = 0; j < N+2; j++){
            printf("%d ", A[i][j]);
        }
        printf("\n");
      }

      printf("************ Full matrix Periodic Done ************\n\n");      
    
    

      //  Update live and dead status

      statusUpdate(A,S,subSize, true);   // Update p0 directly

      // Sending submatrix
      for (i = 0 ; i < size ; i ++){
        for(j = 0; j < size; j ++){
            if(j == 0 ){

              continue; //  Skip p0 
            }
            MPI_Send(&(A[i*subSize][j*subSize]), 1, column_mpi_t, j, 0, MPI_COMM_WORLD);

        }
      }

      req_size = 0 ;
      for (i = 0 ; i < size; i++){

          if(i == 0){   //  P0 already update status itself
            continue;
          }

          MPI_Irecv(&S[0][i*subSize+1], 1, column_mpi_origin, i, 2, MPI_COMM_WORLD, &reqs[req_size++]);
          
      }
      MPI_Waitall(req_size, reqs, MPI_STATUSES_IGNORE);

      //After recieve from other processes, check S 
      //The outer layer of the matrix is used for periodicity
      //so the for loop indices i and j start from 1 and end at N + 1.
      printf("\n ########## P0 recieved All.  GEN %d Result: ##########\n", (g+1));

      for (i = 1; i < N+1; i++) {
        for (j = 1; j < N+1; j++){
          printf("%d ", S[i][j]);
        }
        printf("\n");
      }
      printf("\n");

    
    // ----------------------------------Other Processes-------------------------
    } else { 
        for (i = 0; i < N+2; i++){
          for (j = 0; j < N+2; j++){
            T[i][j] = 0;
            S_U[i][j] = 0;
          }
        }
            
        

        for( k =0; k < size; k++){
          MPI_Recv(&(T[0][0]), 1, column_mpi_t, 0, 0,MPI_COMM_WORLD, &status);

          usleep(100000 * (my_rank+1));
          for (i = 0; i < subSize+2; i++) {
            for (j = 0; j < subSize+2; j++){
              printf("%d ", T[i][j]);
            }
            printf("\n");
          }
          printf("\n");

          statusUpdate(T,S,subSize, false);  // sub matrix update
          
          // after update status, pass value to S_U without periodic values
          for (i = 1; i < subSize+1; i++) {
            for (j = 1; j < subSize+1; j++){

              // need k here, k is the row 
              //printf("%d ", S[i][j]);
              //printf("[%d][%d],", k*subSize +i,j);

              S_U[k*subSize + i][j] = S[i][j];
            }
            //printf("\n");
          }


          printf("\n");
        }

        
        printf("P%d is sending: \n", my_rank);
        // Check S_U
        for (i = 0; i < N+2; i++) {
          for (j = 0; j < subSize+2; j++){

            // need k here, k is the row 
            printf("%d ", S_U[i][j]);
          }
          printf("\n");
        }

        //send submatrix back to P0's  S

        req_size = 0;
        MPI_Isend(&S_U[0][1], 1, column_mpi_origin, 0, 2, MPI_COMM_WORLD, &reqs[req_size++]);
        MPI_Waitall(req_size, reqs, MPI_STATUSES_IGNORE);
    }
  }

  MPI_Finalize();
  return 0 ; 
}  /* main */
	


