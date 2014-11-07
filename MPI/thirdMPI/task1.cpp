#include <iostream>
#include <mpi/mpi.h>
#include <stdlib.h>
using namespace std;

int main(int ac,char **av) {
    //Variable
    int size,rank,sizeBuf, trash, n, *requestBuf,*x, *sendCounts, *indexes;

    MPI_Init(&ac, &av);
    srand(time(0));
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    n=10;
    sizeBuf = n/size;
    trash = n%size;
    sendCounts = new int[size];
    indexes = new int[size];
    requestBuf = new int[sizeBuf+1];

    if(rank == 0) {
        x = new int[n];
        for (int i = 0; i < n; i++) {
            x[i] = rand() % 10 - 5;
            printf("x[%d] = %d\n", i, x[i]);
        }
    }

    for(int i = 0; i < size; i++, trash--){
        int index = (trash > 0) ? n % size - trash + i * sizeBuf : n % size + i * sizeBuf;
        int count = (trash > 0) ? sizeBuf + 1 : sizeBuf;
        sendCounts[i] = count;
        indexes[i] = index;
    }

    MPI_Scatterv(x, sendCounts, indexes, MPI_INT, requestBuf, sendCounts[rank], MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < sendCounts[rank]; i++){
        requestBuf[i] = abs(requestBuf[i]);
    }

    MPI_Reduce(requestBuf, x, sizeBuf+1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if(rank == 0){
        int sum = 0;
        for(int i = 0; i<sendCounts[rank];  sum += x[i], i++){};
        printf("Sum.=%d\n",sum);
    }
    MPI_Finalize();
}