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
    int B = 2;
    int D = 5;
    int COUNT=4;
    int N = 0;
    int RES = 3;

    //Variable
    int size,rank,sizeBuf,n, **a,**b,**c,**d;
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);
    MPI_Comm_size (MPI_COMM_WORLD, &size);
    if(rank == 0){
        int trash, index, count;
        cout<<"Введите размерность матриц n:\n";
        cin>>n;
        sizeBuf = n/(size-1);
        trash = n%(size-1);
        a = new int*[n];
        b = new int*[n];
        c = new int*[n];
        d = new int*[n];
        for(int i = 0; i < n; i++){
            a[i] = new int[n];
            b[i] = new int[n];
            c[i] = new int[n];
            d[i] = new int[n];
        }
        cout<<"Матрица a\n";
        for(int i=0; i<n; i++){
            for (int j = 0; j < n; ++j) {
                a[i][j] = rand()%100-50;
                cout<<a[i][j]<<"\t ";
            }
            cout<<"\n";
        }
        cout<<"Матрица b\n";
        for(int i=0; i<n; i++){
            for (int j = 0; j < n; ++j) {
                b[i][j] = rand()%100-50;
                cout<<b[i][j]<<"\t ";
            }
            cout<<"\n";
        }
        cout<<"Матрица d\n";
        for(int i=0; i<n; i++){
            for (int j = 0; j < n; ++j) {
                d[i][j] = rand()%100-50;
                cout<<d[i][j]<<"\t ";
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
            for (int j = 0; j < n; ++j) {
                MPI_Send(&b[j][0],n, MPI_INT, i, B, MPI_COMM_WORLD);
                MPI_Send(&d[j][0],n, MPI_INT, i, D, MPI_COMM_WORLD);
            }
        }
        trash = n%(size-1);
        for(int i=1; i<size; i++,trash--) {
            int index = (trash > 0) ? n % (size - 1) - trash + (i - 1) * sizeBuf : n % (size - 1) + (i - 1) * sizeBuf;
            int count = (trash > 0) ? sizeBuf + 1 : sizeBuf;
            for (int j = index; j < count + index; ++j) {
                MPI_Recv(&c[j][0], n, MPI_INT, i, RES, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        };
        cout << "Матрица c\n";
        for(int i=0; i<n; i++){
            for (int j = 0; j < n; ++j) {
                cout<<c[i][j]<<"\t";
            }
            cout<<endl;
            delete []a[i],b[i],c[i],d[i];
        }
    } else {
        MPI_Recv(&n, 1, MPI_INT, 0, N, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&sizeBuf, 1, MPI_INT, 0, COUNT, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        a = new int*[sizeBuf];
        b = new int*[n];
        d = new int*[n];
        c = new int*[sizeBuf];
        for (int i = 0; i < sizeBuf; ++i) {
            a[i] = new int[n];
            c[i] = new int[n];
            MPI_Recv(&a[i][0], n, MPI_INT, 0, A, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        for (int i = 0; i < n; ++i) {
            b[i] = new int[n];
            MPI_Recv(&b[i][0], n, MPI_INT, 0, B, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            d[i] = new int[n];
            MPI_Recv(&d[i][0], n, MPI_INT, 0, D, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        int sum;
        for (int l = 0; l < sizeBuf; ++l) {
            for(int i=0; i<n; ++i){
                for (int j = 0; j < n; ++j) {
                    sum = 0;
                    for (int k = 0; k < n; ++k) {
                        sum += a[l][k]*b[k][j];
                    }
                    c[l][i] += sum*d[j][i];
                }
            }
            MPI_Send(&c[l][0], n, MPI_INT, 0, RES, MPI_COMM_WORLD);
            delete []a[l],c[l];
        }
        for (int i = 0; i < n; ++i) {
            delete []b[i],d[i];
        }
    }
    delete []a,b,c,d;
    MPI_Finalize();
    return 0;
}