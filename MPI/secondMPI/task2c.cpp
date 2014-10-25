#include <mpi/mpi.h>
#include <iostream>
#include <stdlib.h>
#include <ctime>

using namespace std;
int main(int ac, char** av){
    MPI_Init (&ac, &av);
    srand( time(0));

    //TAGS
    int A = 1;
    int COUNT=4;
    int N = 0;
    int RES = 3;

    //Variable
    int size,rank,sizeBuf,n, **a,**b;
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);
    MPI_Comm_size (MPI_COMM_WORLD, &size);
    if(rank == 0){
        int trash, index, count;
        cout<<"Введите размерность матрицы n:\n";
        cin>>n;
        sizeBuf = n/(size-1);
        trash = n%(size-1);
        a = new int*[n];
        b = new int*[n];
        for(int i = 0; i < n; i++){
            a[i] = new int[n];
            b[i] = new int[n];
        }
        cout<<"Матрица a\n";
        for(int i=0; i<n; i++){
            for (int j = 0; j < n; ++j) {
                a[i][j] = rand()%100-50;
                cout<<a[i][j]<<"\t ";
            }
            cout<<"\n";
        }
        for(int i = 1; i<size; i++,trash--) {
            index = (trash > 0)?n%(size-1) - trash+(i-1)*sizeBuf:n%(size-1)+(i-1)*sizeBuf;
            count = (trash > 0)?sizeBuf + 1:sizeBuf;
            MPI_Send(&n,1, MPI_INT, i, N, MPI_COMM_WORLD);
            MPI_Send(&count,1, MPI_INT, i, COUNT, MPI_COMM_WORLD);
            for (int j = index; j < index + count; ++j) {
                MPI_Send(&a[j][0],n, MPI_INT, i, A, MPI_COMM_WORLD);
            }
        }
        trash = n%(size-1);
        for(int i=1; i<size; i++,trash--) {
            int index = (trash > 0) ? n % (size - 1) - trash + (i - 1) * sizeBuf : n % (size - 1) + (i - 1) * sizeBuf;
            int count = (trash > 0) ? sizeBuf + 1 : sizeBuf;
            for (int j = 0; j < n; ++j) {
                MPI_Recv(&b[j][index], count, MPI_INT, i, RES, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        };
        cout << "Матрица b\n";
        for(int i=0; i<n; i++){
            for (int j = 0; j < n; ++j) {
                cout<<b[i][j]<<"\t";
            }
            cout<<endl;
            delete []a[i],b[i];
        }
    } else {
        MPI_Recv(&n, 1, MPI_INT, 0, N, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&sizeBuf, 1, MPI_INT, 0, COUNT, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        a = new int*[sizeBuf];
        b = new int*[n];
        for (int i = 0; i < sizeBuf; ++i) {
            a[i] = new int[n];
            b[i] = new int[sizeBuf];
            MPI_Recv(&a[i][0], n, MPI_INT, 0, A, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        for (int i = sizeBuf; i < n; ++i) {
            b[i] = new int[sizeBuf];
        }
        for(int i=0; i<sizeBuf; i++){
            for (int j = 0; j < n; ++j) {
                b[j][i] = a[i][j];
            }
            delete []a[i];
        }
        for (int i = 0; i < n; ++i) {
            MPI_Send(&b[i][0], sizeBuf, MPI_INT, 0, RES, MPI_COMM_WORLD);
            delete []b[i];
        }
    }
    delete []a,b;
    MPI_Finalize();
    return 0;
}