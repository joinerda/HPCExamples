#include <stdio.h>
#include <time.h>

    

#define N 10000

int main(int argc, char ** argv) {
    int i,j;
    int a[N][N];
	int mode=-1;
    clock_t start,end;
    double elapsed_time;

    printf("USAGE: arraystride 0|1\n  0 = inner loop is j\n  1 = inner loop is i\n");
    if(argc>1) {
        sscanf(argv[1],"%d",&mode);
    } 

    start = clock();
    if(mode==0) {
		for(i=0;i<N;i+=1) {
			for(j=0;j<N;j+=1) {
				a[i][j] = i;
			}
		}
	} else if(mode==1) {
	    for(j=0;j<N;j+=1) {
			for(i=0;i<N;i+=1) {
				a[i][j] = i;
			}
		}
	} else {
        printf("ERROR: INVALID MODE ENTERED\n");
    }
    end = clock();
    elapsed_time = (end-start)/(double)CLOCKS_PER_SEC;
    printf("Elapsed Time = %lf\n",elapsed_time);
}
