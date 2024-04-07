/*
    Read Me
  - Only 0 and 1, so use INT array
  - 0 stands for dead, 1 stands for alive
  - N for matrix edge size
  - We assume that  N is divisible by p ( the number of process)  
  - p also is a square number
*/
#include "stdio.h"
#include "stdlib.h"
#include "mpi.h"
#include "time.h"
#define N 4

int main(int argc, char* argv[]) {


    // Print the first command-line argument if it exists
    if (argc > 1) {
        printf("First command-line argument: %s\n", argv[1]);
    } else {
        printf("No command-line arguments provided.\n");
    }


 
    return 0 ; 


}