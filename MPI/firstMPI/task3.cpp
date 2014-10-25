#include <mpi/mpi.h>
#include <iostream>
#include <stdlib.h>
int main(int ac, char** av){
    int size,rank;
    MPI_Init (&ac, &av);
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);
    if(rank == 0){
        int *a = new int[10];
        for(int i=0; i<10; i++){
            a[i] = rand()%100;
        }
        MPI_Send(a, 10, MPI_INT, 1,0, MPI_COMM_WORLD);
        delete []a;
    }
    if(rank == 1){
        int *a = new int[10];
        MPI_Recv(a, 10, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for(int i=0; i<10; i++){
            std::cout<<a[i]<<std::endl;
        }
        delete []a;
    }
    MPI_Finalize();
    return 0;
}