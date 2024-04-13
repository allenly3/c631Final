Compile:
mpicc -fopenmp -O2 finalCGOL.c -o finalCGOL.x

-------------------------------------------------------

Run: 
- 4 Threads ,  4 Processes,  4 Gen:<br>
    OMP_NUM_THREADS=4 mpirun -np 4 ./finalCGOL.x 4


- General with default:<br>
    mpirun -np 4 ./finalCGOL.x
