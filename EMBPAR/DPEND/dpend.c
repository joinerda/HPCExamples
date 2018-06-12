/* rr.c 
 * Round Robin Parallel Example
 */
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <gd.h>
#include <gdfontl.h>
#include <math.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv.h>
#include "timer.h"

MPI_Status status;
int rank;
int size;


void drawImage(double * buffer,int n,const char * fname);

int func (double t, const double y[], double f[],
           void *params) {
    double th1 = y[0];
    double th2 = y[1];
    double th1d = y[2];
    double th2d = y[3];
    double A,B,C,D,E,F;
    double mr = 0.5;
    double l1=0.5;
    double l2=0.5;
    double g=9.8;

    A=1.0;
    B = mr*l2/l1*cos(th1-th2);
    C = l1/l2*cos(th1-th2);
    D = 1.0;
    E = -mr*l2/l1*th2d*th2d*sin(th1-th2)-g/l1*sin(th1);
    F = l1/l2*th1d*th1d*sin(th1-th2)-g/l2*sin(th2);

    f[0]=th1d;
    f[1]=th2d;
    f[2]=(D*E-B*F)/(A*D-B*C);
    f[3]=(A*F-C*E)/(A*D-B*C);

    return GSL_SUCCESS;
}


double pixelValue(int i,int j,int n,
        double th1min,double th1max,double th2min,double th2max,
        double tfinal) {
    double th1i,th2i;
    gsl_odeiv_system sys = {func, NULL, 4, NULL};
    const gsl_odeiv_step_type * T = gsl_odeiv_step_rk8pd;
    gsl_odeiv_step * s = gsl_odeiv_step_alloc (T, 4);
    gsl_odeiv_control * c = gsl_odeiv_control_y_new (1e-6, 0.0);
    gsl_odeiv_evolve * e = gsl_odeiv_evolve_alloc (4);
    double t = 0.0, t1 = tfinal;
    double h = 1e-6;
    double y[4] = {0.0 , 0.0, 0.0 , 0.0};
    double max1,max,dist;


    th1i = th1min + i*(th1max-th1min)/(double)(n-1);
    th2i = th2min + j*(th2max-th2min)/(double)(n-1);
    y[0] = th1i;
    y[1] = th2i;

    max = th1i*th1i+th2i*th2i;
    max1=max;
    while (t < t1) {
        int status = gsl_odeiv_evolve_apply (e, c, s, &sys, &t, t1, &h, y);
        if (status != GSL_SUCCESS) break;
        dist = (y[0]*y[0]+y[1]*y[1])/max1;
        if(dist>max)max=dist;
    }
    gsl_odeiv_evolve_free (e);
    gsl_odeiv_control_free (c);
    gsl_odeiv_step_free (s);
    return log(max);
}

int main(int argc, char ** argv) {
    int i,j,ij,n,nn,nns1,count,ir,is;
    double * send_buffer;
    double * recv_buffer;
    double * tmp_buffer;
    double th1min=0.0;
    double th1max=M_PI;
    double th2min=M_PI;
    double th2max=0.0;
    double tfinal=10.0;
    double perc,nperc;
    char fname[120];
    Timer timer;

    TimerReset(&timer);
    TimerStart(&timer);
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    if(argc>1) {
        sscanf(argv[1],"%d",&n);
    } else {
        n=10;
    }
    nn = n*n;
    nns1 = nn/size+1;
    if(argc>2) sscanf(argv[2],"%lf",&th1min);
    if(argc>3) sscanf(argv[3],"%lf",&th1max);
    if(argc>4) sscanf(argv[4],"%lf",&th2min);
    if(argc>5) sscanf(argv[5],"%lf",&th2max);
    if(argc>6) sscanf(argv[6],"%lf",&tfinal);
    if(argc>7) sscanf(argv[7],"%s",fname);
    else sprintf(fname,"file.png");

    if(rank==0) {
        printf("Round Robin Example, n=%d\n",n);
        printf("th1min=%lf\n",th1min);
        printf("th1max=%lf\n",th1max);
        printf("th2min=%lf\n",th2min);
        printf("th2max=%lf\n",th2max);
    }

    send_buffer = (double *)malloc(sizeof(double)*nns1);
    tmp_buffer = (double *)malloc(sizeof(double)*size*nns1);
    recv_buffer = (double *)malloc(sizeof(double)*nn);

    nperc = 10.0;
    count=0;
    for (ij = rank ; ij < n*n; ij+=size) {
        i = ij/n;
        j = ij%n;
        send_buffer[count] = pixelValue(i,j,n,
            th1min,th1max,th2min,th2max,tfinal);
        if(rank==0) {
            perc = (double)ij/(double)(n*n)*100.0;
            if(perc>nperc) {
                printf("PERCENTAGE COMPLETE = %lf\n",nperc);
                nperc += 10.0;
            }
        }
        count++;
    }
    MPI_Gather(send_buffer,nns1,MPI_DOUBLE,tmp_buffer,nns1,MPI_DOUBLE,
        0,MPI_COMM_WORLD);


    if(rank==0) {
        for(ij=0;ij<nn;ij++) {
            ir = ij/size;
            is = ij%size;
            recv_buffer[ij] = tmp_buffer[is*nns1+ir];
        }
        printf("Drawing image\n");
        drawImage(recv_buffer,n,fname);
        TimerStop(&timer);
        printf("Wall Time %lf s \n",TimerRead(&timer));
    }

    free(recv_buffer);
    free(tmp_buffer);
    free(send_buffer);
    MPI_Finalize();
    return 0;

}

gdImagePtr im;
FILE *pngout;
int black;
int white;
#define NCOLORS 100
int cmap[NCOLORS];

void drawImage(double * buffer,int n,const char * fname) {
    int i,j,scale;
    double *maxlist,max,*minlist,min,ncolors;
    int nmax;
    im = gdImageCreate(n,n);
    black = gdImageColorResolve(im, 0, 0, 0);
    white = gdImageColorResolve(im, 255, 255, 255);
    ncolors = (double)NCOLORS;
    for (i=0;i<NCOLORS;i++) {
        cmap[i] = gdImageColorResolve(im,
            (int)((ncolors-i)/ncolors*255.0),
            (int)((ncolors/2.0-abs(ncolors/2.0-i))/(ncolors/2.0)*250.0)+5,
            (int)((i)/ncolors*255.0)
        );
    }
    nmax = (n*n)/100;
    if(nmax<1) nmax=1;
    if(nmax>5) nmax=5;
    if(nmax>n/3) nmax=n/3;
    maxlist = (double *)malloc(sizeof(double)*nmax);
    minlist = (double *)malloc(sizeof(double)*nmax);

    gdImageFilledRectangle(im,0,0,n,n,white);
    for(i=0;i<nmax;i++) maxlist[i] = buffer[0];
    for(i=0;i<nmax;i++) minlist[i] = buffer[0];
    for(i=0;i<n*n;i++) {
        if(buffer[i]>maxlist[nmax-1]) {
            for(j=0;j<nmax-1;j++) maxlist[j]=maxlist[j+1];
            maxlist[nmax-1]=buffer[i];
        }
        if(buffer[i]<minlist[nmax-1]) {
            for(j=0;j<nmax-1;j++) minlist[j]=minlist[j+1];
            minlist[nmax-1]=buffer[i];
        }
    }
    max = maxlist[0];
    min = minlist[0];
    if(max==min) { max += 0.5; min -= 0.5; }
    for(i=0;i<n;i++) {
        for(j=0;j<n;j++) {
            scale = (int)((buffer[i*n+j]-min)/(max-min)*ncolors);
            if(scale<0) scale=0;
            if(scale>ncolors-1) scale=ncolors-1;
            gdImageSetPixel(im,i,j,cmap[scale]);
        }
    }
    gdImageRectangle(im,0,0,n-1,n-1,black);
    pngout = fopen(fname, "wb");
    gdImagePng(im, pngout);
    fclose(pngout);

    //cleanup
    gdImageDestroy(im);
    free(maxlist);
    free(minlist);
}


