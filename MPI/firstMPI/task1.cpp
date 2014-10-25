#include <mpi/mpi.h>
#include <iostream>
int main(int ac, char** av){
    MPI_Init (&ac, &av);
    std::cout<<"Hello world!\n";
    MPI_Finalize();
    return 0;
}