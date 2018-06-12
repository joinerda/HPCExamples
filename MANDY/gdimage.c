#ifdef USE_GD

#include <gd.h>
#include <gdfontl.h>
#include <math.h>
#include "gdimage.h"

void drawImage(int * buffer,int nx, int ny,const char * fname) {
    int i,j,k,scale;
    int *maxlist,max,*minlist,min;
    int nmax;
    double ncolors;
    GDIMAGE_IM = gdImageCreate(nx,ny);
    GDIMAGE_BLACK = gdImageColorResolve(GDIMAGE_IM, 0, 0, 0);
    GDIMAGE_WHITE = gdImageColorResolve(GDIMAGE_IM, 255, 255, 255);
    ncolors = (double)GDIMAGE_NCOLORS;
    for (i=0;i<GDIMAGE_NCOLORS;i++) {
        GDIMAGE_CMAP[GDIMAGE_NCOLORS-i-1] = gdImageColorResolve(GDIMAGE_IM,
            (int)(1.1*(ncolors-i)/ncolors*255.0),
            (int)((ncolors/2.0-abs(ncolors/2.0-i))/(ncolors/2.0)*250.0)+5,
            (int)((double)(i)/ncolors*255.0)
        );
    }
    nmax = (nx*ny)/100;
    if(nmax<1) nmax=1;
    if(nmax>5) nmax=5;
    if(nmax>nx/3) nmax=nx/3;
    if(nmax>ny/3) nmax=ny/3;
    maxlist = (int *)malloc(sizeof(double)*nmax);
    minlist = (int *)malloc(sizeof(double)*nmax);

    gdImageFilledRectangle(GDIMAGE_IM,0,0,nx,ny,GDIMAGE_WHITE);
    for(i=0;i<nmax;i++) maxlist[i] = buffer[0];
    for(i=0;i<nmax;i++) minlist[i] = buffer[0];
    for(i=0;i<nx;i++) {
     for(k=0;k<ny;k++) {
        if(buffer[i*ny+k]>maxlist[nmax-1]) {
            for(j=0;j<nmax-1;j++) maxlist[j]=maxlist[j+1];
            maxlist[nmax-1]=buffer[i*ny+k];
        }
        if(buffer[i*ny+k]<minlist[nmax-1]) {
            for(j=0;j<nmax-1;j++) minlist[j]=minlist[j+1];
            minlist[nmax-1]=buffer[i*ny+k];
        }
     }
    }
    max = maxlist[0];
    min = minlist[0];
    if(max==min) {
        min=0;
        max=100;
    }
    if(max>100) max=100;
    if(min<0) min=0;
    if(min>max) min=max-10;
    for(i=0;i<nx;i++) {
        for(j=0;j<ny;j++) {
            scale = (int)((double)(buffer[i*ny+j]-min)/(double)(max-min)*ncolors);
            if(scale<0) scale=0;
            if(scale>ncolors-1) scale=ncolors-1;
            if(buffer[i*ny+j]==0) {
                gdImageSetPixel(GDIMAGE_IM,i,j,GDIMAGE_BLACK);
            } else {
                gdImageSetPixel(GDIMAGE_IM,i,j,GDIMAGE_CMAP[scale]);
            }
        }
    }
    gdImageRectangle(GDIMAGE_IM,0,0,nx-1,ny-1,GDIMAGE_BLACK);
    GDIMAGE_PNGOUT = fopen(fname, "wb");
    gdImagePng(GDIMAGE_IM, GDIMAGE_PNGOUT);
    fclose(GDIMAGE_PNGOUT);

    //cleanup
    gdImageDestroy(GDIMAGE_IM);
    free(maxlist);
    free(minlist);
}

#endif
