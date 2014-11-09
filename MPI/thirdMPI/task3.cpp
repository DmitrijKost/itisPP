#include <iostream>
#include <mpi/mpi.h>
#include <stdlib.h>
using namespace std;
#define AMOUNT 10

int main(int ac,char **av) {
    //Variable
    int size,rank,sizeBuf, trash,  *sendCounts, *indexes;
    struct MINLOC_DATA{
        double value;
        int index;
    };
    MPI_Init(&ac, &av);
    srand(time(0));
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    sizeBuf = AMOUNT/size;
    trash = AMOUNT%size;
    sendCounts = new int[size];
    indexes = new int[size];
    MINLOC_DATA *x,*rBuf;
    rBuf = new MINLOC_DATA[sizeBuf+1];
    if(rank == 0) {
        x = new MINLOC_DATA[AMOUNT];
        for (int i = 0; i < AMOUNT; i++) {
            x[i].value = rand() % 1000 / 100.0 - 5;
            x[i].index = i;
            printf("x[%d] = %f\n", i, x[i].value);
        }
    }

    for(int i = 0; i < size; i++, trash--){
        int index = (trash > 0) ?AMOUNT% size - trash + i * sizeBuf :AMOUNT% size + i * sizeBuf;
        int count = (trash > 0) ? sizeBuf + 1 : sizeBuf;
        sendCounts[i] = count;
        indexes[i] = index;
    }

    MPI_Scatterv(x, sendCounts, indexes, MPI_DOUBLE_INT, rBuf, sendCounts[rank], MPI_DOUBLE_INT, 0, MPI_COMM_WORLD);
    MPI_Reduce(rBuf, x, sizeBuf+1, MPI_DOUBLE_INT, MPI_MINLOC, 0, MPI_COMM_WORLD);

    if(rank == 0){
        MINLOC_DATA min;
        min.value = 5.0;
        for(int i = 0; i<sendCounts[rank]; i++){
            if(min.value > x[i].value){
                min = x[i];
            }
        };
        printf("Min=%f\nIndex=%d\n",min.value,min.index);
    }
    MPI_Finalize();
#undef AMOUNT
}