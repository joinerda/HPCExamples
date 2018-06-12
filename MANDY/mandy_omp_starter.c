//##### mandelbrot set
//## prisoner - escapee space of z = z^2 + c

#include <stdio.h>
#include <ctype.h>    // needed for isprint
#include <math.h>
#include <unistd.h>   // needed for getopt
#include "gdimage.h"
#ifdef _OPENMP
   //   what should get included here?
#endif
#include <string.h>   // needed for strncpy
#include <stdlib.h>

#define MAX_FILE_NAME 256

int main(int argc, char ** argv) {
    double xmin,xmax,ymin,ymax;
    int depth = 1000;
    double radius = 5.0;
    int i,j,k,c;
    int logscale =0;
    double xmid = -1.0;
    double ymid=0.0;
    double scale=1.5;
    int nx=1000;
    int ny=1000;
    int * image;
#ifdef _OPENMP
    double start_time,stop_time;
#endif
    char fname[MAX_FILE_NAME];


    strncpy(fname,"output.png",MAX_FILE_NAME);
    // get command line arguments using getopt
    while ((c = getopt (argc, argv, "x:y:s:d:w:h:f:l")) != -1)
    switch (c) {
      case 's':   //scale
        sscanf(optarg,"%lf",&scale);
        break;
      case 'x':   //center x
        sscanf(optarg,"%lf",&xmid);
        break;
      case 'y':   //center y
        sscanf(optarg,"%lf",&ymid);
        break;
      case 'd':   // depth
        sscanf(optarg,"%d",&depth);
        break;
      case 'w':   // width
        sscanf(optarg,"%d",&nx);
        break;
      case 'h':   //height
        sscanf(optarg,"%d",&ny);
        break;
      case 'l':   //logscale
        logscale=1;
        break;
      case 'f':   //filename
        strncpy(fname,optarg,MAX_FILE_NAME);
        break;
      case '?':   // error messages
        if (optopt == 'x' || optopt == 'X' ||
                optopt == 'y' || optopt == 'Y' || optopt == 'd')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr,
                   "Unknown option character `\\x%x'.\n",
                   optopt);
        return 1;
      default:
        abort ();
    }
    printf("USAGE: mandy -x (float) -y (float) -s (float) -d (integer) -l\n");
    printf("     -x (center x)\n     -y (center y)\n     -s (box size)\n");
    printf("     -d (depth)\n     -l (log scale display)\n");

    // echo input
    printf("x = %lf\n",xmid);
    printf("y = %lf\n",ymid);
    printf("s = %lf\n",scale);
    printf("d = %d\n",depth);
    printf("w = %d\n",nx);
    printf("h = %d\n",ny);

    xmin = xmid-scale;
    xmax = xmid+scale;
    ymin = ymid+scale;
    ymax = ymid-scale;
    image = (int *)malloc(sizeof(int)*nx*ny);


#ifdef _OPENMP
    // set a start for a timer
#endif
// set a pragma statement to parallelize the loop
    for(i=0;i<nx;i++) {
        for(j=0;j<ny;j++) { // for every pixel
              // determine pixels value on x-y plane
            double cx = xmin + (xmax-xmin)/(double)(nx-1)*i;
            double cy = ymin + (ymax-ymin)/(double)(ny-1)*j;
              // start iterative loop
            double x = cx;
            double y = cy;
            int done=0;
              // apply iteration, stop if escapee
            for(k=0;k<depth&&!done;k++) {
                double tx;
                tx = x*x-y*y + cx;
                y = 2*x*y + cy;
                x = tx;
                  // check for escape
                if(x*x+y*y>radius*radius) {
                      // set pixel value by time to escape
                    image[i*ny+j]=k;
                    done=1;
                }
            }
              // if no escape, set pixel value ot zero
            if(!done) image[i*ny+j]=0;
        }
    }
#ifdef _OPENMP
    // set a stop timer and print elapsed time
#endif

      // logscale to enhance image features
    if(logscale)
        for (i=0;i<nx;i++) 
            for (j=0;j<ny;j++)
                if (image[i*ny+j] != 0)
                    image[i*ny+j] = (int)(10.0*log((double)image[i*ny+j]));

      // output results
    drawImage(image,nx,ny,fname);

}
