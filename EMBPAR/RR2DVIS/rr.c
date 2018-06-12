/* rr.c 
 * Round Robin Parallel Example
 */
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <gd.h>
#include <gdfontl.h>
#include "timer.h"

MPI_Status status;
int rank;
int size;

void drawImage(double * buffer,int n,const char * fname);

int main(int argc, char ** argv) {
    int i,j,ij,n;
    double * send_buffer;
    double * recv_buffer;
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

    if(rank==0) {
        printf("Round Robin Example, n=%d\n",n);
    }

    send_buffer = (double *)malloc(sizeof(double)*n*n);
    recv_buffer = (double *)malloc(sizeof(double)*n*n);
    for (ij = 0 ; ij < n*n; ij++) {
            send_buffer[ij]=0.0;
    }

    for (ij = rank ; ij < n*n; ij+=size) {
        i = ij/n;
        j = ij%n;
        send_buffer[i*n+j] = (double)i+4.0*(double)j;
    }

    MPI_Reduce(send_buffer,recv_buffer,n*n,MPI_DOUBLE,
        MPI_SUM,0,MPI_COMM_WORLD);

    if(rank==0) {
        printf("Drawing image\n");
        drawImage(recv_buffer,n,"file.png");
        TimerStop(&timer);
        printf("Wall Time %lf s \n",TimerRead(&timer));
    }

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
    double max,min,ncolors;
    im = gdImageCreate(n,n);
    black = gdImageColorResolve(im, 0, 0, 0);
    white = gdImageColorResolve(im, 255, 255, 255);
    ncolors = (double)NCOLORS;
    for (i=0;i<NCOLORS;i++) {
        cmap[i] = gdImageColorResolve(im,
            (int)((ncolors-i)/ncolors*255.0),
            (int)((ncolors/2.0-abs(ncolors/2.0-i))/(ncolors/2.0)*255.0),
            (int)((i)/ncolors*255.0)
        );
    }

    gdImageFilledRectangle(im,0,0,n,n,white);
    max=buffer[0];
    min=buffer[0];
    for(i=0;i<n*n;i++) {
        if(buffer[i]>max) max=buffer[i];
        if(buffer[i]<min) min=buffer[i];
    }
    if(max==min) { max += 0.5; min -= 0.5; }
    for(i=0;i<n;i++) {
        for(j=0;j<n;j++) {
            scale = (int)((buffer[i*n+j]-min)/(max-min)*ncolors);
            if(scale<0) scale=0;
            if(scale>99) scale=99;
            gdImageSetPixel(im,i,j,cmap[scale]);
        }
    }
    gdImageRectangle(im,0,0,n-1,n-1,black);
    pngout = fopen(fname, "wb");
    gdImagePng(im, pngout);
    fclose(pngout);

    //cleanup
    gdImageDestroy(im);
}


