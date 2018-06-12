/* rr.c 
 * Round Robin Parallel Example
 */
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

MPI_Status status;
int rank;
int size;

void myMPI_dRRGather(double * sbuffer, int n,int root, MPI_Comm comm) {
    double * send_buffer;
    double * tmp_buffer;
    int i,count,is,ir;

    send_buffer = (double *)malloc(sizeof(double)*(n/size+1));
    tmp_buffer = (double *)malloc(sizeof(double)*size*(n/size+1));

    count=0;
    for(i=rank;i<n;i+=size) {
        send_buffer[count++] = sbuffer[i];
    }

    MPI_Gather(send_buffer,n/size+1,MPI_DOUBLE,
        tmp_buffer,n/size+1,MPI_DOUBLE,root,
        comm);

    if(rank==root) {
        for(i=0;i<n;i++) {
            is = i/size;
            ir = i%size;
            sbuffer[i]=tmp_buffer[ir*(n/size+1)+is];
        }
    }

    free(send_buffer);
    free(tmp_buffer);
    return;
}

void myMPI_dRRAGather(double * sbuffer, int n, MPI_Comm comm) {
    double * send_buffer;
    double * tmp_buffer;
    int i,count,is,ir;

    send_buffer = (double *)malloc(sizeof(double)*(n/size+1));
    tmp_buffer = (double *)malloc(sizeof(double)*size*(n/size+1));

    count=0;
    for(i=rank;i<n;i+=size) {
        send_buffer[count++] = sbuffer[i];
    }

    MPI_Allgather(send_buffer,n/size+1,MPI_DOUBLE,
        tmp_buffer,n/size+1,MPI_DOUBLE,
        comm);

    for(i=0;i<n;i++) {
        is = i/size;
        ir = i%size;
        sbuffer[i]=tmp_buffer[ir*(n/size+1)+is];
    }

    free(send_buffer);
    free(tmp_buffer);
    return;
}

int main(int argc, char ** argv) {
    int i,n;
    double * send_buffer;

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

    send_buffer = (double *)malloc(sizeof(double)*(n));

    for (i = rank ; i < n; i+=size) {
        send_buffer[i] = (double)i;
        printf("process %d, x[%d] being set to %lf\n",rank,
            i,send_buffer[i]);
    }

    myMPI_dRRGather(send_buffer,n,0,MPI_COMM_WORLD);

    if(rank==0) {
        printf("Round Robin results from server\n");
        for(i=0;i<n;i++) {
            printf("\tx[%d] = %lf\n",i,send_buffer[i]);
        }
    }

    free(send_buffer);
    MPI_Finalize();
    return 0;

}

