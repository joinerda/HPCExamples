#include <stdio.h>
#include <time.h>

    
#define N 100000000

int main(int argc, char ** argv) {
    int i;
    int j,k;
    int a[N];
	int mode=-1;
    clock_t start,end;
    double elapsed_time;

    printf("USAGE: loopinvariance 0|1\n  0=invariant in loop\n  1=invariant outside loop\n");
    if(argc>1) {
        sscanf(argv[1],"%d",&mode);
    }

    start = clock();
	if(mode==0) {
		for(i=0;i<N;i+=1) {
			j=7;
			k=3;
			a[i] = i*j*k;
		}
	} else if(mode==1) {
		j=7;
		k=3;
		for(i=0;i<N;i+=1) {
			a[i] = i*j*k;
		}
	} else {
	    printf("ERROR: INVALID MODE ENTERED\n");
	}
    end = clock();
    elapsed_time = (end-start)/(double)CLOCKS_PER_SEC;
    printf("Elapsed Time = %lf\n",elapsed_time);
    
}
