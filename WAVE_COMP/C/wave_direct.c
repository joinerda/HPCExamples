#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

double * linspace(double min, double max, int n)  {
    int i;
    double * array = (double *)malloc(sizeof(double)*n);
    double step = (max-min)/(double)(n-1);
    array[0] = min;
    for(i=1;i<n-1;i++)
        array[i] = min+(double)i*step;
    array[n-1]=max;
    return array;
}
double * zeros(int n)  {
    int i;
    double * array = (double *)malloc(sizeof(double)*n);
    for(i=0;i<n;i++)
        array[i] = 0.0;
    return array;
}

int main(int argc, char ** argv) {
    int n=500;
    double * x = linspace(0,10,n);
    double * A = zeros(n);
    double * V = zeros(n);
    double * dVdt = zeros(n);
    double dx = x[1]-x[0];
    double dxsquared = dx*dx;
    int nt=100000;
    double * tgrid = linspace(0,10,nt);
    double dt = tgrid[1]- tgrid[0];
    int i,it;
    clock_t start,end;
    double time_elapsed;

    for(i=0;i<n;i++) {
        A[i] = exp(-(x[i]-5.0)*(x[i]-5.0));
    }
     
    start = clock();
    for(it=0;it<nt;it++) {
        for(i=1;i<n-1;i++) {
          dVdt[i] = (A[i+1]+A[i-1]-2.0*A[i])/dxsquared;
        }
        for(i=1;i<n-1;i++) {
            A[i] = A[i] + V[i]*dt;
            V[i] = V[i] + dVdt[i]*dt;
        }
    }
    end = clock();
	
	
	
    time_elapsed = (end-start)/(double)CLOCKS_PER_SEC;
    

    for(i=0;i<n;i++) {
        if(x[i]>4.9 && x[i]<5.1) 
            printf("%g %g\n",x[i],A[i]);
    }
    printf("TIME IN LOOP = %lf\n",time_elapsed);
    
    free(x);
    free(A);
    free(V);
    free(dVdt);
    free(tgrid);

}
