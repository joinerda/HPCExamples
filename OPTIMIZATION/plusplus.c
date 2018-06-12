#include <stdio.h>
#include <time.h>
#define N 100000000

int main(int argc, char ** argv) {
    int i,j;
    int a[N];
	int mode=-1;
    int repeat=10;
    clock_t start,end;
    double elapsed_time;
    int Nstop;

    printf("USAGE: plusplus 0|1 \n    0 = pluplus     1 = plusequals1\n");
    if(argc>1) {
        sscanf(argv[1],"%d",&mode);
    }

    start = clock() ;

    if(mode==0) {
        for(j=0;j<repeat;j+=1) 
            for(i=0;i<N;i++) {
                a[i] = i;
            }
    } else if(mode==1) {
        for(j=0;j<repeat;j+=1) 
            for(i=0;i<N;i+=1) {
                a[i] = i;
            }
    } else {
        printf("READ THE FINE USAGE STATEMENT!\n");
        exit(0);
    }
    end = clock() ;
    elapsed_time = (end-start)/(double)CLOCKS_PER_SEC ;

    printf("Time = %lf\n",elapsed_time);
}

