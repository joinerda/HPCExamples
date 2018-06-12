#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char ** argv) {
    int n=8;
    int rank,size;
    int * array;
    int * array_buffer;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    array = (int *)malloc(sizeof(int)*n);
    array_buffer = (int *)malloc(sizeof(int)*n);
    // initialize array to zero on all nodes
    for(int i=0;i<n;i++) array[i]=0;

    // round robin through array to be populated
    for(int i=rank;i<n;i+=size) {
        array[i] = i;
    }

    //echo arrays on each node
    for(int i=0;i<n;i++) {
        printf("BEFORE REDUCE  NODE %d   INDEX %d VALUE %d\n",rank,i,array[i]);
    }

    // "gather" with reduce
    MPI_Reduce(array,array_buffer,n,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
    
    //output array on master node
    if(rank==0) {
        for(int i=0;i<n;i++) {
            printf("AFTER REDUCE  NODE %d INDEX %d VALUE %d\n",rank,i,array_buffer[i]);
        }
    }

}
