/* rw.c 
 * Random Walk Parallel Example
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

int main(int argc, char ** argv) {
    int i,j,n,s,position;
    int * count;
    int * buffer;
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
    if(argc>2) {
        sscanf(argv[2],"%d",&s);
    } else {
        s=100;
    }
    count = (int *)malloc(sizeof(int)*(2*s+1));
    buffer = (int *)malloc(sizeof(int)*(2*s+1));

    if(rank==0) {
        printf("Random Walk Example, n=%d\n",n);
    }

    for (j=0;j<2*s+1;j++) count[j]=0;
    for (i = rank ; i < n; i+=size) {
        position=0;
        for (j=0;j<s;j++) {
            double test = (double)rand()/(double)RAND_MAX;
            if(test<(1.0/3.0)) {
                position++;
            } else if(test<(2.0/3.0)) {
                position--;
            } // otherwise no change
        }
        count[position+(s)]++;
    }

    MPI_Reduce(count,buffer,2*s+1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);

    if(rank==0) {
        for (j=0;j<2*s+1;j++) count[j]=buffer[j];
        printf("Random Walk results from server\n");
        for (j=0;j<2*s+1;j++) {
            printf("%d  %d \n",j-s,count[j]);
        }
        TimerStop(&timer);
        printf("Wall Timer %lf \n",TimerRead(&timer));
    }

    free(count);
    free(buffer);
    MPI_Finalize();
    return 0;

}

