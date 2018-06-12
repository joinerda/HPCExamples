#include <stdio.h>
#include <time.h>


#define N 100000000

int func(int x) {
    return x*x;
}
void afunc(int *a) {
    int i;
    for(i=0;i<N;i++) a[i] = i*i;
}

int main(int argc, char ** argv) {
    int i;
    int a[N];
    int mode=-1;
    clock_t start,end;
    double elapsed_time;

    printf("USAGE: arrayfunc 0|1\n  0=pass scalar to function within loop\n  1=pass array to function, function contains loop\n");
    if(argc>1) {
        sscanf(argv[1],"%d",&mode);
    } 

    start = clock();
    if(mode==0) {
        for(i=0;i<N;i++) {
            a[i]=func(i);
        }
    } else if(mode==1) {
        afunc(a);
    } else {
        printf("ERROR: INVALID MODE ENTERED\n");
    }
    end = clock();
    elapsed_time = (end-start)/(double)CLOCKS_PER_SEC;
    printf("Elapsed Time = %lf\n",elapsed_time);
}
