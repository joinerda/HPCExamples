#include <stdio.h>
#include <math.h> //COMPILE WITH -lm OPTION!!!
#include <time.h>

#define N 100000000

int main(int argc, char ** argv) {
    int i;
    double a[N];
	int mode=-1;
    clock_t start,end;
    double elapsed_time;

    printf("USAGE: strengthred 0|1\n  0=pow(i,2)\n  1=i*i\n");
    if(argc>1) {
        sscanf(argv[1],"%d",&mode);
    }

    start = clock();
	if(mode==0) {
		for(i=0;i<N;i+=1) {
			a[i] = pow((double)i,2.0);
		}
	} else if(mode==1) {
		for(i=0;i<N;i+=1) {
			a[i] = (double)i*(double)i;
		}
	} else {
		printf("ERROR: INVALID MODE ENTERED\n");
	}
    end = clock();
    elapsed_time = (end-start)/(double)CLOCKS_PER_SEC;
    printf("Elapsed Time = %lf\n",elapsed_time);
    
}
