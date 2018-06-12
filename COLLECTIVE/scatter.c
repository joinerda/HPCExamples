#include <stdio.h>
#include <mpi.h>

#define func(x) (x*x)
#define n 16

int main(int argc, char ** argv) {

    int i;
    int rank,size;
    int array1[n];
    int array1_buffer[n];
    int array2[n];
    int array2_buffer[n];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);


    if(rank==0) {
        if(n%size != 0 ) {
            printf("WARNING using scatter/gather with array size that does \n");
            printf(" not split evenly. Use scatterv/gatherv instead to \n");
            printf(" avoid errors! \n");
        }
    }

    if(rank==0) {
        for(int i=0;i<n;i++) {
            array1[i] = i;
        }
    }

    MPI_Scatter(array1,n/size,MPI_INT,array1_buffer,
        n/size,MPI_INT,0,MPI_COMM_WORLD);
    for(int i=0;i<n/size;i++) {
        array2_buffer[i] = func(array1_buffer[i]);
    }
    MPI_Gather(array2_buffer,n/size,MPI_INT,array2,
        n/size,MPI_INT,0,MPI_COMM_WORLD);
    
    if(rank==0) {
        for(int i=0;i<n;i++) {
            printf("%d %d \n",array1[i],array2[i]);
        }
    }
    
    MPI_Finalize();

}
