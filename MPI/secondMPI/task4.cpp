#include <mpi/mpi.h>
#include <iostream>
#include <stdlib.h>
#include <ctime>

using namespace std;
int main(int ac, char** av){
    MPI_Init (&ac, &av);
    srand( time(0));
    MPI_GROUP_EMPTY group;
    //TAGS
    int N = 0;
    int REQ = 1;
    int RES = 2;

    //Variable
    int n,size,rank;
    char *message;
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);
    MPI_Comm_size (MPI_COMM_WORLD, &size);
    if(rank == 0){
        long time;
        cout<<"Введите размер сообщения:\n";
        cin>>n;
        message = new char[n];
        for(int i=0; i<n;i++){
            message[i] = (char)(65 + rand()%28);
            cout<<message[i];
        }
        cout<<endl;
        MPI_Send(&n,1, MPI_INT, 1, N, MPI_COMM_WORLD);
        cout<<endl;
        time = clock();
        MPI_Send(message,n, MPI_CHAR, 1, REQ, MPI_COMM_WORLD);
        MPI_Recv(message, n, MPI_CHAR, 1, RES, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        time = clock() - time;
        cout<<time<<endl;
    }else if(rank == 1){
        MPI_Recv(&n, 1, MPI_INT, 0, N, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        message = new char(n);
        MPI_Recv(message, n, MPI_CHAR, 0, REQ, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(message, n, MPI_CHAR, 0, RES, MPI_COMM_WORLD);
    }

    delete []message;
    MPI_Finalize();
    return 0;
}