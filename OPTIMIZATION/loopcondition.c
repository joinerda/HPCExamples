#include <stdio.h>
#include <time.h>

    

#define N 100000000
#define nm 99999999

int main(int argc, char ** argv) {
    int i;
    int a[N];
    //int nm = N-1;
    int mode=0;
    clock_t start,end;
    double elapsed_time;

    printf("USAGE: loopcondition 0|1\n  0=no if in loop\n  1=if in loop\n");
    if(argc>1) {
        sscanf(argv[1],"%d",&mode);
    }
    

    start = clock();
    if(mode==0) {
        // no conditions in loop
        a[0]=0;
        for(i=1;i<nm;i++) {
            a[i]=i;
        }
        a[nm]=0;
    } else {
        // conditions in loop
        for(i=0;i<N;i++) {
            if(i==0) a[i]=0;
            else if(i==N-1) a[i]=0;
            else a[i]=i;
        }
    }
    end = clock();
    elapsed_time = (end-start)/(double)CLOCKS_PER_SEC;
    printf("Elapsed Time = %lf\n",elapsed_time);
}
