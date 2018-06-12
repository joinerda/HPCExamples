/* trap.c 
 * Trapezoidal Rule MPI Example
 */
#include <stdio.h>
#ifdef USE_MPI
#include <mpi.h>
#endif
#include <math.h>
#include "timer.h"

int rank;
int size;

double func(double x) {
    return cos(x);
}

int main(int argc, char ** argv) {
    int i,n;
    double weight,x,x_1,x_n,step,buffer,sum;
    Timer timer;

#ifdef USE_MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
#else
    rank=0;
    size=1;
#endif

    if(argc>1) {
        sscanf(argv[1],"%d",&n);
    } else {
        n=10;
    }

    if(rank==0) {
        printf("Trapezoidal Rule Example, n=%d\n",n);
        TimerReset(&timer);
        TimerStart(&timer);
    }

    sum = 0.0;
    x_1=-M_PI/2;
    x_n=M_PI/2;
    step = (x_n-x_1)/(double)(n-1);
    for(i=rank; i<n; i+=size) {
        x = x_1 + step*i;
        if(i==0||i==n-1) weight=0.5*step; else weight=step;
        sum += weight * func(x);
    }

#ifdef USE_MPI
    MPI_Allreduce(&sum,&buffer,1,MPI_DOUBLE,
        MPI_SUM,MPI_COMM_WORLD);
    sum = buffer;
#endif

    if(rank==0) {
        TimerStop(&timer);
        printf("Trapezoidal Rule results from server\n");
        printf("\tintegral = %le \n",sum);
        printf("Wall Time %lf s\n",TimerRead(&timer));
    }

#ifdef USE_MPI
    MPI_Finalize();
#endif
    return 0;

}

