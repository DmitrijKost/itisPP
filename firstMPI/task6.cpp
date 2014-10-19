#include <mpi/mpi.h>
#include <iostream>
#include <stdlib.h>
using namespace std;
int main(int ac, char** av){
    int size,rank,sizeBuf,*a,*b,*z;
    MPI_Init (&ac, &av);
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);
    MPI_Comm_size (MPI_COMM_WORLD, &size);
    if(rank == 0){
        int amount,x,y;
        cout<<"Введите размерность пространства:\n";
        cin>>amount;
        sizeBuf = amount/(size-1);
        int trash = amount%(size-1);
        a = new int[amount];
        b = new int[amount];
        z = new int[amount];
        cout<<"Введите alpha,beta:\n";
        cin>>x>>y;
        cout<<"a\tb\n";
        for(int i=0; i<amount; i++){
            a[i] = rand()%100-50;
            b[i] = rand()%100-50;
            cout<<a[i]<<"\t "<<b[i]<<endl;
            a[i] *= x;
            b[i] *= y;
        }
        for(int i = 1; i<size; i++,trash--) {
            int index = (trash > 0)?amount%(size-1) - trash+(i-1)*sizeBuf:amount%(size-1)+(i-1)*sizeBuf;
            int count = (trash > 0)?sizeBuf + 1:sizeBuf;
            MPI_Send(&a[index],count, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&b[index],count, MPI_INT, i, 1, MPI_COMM_WORLD);
        }
        trash = amount%(size-1);
        for(int i=1; i<size; i++,trash--){
            int index = (trash > 0)?amount%(size-1) - trash+(i-1)*sizeBuf:amount%(size-1)+(i-1)*sizeBuf;
            int count = (trash > 0)?sizeBuf + 1:sizeBuf;
            MPI_Recv(&z[index],count,MPI_INT, i, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        cout<<"Вектор z\n";
        for(int i=0; i<amount; i++){
            cout<<z[i]<<endl;
        }
    } else {
        MPI_Status status;
        MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &sizeBuf);
        a = new int[sizeBuf];
        b = new int[sizeBuf];
        z = new int[sizeBuf];
        MPI_Recv(a, sizeBuf, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(b, sizeBuf, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for(int i=0; i<sizeBuf; i++){
            z[i] = a[i] + b[i];
        }
        MPI_Send(z, sizeBuf, MPI_INT, 0, 2, MPI_COMM_WORLD);
    }
    delete []a,b,z;
    MPI_Finalize();
    return 0;
}