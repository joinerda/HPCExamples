#include <stdio.h>
#include <mpi.h>

#define func(x) (x*x)
#define n 16

int main(int argc, char ** argv) {

    int i;
    int rank,size;
    int array1[n];
    int array2[n];
    int array2_buffer[n];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);


    for(int i=0;i<n;i++) {
        array1[i] = i;
    }

    for(int i=0;i<n;i++) {
        array2[i] = 0;
    }

    for(int i=rank;i<n;i+=size) {
        array2[i] = func(array1[i]);
    }
    MPI_Reduce(array2,array2_buffer,n,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);

    if(rank==0) {
        for(int i=0;i<n;i++) {
            array2[i]=array2_buffer[i];
        }
    }
    
    if(rank==0) {
        for(int i=0;i<n;i++) {
            printf("%d %d \n",array1[i],array2[i]);
        }
    }
    
    MPI_Finalize();

}
