Compile:
mpicc -fopenmp -O2 finalCGOL.c -o finalCGOL.x

-------------------------------------------------------

Run:
- 4 Threads ,  4 Processes,   8 Gen:
    OMP_NUM_THREADS=4 mpirun -np 4 ./finalCGOL.x 6


- General with default:
    mpirun -np 4 ./finalCGOL.x
