#include <iostream>
#include <mpi/mpi.h>
#include <stdlib.h>
using namespace std;
#define AMOUNT 5

int main(int ac,char **av) {
    //Variable
    int size,rank,sizeBuf, trash, *a,*b, *a2, *b2, *sendCounts, *indexes;

    MPI_Init(&ac, &av);
    srand(time(0));
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    sizeBuf = AMOUNT/size;
    trash = AMOUNT%size;
    sendCounts = new int[size];
    indexes = new int[size];
    a2 = new int[(sizeBuf+1)* AMOUNT];
    b2 = new int[(sizeBuf+1)* AMOUNT];

    if(rank == 0) {
        a = new int[AMOUNT* AMOUNT];
        b = new int[AMOUNT* AMOUNT];
        for (int i = 0; i < AMOUNT; i++) {
            for (int j = 0; j < AMOUNT; ++j) {
                a[i* AMOUNT + j] = rand()%10 - 5;
                printf("a[%d][%d] = %d\t", i,j, a[i* AMOUNT + j]);
            }
            cout<<endl;
        }
        cout<<endl;
        for (int i = 0; i < AMOUNT; i++) {
            for (int j = 0; j < AMOUNT; ++j) {
                b[i* AMOUNT + j] = rand()%10 - 5;
                printf("b[%d][%d] = %d\t", i,j, b[i* AMOUNT + j]);
            }
            cout<<endl;
        }
        cout<<endl;
    }

    for(int i = 0; i < size; i++, trash--){
        int index = (trash > 0) ?AMOUNT% size - trash + i * sizeBuf :AMOUNT% size + i * sizeBuf;
        int count = (trash > 0) ? sizeBuf + 1 : sizeBuf;
        sendCounts[i] = count * AMOUNT;
        indexes[i] = index* AMOUNT;
    }

    MPI_Scatterv(a, sendCounts, indexes, MPI_INT, a2, sendCounts[rank], MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatterv(b, sendCounts, indexes, MPI_INT, b2, sendCounts[rank], MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < sendCounts[rank]/ AMOUNT; ++i) {
        for (int j = 0; j < AMOUNT; ++j){
            a2[i* AMOUNT + j] = a2[i* AMOUNT + j] * b2[i* AMOUNT + j];
        }
    }
    MPI_Gatherv(a2, sendCounts[rank], MPI_INT, a, sendCounts, indexes, MPI_INT, 0, MPI_COMM_WORLD);

    if(rank == 0){
        for (int i = 0; i < AMOUNT; i++) {
            for (int j = 0; j < AMOUNT; ++j) {
                printf("c[%d][%d] = %d\t", i,j, a[i* AMOUNT + j]);
            }
            cout<<endl;
        }
        delete []a,b;
    }
    delete []sendCounts,indexes,a2,b2;
    MPI_Finalize();
#undef AMOUNT
}