#include <mpi/mpi.h>
#include <iostream>
#include <stdlib.h>
int main(int ac, char** av){
    int size,rank, sizeBuf;
    MPI_Init (&ac, &av);
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);
    MPI_Comm_size (MPI_COMM_WORLD, &size);
    if(size > 1 && rank == 1){
        sizeBuf = rand()%20;
        int *a = new int[sizeBuf];
        for(int i=0; i<sizeBuf; i++){
            a[i] = rand()%100-50;
        }
        MPI_Send(a, sizeBuf, MPI_INT, 0, 0, MPI_COMM_WORLD);
        for(int i=2; i<size; i++) {
            MPI_Send(a, sizeBuf, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        delete []a;
    } else if(size > 1){
        MPI_Status status;
        MPI_Probe(1, 0, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &sizeBuf);
        int *a = new int[sizeBuf];
        MPI_Recv(a, sizeBuf, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
        for(int i=0; i<sizeBuf; i++){
            printf("Process %d\tvalue = %d\n",rank,a[i]);
        }
        delete []a;
    }
    MPI_Finalize();
    return 0;
}