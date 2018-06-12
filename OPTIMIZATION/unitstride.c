#include <stdio.h>
#include <time.h>
#define N 100000000

int main(int argc, char ** argv) {
    int i,j;
    int a[N];
	int stride=1;
    int repeat=10;
    clock_t start,end;
    double elapsed_time;
    int Nstop;

    printf("USAGE: unitstride N\n  N = stridelength\n");
    if(argc>1) {
        sscanf(argv[1],"%d",&stride);
    }

    start = clock() ;

    for(j=0;j<repeat;j+=1) 
        for(i=0;i<N;i+=stride) {
            a[i] = i;
        }
    end = clock() ;
    elapsed_time = (end-start)/(double)CLOCKS_PER_SEC ;

    printf("Time doing work w unit stride = %lf\n",elapsed_time);

    Nstop = 0;
    for(i=0;i<N;i+=stride) Nstop++;

    start = clock() ;
    for(j=0;j<repeat;j+=1) 
        for(i=0;i<Nstop;i+=1) {
            a[i] = i;
        }
    end = clock() ;
    elapsed_time = (end-start)/(double)CLOCKS_PER_SEC ;

    printf("Time doing work w/out unit stride = %lf\n",elapsed_time);
}

