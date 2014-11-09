#include <iostream>
#include <mpi/mpi.h>
#include <stdlib.h>
using namespace std;
#define AMOUNT 8

int main(int ac,char **av) {
    //Variable
    int size,rank,sizeBuf, trash, *A2,*A, *sendCounts, *sendCountsSmall, *indexes, *indexesSmall,*x;

    MPI_Init(&ac, &av);
    srand(time(0));
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    sizeBuf = AMOUNT/size;
    trash = AMOUNT%size;
    sendCounts = new int[size];
    indexes = new int[size];
    indexesSmall = new int[size];
    sendCountsSmall = new int[size];
    x = new int[AMOUNT];
    A2 = new int[(sizeBuf+1)*AMOUNT];

    if(rank == 0) {
        A = new int[AMOUNT* AMOUNT];
        for (int i = 0; i < AMOUNT; i++) {
            for (int j = 0; j < AMOUNT; ++j) {
                A[i* AMOUNT + j] = rand()%10 - 5;
                printf("A[%d][%d] = %d\t", i,j, A[i* AMOUNT + j]);
            }
            cout<<endl;
        }
        cout<<endl;
        for (int i = 0; i < AMOUNT; ++i) {
            x[i] = rand()%10 - 5;
            printf("x[%d] = %d\t", i, x[i]);
        }
        cout<<endl<<endl;
    }

    for(int i = 0; i < size; i++, trash--){
        int index = (trash > 0) ?AMOUNT% size - trash + i * sizeBuf :AMOUNT% size + i * sizeBuf;
        int count = (trash > 0) ? sizeBuf + 1 : sizeBuf;
        sendCountsSmall[i] = count;
        sendCounts[i] = count * AMOUNT;
        indexesSmall[i] = index;
        indexes[i] = index * AMOUNT;
    }

    MPI_Scatterv(A, sendCounts, indexes, MPI_INT, A2, sendCounts[rank], MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(x, AMOUNT, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < sendCountsSmall[rank]; ++i) {
        int sum = 0;
        for (int j = 0; j < AMOUNT; ++j){
            sum+= A2[AMOUNT*i + j] * x[j];
        }
        A2[i] = sum;
    }

    MPI_Gatherv(A2, sendCountsSmall[rank], MPI_INT, x, sendCountsSmall, indexesSmall, MPI_INT, 0, MPI_COMM_WORLD);

    if(rank == 0){
        for(int i = 0; i<AMOUNT;i++){
            printf("z[%d] = %d\t",i,x[i]);
        };
        delete []A;
        cout<<endl;
    }
    delete []sendCounts,sendCountsSmall,indexes,indexesSmall,A2,x;
    MPI_Finalize();
#undef AMOUNT
}