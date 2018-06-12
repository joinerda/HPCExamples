#ifdef USE_GD

#ifndef GDIMAGE
#define GDIMAGE

#include <stdio.h>
#include <stdlib.h>
#include <gd.h>
#include <gdfontl.h>
#include <math.h>

void drawImage(int * buffer,int nx, int ny,const char * fname);

gdImagePtr GDIMAGE_IM;
FILE *GDIMAGE_PNGOUT;
int GDIMAGE_BLACK;
int GDIMAGE_WHITE;
#define GDIMAGE_NCOLORS 200
int GDIMAGE_CMAP[GDIMAGE_NCOLORS];

#endif

#endif
