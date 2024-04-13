Compile:
mpicc -fopenmp -O2 finalCGOL.c -o finalCGOL.x

-------------------------------------------------------

Run: 
- 4 Threads ,  4 Processes,   2 Gen:\n
    OMP_NUM_THREADS=4 mpirun -np 2 ./finalCGOL.x 4


- General with default:\n
    mpirun -np 4 ./finalCGOL.x
