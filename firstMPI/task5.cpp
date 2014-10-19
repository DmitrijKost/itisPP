#include <mpi/mpi.h>
#include <iostream>
#include <stdlib.h>
using namespace std;
int main(int ac, char** av){
    int size,rank,sizeBuf,*a;
    MPI_Init (&ac, &av);
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);
    MPI_Comm_size (MPI_COMM_WORLD, &size);
    if(rank == 0){
        int amount = 12;
        sizeBuf = amount/size;
        int trash = amount%size;
        a = new int[amount];
        cout<<"Начальный массив:\t";
        for(int i=0; i<amount; i++){
            a[i] = rand()%100-50;
            cout<<a[i]<<"\t ";
        }
        cout<<endl;
        printf("%d - sizeBuf\n%d - trash\n", sizeBuf,trash);
        for(int i = 1; i<size; i++) {
            MPI_Send(&a[(trash > 0)?amount%size - trash+i*sizeBuf:amount%size+i*sizeBuf],(trash-- > 0)?sizeBuf + 1:sizeBuf, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        for(int i=0; i<sizeBuf; i++){
            printf("Процесс %d элемент %d: %d\n",rank,i,a[i]);
        }
    } else {
        MPI_Status status;
        MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &sizeBuf);
        a = new int[sizeBuf];
        MPI_Recv(a, sizeBuf, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for(int i=0; i<sizeBuf; i++){
            printf("Процесс %d элемент %d: %d\n",rank,i,a[i]);
        }
    }
    delete []a;
    MPI_Finalize();
    return 0;
}