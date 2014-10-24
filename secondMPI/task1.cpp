#include <mpi/mpi.h>
#include <iostream>
#include <stdlib.h>
#include <ctime>

using namespace std;
int main(int ac, char** av){
    srand( time(0));
    //TAGS
    int FLAG = 0;
    int ALPHA = 1;
    int BETA = 2;
    int A = 3;
    int B = 4;
    int RES = 5;

    //Variable
    int size,rank,sizeBuf,*a,*b,*z,flag, alpha, beta;
    MPI_Init (&ac, &av);
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);
    MPI_Comm_size (MPI_COMM_WORLD, &size);
    if(rank == 0){
        int amount, trash, index, count;
        cout<<"Введите размер вектора:\n";
        cin>>amount;
        cout<<"Выберите задание(1, 2, 3 или 4):\n";
        cin>>flag;
        if(flag > 0 && flag < 3) {
            cout << "Введите alpha,beta:\n";
            cin >> alpha >> beta;
        }
        sizeBuf = amount/(size-1);
        trash = amount%(size-1);
        a = new int[amount];
        b = new int[amount];
        cout<<"a\tb\n";
        for(int i=0; i<amount; i++){
            a[i] = rand()%100-50;
            b[i] = rand()%100-50;
            cout<<a[i]<<"\t "<<b[i]<<endl;
        }
        for(int i = 1; i<size; i++,trash--) {
            index = (trash > 0)?amount%(size-1) - trash+(i-1)*sizeBuf:amount%(size-1)+(i-1)*sizeBuf;
            count = (trash > 0)?sizeBuf + 1:sizeBuf;
            MPI_Send(&flag,1, MPI_INT, i, FLAG, MPI_COMM_WORLD);
            if(flag > 0 && flag < 3){
                MPI_Send(&alpha,1, MPI_INT, i, ALPHA, MPI_COMM_WORLD);
                MPI_Send(&beta,1, MPI_INT, i, BETA, MPI_COMM_WORLD);
            }
            MPI_Send(&a[index],count, MPI_INT, i, A, MPI_COMM_WORLD);
            MPI_Send(&b[index],count, MPI_INT, i, B, MPI_COMM_WORLD);
        }
        trash = amount%(size-1);
        switch(flag){
            case 1:
            z = new int[amount];
            for(int i=1; i<size; i++,trash--) {
                int index = (trash > 0) ? amount % (size - 1) - trash + (i - 1) * sizeBuf : amount % (size - 1) + (i - 1) * sizeBuf;
                int count = (trash > 0) ? sizeBuf + 1 : sizeBuf;
                MPI_Recv(&z[index], count, MPI_INT, i, RES, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            };
            cout << "Вектор z\n";
            for(int i=0; i<amount; i++){
                cout<<z[i]<<endl;
            };break;
            case 2:
            for(int i=1; i<size; i++,trash--) {
                index = (trash > 0) ? amount % (size - 1) - trash + (i - 1) * sizeBuf : amount % (size - 1) + (i - 1) * sizeBuf;
                count = (trash > 0) ? sizeBuf + 1 : sizeBuf;
                MPI_Recv(&b[index], count, MPI_INT, i, RES, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            };
            cout << "Вектор y\n";
            for(int i=0; i<amount; i++){
                cout<<b[i]<<endl;
            };break;
            case 3:
            z = new int[amount];
            for(int i=1; i<size; i++,trash--) {
                int index = (trash > 0) ? amount % (size - 1) - trash + (i - 1) * sizeBuf : amount % (size - 1) + (i - 1) * sizeBuf;
                int count = (trash > 0) ? sizeBuf + 1 : sizeBuf;
                MPI_Recv(&z[index], count, MPI_INT, i, RES, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            };
            cout << "Вектор z\n";
            for(int i=0; i<amount; i++){
                cout<<z[i]<<endl;
            };break;
            case 4:
            for(int i=1; i<size; i++,trash--) {
                int index = (trash > 0) ? amount % (size - 1) - trash + (i - 1) * sizeBuf : amount % (size - 1) + (i - 1) * sizeBuf;
                int count = (trash > 0) ? sizeBuf + 1 : sizeBuf;
                MPI_Recv(&a[index], count, MPI_INT, i, RES, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Recv(&b[index], count, MPI_INT, i, RES, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            };
            cout<<"a\tb\n";
            for(int i=0; i<amount; i++){
                cout<<a[i]<<"\t "<<b[i]<<endl;
            }break;
            default:break;

        }
    } else {
        MPI_Status status;
        MPI_Recv(&flag, 1, MPI_INT, 0, FLAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if(flag > 0 && flag < 3){
            MPI_Recv(&alpha, 1, MPI_INT, 0, ALPHA, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&beta, 1, MPI_INT, 0, BETA, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        MPI_Probe(0, A, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &sizeBuf);
        a = new int[sizeBuf];
        b = new int[sizeBuf];
        MPI_Recv(a, sizeBuf, MPI_INT, 0, A, MPI_COMM_WORLD, &status);
        MPI_Recv(b, sizeBuf, MPI_INT, 0, B, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        switch(flag){
            case 1:
                z = new int[sizeBuf];
                for(int i=0; i<sizeBuf; i++){
                    z[i] = alpha*a[i] + beta*b[i];
                }
                MPI_Send(z, sizeBuf, MPI_INT, 0, RES, MPI_COMM_WORLD);
                break;
            case 2:
                for(int i=0; i<sizeBuf; i++){
                    b[i] = alpha*a[i] + beta*b[i];
                }
                MPI_Send(b, sizeBuf, MPI_INT, 0, RES, MPI_COMM_WORLD);
                break;
            case 3:
                z = new int[sizeBuf];
                for(int i=0; i<sizeBuf; i++){
                    z[i] = a[i] * b[i];
                }
                MPI_Send(z, sizeBuf, MPI_INT, 0, RES, MPI_COMM_WORLD);
                break;
            case 4:
                int store;
                for(int i=0; i<sizeBuf; i++){
                    store = a[i];
                    a[i] = b[i];
                    b[i] = store;
                }
                MPI_Send(a, sizeBuf, MPI_INT, 0, RES, MPI_COMM_WORLD);
                MPI_Send(b, sizeBuf, MPI_INT, 0, RES, MPI_COMM_WORLD);
                break;
            default: break;
        }
    }
    delete []a,b,z;
    MPI_Finalize();
    return 0;
}