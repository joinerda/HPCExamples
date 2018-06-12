#include <mpi.h>
#include <stdio.h>

#define MAX_ARRAY_LENGTH 800000

int main(int argc,char ** argv) {
    int size,rank;
    int buffer_in[MAX_ARRAY_LENGTH];
    int buffer_out[MAX_ARRAY_LENGTH];
    int other;
    MPI_Status status;
    int messagelength,sendfirst;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    if (size!=2) {
        printf("\bwrong number of processes\n");
        exit(0);
    }

    if (argc<3) {
        printf("\n(1) Usage: a.out _sendfirst(1|0|-1)_ _messagelength_\n");
        exit(0);
    }
    sscanf(argv[1],"%d",&sendfirst);
    if (sendfirst!=0&&sendfirst!=1&&sendfirst!=-1) {
        printf("\n(2) Usage: a.out _sendfirst(1|0|-1)_ _messagelength_\n");
        exit(0);
    }
    sscanf(argv[2],"%d",&messagelength);
    if (messagelength<1 || messagelength>MAX_ARRAY_LENGTH) {
        printf("(3) _messagelength_ should be between 1 and %d\n",
            MAX_ARRAY_LENGTH);
        exit(0);
    }

    other = (rank+1)%2;

    if (sendfirst==1) {
        printf("\n%d/%d: sending %d\n",rank,size,messagelength);
        MPI_Send(buffer_out,messagelength,MPI_INT,other,999,MPI_COMM_WORLD);
        MPI_Recv(buffer_in,MAX_ARRAY_LENGTH,MPI_INT,other,999,MPI_COMM_WORLD,
            &status);
        printf("\n%d/%d: received %d\n",rank,size,messagelength);
    } else if(sendfirst==0) {
        printf("\n%d/%d: receiving %d\n",rank,size,messagelength);
        MPI_Recv(buffer_in,MAX_ARRAY_LENGTH,MPI_INT,other,999,MPI_COMM_WORLD,
            &status);
        MPI_Send(buffer_out,messagelength,MPI_INT,other,999,MPI_COMM_WORLD);
        printf("\n%d/%d: sent %d\n",rank,size,messagelength);
    } else {
      if(rank==0) {
        printf("\n%d/%d: sending %d\n",rank,size,messagelength);
        MPI_Send(buffer_out,messagelength,MPI_INT,other,999,MPI_COMM_WORLD);
        MPI_Recv(buffer_in,MAX_ARRAY_LENGTH,MPI_INT,other,999,MPI_COMM_WORLD,
            &status);
        printf("\n%d/%d: received %d\n",rank,size,messagelength);
      } else {
        printf("\n%d/%d: receiving %d\n",rank,size,messagelength);
        MPI_Recv(buffer_in,MAX_ARRAY_LENGTH,MPI_INT,other,999,MPI_COMM_WORLD,
            &status);
        MPI_Send(buffer_out,messagelength,MPI_INT,other,999,MPI_COMM_WORLD);
        printf("\n%d/%d: sent %d\n",rank,size,messagelength);
      }
    }

    MPI_Finalize();
}
