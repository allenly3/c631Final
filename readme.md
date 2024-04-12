Compile:
gcc -fopenmp -O2 finalCGOL.c -o finalCGOL.x -lmpi

-------------------------------------------------------

Run:

default number of generation is 1

1. Using default number of generation,  default number of threads:

    mpirun -np 2 ./finalCGOL.x 

2. Using default number of Generation:

    OMP_NUM_THREADS=4 mpirun -np 2 ./finalCGOL.x 

3. 4 Generations:

    mpirun -np 2 ./finalCGOL.x 4