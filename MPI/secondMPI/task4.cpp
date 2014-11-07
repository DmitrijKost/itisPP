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
    int FLAG = 3;

    //Variable
    int ranks[2] = {0, 1};
    int n,size,rank, flag;
    MPI_Status status[2];
    MPI_Request request[2];
    MPI_Comm chatComm;
    MPI_Group worldGroup, chatGroup;
    MPI_Comm_group(MPI_COMM_WORLD, &worldGroup);
    MPI_Group_incl(worldGroup, 2, ranks, &chatGroup);
    MPI_Comm_create(MPI_COMM_WORLD, chatGroup, &chatComm);
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);
    MPI_Comm_size (MPI_COMM_WORLD, &size);
    if(rank == 0){
        char *message;
        long time;
        cout<<"Введите размер сообщения:\n";
        message = (char*)malloc(n * sizeof(char));
        cin>>n;
        cout<<"0 or 1?:\n";
        cin>>flag;
        MPI_Send(&n,1, MPI_INT, 1, N, chatComm);
        MPI_Send(&flag,1, MPI_CHAR, 1, FLAG, chatComm);
        cout<<endl;
        if(flag == 0) {
            MPI_Barrier(chatComm);
            time = clock();
            MPI_Send(message, n, MPI_CHAR, 1, REQ, chatComm);
            MPI_Recv(message, n, MPI_CHAR, 1, RES, chatComm, MPI_STATUS_IGNORE);
            time = clock() - time;
            printf("Task4a\nTime: %ld\n", time);
        }else if(flag == 1){
            char* message2 = (char*)malloc(n * sizeof(char));
            MPI_Barrier(chatComm);
            time = clock();
            MPI_Isend(message, n, MPI_CHAR, 1, REQ, chatComm, &request[1]);
            MPI_Irecv(message2, n, MPI_CHAR, 1, RES, chatComm, &request[0]);
            MPI_Waitall(2, request, status);
            time = clock() - time;
            printf("Task4b:\nTime: %ld\n", time);
            free(message2);
        }
        free(message);
    }else if(rank == 1){
        char *message;
        MPI_Recv(&n, 1, MPI_INT, 0, N, chatComm, MPI_STATUS_IGNORE);
        MPI_Recv(&flag, 1, MPI_CHAR, 0, FLAG, chatComm, MPI_STATUS_IGNORE);
        message = (char*)malloc(n * sizeof(char));
        if(flag == 0) {
            MPI_Barrier(chatComm);
            MPI_Recv(message, n, MPI_CHAR, 0, REQ, chatComm, MPI_STATUS_IGNORE);
            MPI_Send(message, n, MPI_CHAR, 0, RES, chatComm);
        }else if(flag == 1){
            char* message2 = (char*)malloc(n * sizeof(char));
            MPI_Barrier(chatComm);
            MPI_Isend(message2, n, MPI_CHAR, 0, RES, chatComm, &request[0]);
            MPI_Irecv(message, n, MPI_CHAR, 0, REQ, chatComm, &request[1]);
            free(message2);
        }
        free(message);
    }
    MPI_Finalize();
    return 0;
}