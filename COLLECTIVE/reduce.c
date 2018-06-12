#include <stdio.h>
#include <mpi.h>

#define func(x) (x*x)
#define n 1000

int main(int argc, char ** argv) {

    int i;
    double sum,sumbuffer;
    double xmin = 0.0;
    double xmax = 10.0;
    int rank,size;
    double dx = (xmax-xmin)/(double)(n-1);

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    if(rank==0) sum = 0.5*(func(xmin)+func(xmax));
    else sum=0.0;
    for(i=rank+1;i<n-1;i+=size) {
        sum += func(xmin+i*dx);
    }
    sum *= dx;
    MPI_Reduce(&sum,&sumbuffer,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
    if(rank==0) sum=sumbuffer;

    
    if(rank==0) printf("ANSWER = %lf\n",sum);
    
    MPI_Finalize();

}
