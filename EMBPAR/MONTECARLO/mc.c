/* mc.c 
 * Monte Carlo Parallel Example
 */
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <unistd.h>
#include <time.h>
#include "timer.h"

MPI_Status status;
int rank;
int size;

void seed_by_time(int offset) {
    time_t the_time;
    time(&the_time);
    srand((int)the_time+offset);
}

double func(double x, double y, double z) {
    return x*x+y*y+z*z;
}

int main(int argc, char ** argv) {
    int i,n,count;
    double x,y,z,f,x1,x2,y1,y2,z1,z2,f1,f2;
    int buffer;
    Timer timer;

    TimerReset(&timer);
    TimerStart(&timer);

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    seed_by_time(rank);

    if(argc>1) {
        sscanf(argv[1],"%d",&n);
    } else {
        n=10;
    }

    x1=0.0;
    x2=1.0;
    y1=0.0;
    y2=1.0;
    z1=0.0;
    z2=1.0;
    f1=0.0;
    f2=3.0;

    if(rank==0) {
        printf("Monte Carlo Example, n=%d\n",n);
    }

    count=0;
    for (i = rank ; i < n; i+=size) {
        x = x1+(x2-x1)*rand()/RAND_MAX;
        y = y1+(y2-y1)*rand()/RAND_MAX;
        z = z1+(z2-z1)*rand()/RAND_MAX;
        f = f1+(f2-f1)*rand()/RAND_MAX;

        if(f<func(x,y,z)) {
            count++;
        }
    }

    MPI_Reduce(&count,&buffer,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);

    if(rank==0) {
        count=buffer;
        printf("Monte Carlo results from server\n");
        printf("%d of %d hits\n",count,n);
        printf("Integral = %lf\n",
            ((double)count/(double)n*(x2-x1)*(y2-y1)*(z2-z1)*(f2-f1))
        );
        TimerStop(&timer);
        printf("Wall Time %lf\n",TimerRead(&timer));
    }

    MPI_Finalize();
    return 0;

}

