#include <iostream>
#include <mpi/mpi.h>
#include <stdlib.h>
using namespace std;
#define AMOUNT 10

int main(int ac,char **av) {
    //Variable
    int size,rank,sizeBuf, trash, *requestBuf,*x, *sendCounts, *indexes;

    MPI_Init(&ac, &av);
    srand(time(0));
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    sizeBuf = AMOUNT/size;
    trash = AMOUNT%size;
    sendCounts = new int[size];
    indexes = new int[size];
    requestBuf = new int[(sizeBuf+1)* AMOUNT];

    if(rank == 0) {
        x = (int*)malloc(AMOUNT* AMOUNT*sizeof(int));
        for (int i = 0; i < AMOUNT; i++) {
            for (int j = 0; j < AMOUNT; ++j) {
                x[i* AMOUNT + j] = rand()%10 - 5;
                printf("x[%d][%d] = %d\t", i,j, x[i* AMOUNT + j]);
            }
            cout<<endl;
        }
    }

    for(int i = 0; i < size; i++, trash--){
        int index = (trash > 0) ?AMOUNT% size - trash + i * sizeBuf :AMOUNT% size + i * sizeBuf;
        int count = (trash > 0) ? sizeBuf + 1 : sizeBuf;
        sendCounts[i] = count * AMOUNT;
        indexes[i] = index* AMOUNT;
    }

    MPI_Scatterv(x, sendCounts, indexes, MPI_INT, requestBuf, sendCounts[rank], MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < sendCounts[rank]/ AMOUNT; ++i) {
        int sum = 0;
        for (int j = 0; j < AMOUNT; ++j){
            sum+= abs(requestBuf[AMOUNT*i + j]);
        }
        requestBuf[i] = sum;
    }

    MPI_Reduce(requestBuf, x, (sizeBuf+1), MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    if(rank == 0){
        int max = 0;
        for(int i = 0; i<sendCounts[rank]/ AMOUNT;i++){
            if(max < x[i]){
                max = x[i];
            }
        };
        printf("MAX=%d\n",max);
        delete []x;
    }
    delete []sendCounts,indexes,requestBuf;
    MPI_Finalize();
#undef AMOUNT
}