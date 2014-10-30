#include <mpi/mpi.h>
#include <iostream>
#include <stdlib.h>
#include <ctime>

using namespace std;
int main(int ac, char** av){
    srand(time(0));
    MPI_Init (&ac, &av);

    //TAGS
    int N = 0;
    int REQ = 1;
    int RES = 2;

    //Variable
    int ranks[2] = {0, 1};
    int n,size,rank;
    char *message;
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);
    MPI_Comm_size (MPI_COMM_WORLD, &size);
    MPI_Comm chatComm;
    MPI_Group worldGroup, chatGroup;
    MPI_Comm_group(MPI_COMM_WORLD, &worldGroup);
    MPI_Group_incl(worldGroup, 2, ranks, &chatGroup);
    MPI_Comm_create(MPI_COMM_WORLD, chatGroup, &chatComm);

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
        MPI_Send(&n,1, MPI_INT, 1, N, chatComm);
        cout<<endl;
        MPI_Barrier(chatComm);
        time = clock();
        MPI_Send(message,n, MPI_CHAR, 1, REQ, chatComm);
        MPI_Recv(message, n, MPI_CHAR, 1, RES, chatComm, MPI_STATUS_IGNORE);
        time = clock() - time;
        printf("Time: %ld\n", time);
    }else if(rank == 1){
        MPI_Recv(&n, 1, MPI_INT, 0, N, chatComm, MPI_STATUS_IGNORE);
        message = new char(n);
        MPI_Barrier(chatComm);
        MPI_Recv(message, n, MPI_CHAR, 0, REQ, chatComm, MPI_STATUS_IGNORE);
        MPI_Send(message, n, MPI_CHAR, 0, RES, chatComm);
    }
    delete []message;
    MPI_Finalize();
    return 0;
}