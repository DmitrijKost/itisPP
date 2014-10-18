#include <stdio.h>
#include <mpi/mpi.h>
#include <iostream>

int main(int ac, char** av){
    int size,rank;
    MPI_Init (&ac, &av);
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);
    MPI_Comm_size (MPI_COMM_WORLD, &size);
    printf("rank: %d of %d \n",rank,size);
    MPI_Finalize();
    return 0;
}