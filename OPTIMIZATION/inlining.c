#include <stdio.h>
#include <time.h>


#define macro_avg(x,y) ((x)+(y))/2
#define N 100000000

int func_avg(int x, int y) {
    return (x+y)/2;
}

int main(int argc, char ** argv) {
    int i;
    int a[N];
	int mode=-1;
    clock_t start,end;
    double elapsed_time;

    printf("USAGE: inlining 0|1\n  0=use standard function\n  1=use macro\n");
    if(argc>1) {
        sscanf(argv[1],"%d",&mode);
    }
    
	
    start = clock();
    if(mode==0) {
		for(i=0;i<N;i++) {
            a[i] = func_avg(i,1);
        }
	} else if(mode==1) {
		for(i=0;i<N;i++) {
            a[i] = macro_avg(i,1);
        }
	} else {
        printf("ERROR: INVALID MODE ENTERED\n");
    }
    end = clock();
    elapsed_time = (end-start)/(double)CLOCKS_PER_SEC;
    printf("Elapsed Time = %lf\n",elapsed_time);
}
