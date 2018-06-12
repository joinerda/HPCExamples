/* rr.c 
 * Round Robin Parallel Example
 */
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

MPI_Status status;
int rank;
int size;

int main(int argc, char ** argv) {
    int i,j,n;
    double * send_buffer;
    double * recv_buffer;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    if(argc>1) {
        sscanf(argv[1],"%d",&n);
    } else {
        n=10;
    }

    if(rank==0) {
        printf("Round Robin Example, n=%d\n",n);
    }

    send_buffer = (double *)malloc(sizeof(double)*n*n);
    recv_buffer = (double *)malloc(sizeof(double)*n*n);
    for (i = rank ; i < n; i+=size) {
        for (j = 0 ; j < n; j++) {
            send_buffer[i*n+j]=0.0;
        }
    }

    for (i = rank ; i < n; i+=size) {
        for (j = 0 ; j < n; j++) {
            send_buffer[i*n+j] = (double)i+4.0*(double)j;
            printf("process %d, x[%d,%d] being set to %lf\n",rank,
                i,j,send_buffer[i]);
        }
    }

    MPI_Reduce(send_buffer,recv_buffer,n*n,MPI_DOUBLE,
        MPI_SUM,0,MPI_COMM_WORLD);

    if(rank==0) {
        printf("Round Robin results from server\n");
        for(i=0;i<n;i++) {
            for(j=0;j<n;j++) {
                printf("\tx[%d,%d] = %lf\n",i,j,recv_buffer[i*n+j]);
            }
        }
    }

    MPI_Finalize();
    return 0;

}

