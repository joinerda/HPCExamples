////////////////////////////////////////////
// MPI Life 0.9
// Copyright 2002, David Joiner and
//   The Shodor Education Foundation, Inc.
////////////////////////////////////////////

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Life.h"
#include "mpi.h"
#include <time.h>

#include <X11/Xlib.h> // Every Xlib program must include this
#include <assert.h>   // I include this to test return values the lazy way
#include <unistd.h>   // So we got the profile for 10 seconds
#define NIL (0)       // A name for the void pointer

#include "cart_tools.h"

int rank;
int size;
int ndims=2;
int dims[2];
int periods[2];
MPI_Status status;
MPI_Comm grid_world;
#define SHIFT_LEFT 50
#define SHIFT_RIGHT 51
#define SHIFT_UP 52
#define SHIFT_DOWN 53


#if !defined( min )
# define min(a,b) (a<b?a:b)
#endif
#if !defined( max )
# define max(a,b) (a>b?a:b)
#endif


int main(int argc, char ** argv) {

    double time_begin,time_end;
    int i,j;
    int count;
    bool done;
    // defaults
    int ngrid=105;
    int ncols=ngrid;
    int nrows=ngrid;
    int max_count=10000;
    int do_display=1;

    // Set up MPI
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    time_begin = MPI_Wtime();

    periods[0]=true;
    periods[1]=true;
    median_factorization(size,ndims,dims);

    MPI_Cart_create(MPI_COMM_WORLD,ndims,dims,
        periods,true,&grid_world);

    random_initByTime(rank);

    
    // command line arguments
    if (argc > 1) {
        sscanf(argv[1],"%d",&nrows);
    }
    if (argc > 2) {
        sscanf(argv[2],"%d",&ncols);
    }
    if (argc > 3) {
        sscanf(argv[3],"%d",&max_count);
    }
    if (argc > 4) {
        sscanf(argv[4],"%d",&do_display);
    }
    if (do_display!=0) do_display=1;

    //ncols=ncols/dims[0];
    //nrows=nrows/dims[1];

    printf("Rank %d: Running NCOLS=%d, NROWS=%d\n",rank,ncols,nrows);

    int ** grid;
    int ** next_grid;

    allocate_grid(ncols, nrows, &grid);
    allocate_grid(ncols, nrows, &next_grid);
    randomize_grid(ncols, nrows, grid, 0.25);
    if (do_display==1) 
        setupWindow(ncols, nrows);

    done=false;
    count=0;
    while(!done) {
        if (count++>max_count) done=true;
        // output
        if (count%1==0&&do_display==1) doDraw(rank,ncols,nrows,grid);
        do_step(rank,size,ncols,nrows,grid,next_grid);
        //doPrint(rank,ncols,nrows,grid);
        for (i=0;i<ncols+2;i++) {
            for (j=0;j<nrows+2;j++) {
                grid[i][j]=next_grid[i][j];
            }
        }
    }
    
    cleanup_grid(ncols, nrows, &grid);
    cleanup_grid(ncols, nrows, &next_grid);
    time_end = MPI_Wtime();
    printf("Rank %d: Elapsed time = %lf seconds\n",rank,(time_end-time_begin));
    MPI_Finalize();

}

void doPrint(int rank, int ncols, int nrows, int ** grid) {
    int coords[2];
    int i,j;
    MPI_Cart_coords(grid_world,rank,2,coords);
    printf("-- rank %d -(%d,%d)------\n",rank,coords[0],coords[1]);
    for (j=1;j<nrows+1;j++) {
        for (i=1;i<ncols+1;i++) {
            if (grid[i][j]) printf("x"); else printf(".");
        }
        printf("\n");
    }
    printf("----------------\n");
}

void do_step(int rank, int size, int ncols, int nrows, int ** grid,
        int ** next_grid) {
    int i,j,k,l;
    
    // top and bottom we get from current cell.
    //left right and corners we get from neighboring grids.
    // start off with non blocking sends of each "row"
    // left is rank - 1 % size, right is rank + 1 % size.
    int * send_buffer = (int *) calloc(max(nrows+2,ncols+2),sizeof(int));
    int * recv_buffer = (int *) calloc(max(nrows+2,ncols+2),sizeof(int));
    int source;
    int destination;
    int neighbors;
    
    //pass right
    MPI_Cart_shift(grid_world,0,1,&source,&destination);
    for (j=1;j<=nrows;j++) {
        send_buffer[j]=grid[ncols][j];
    }
    MPI_Sendrecv(send_buffer,nrows+2,MPI_INT,destination,SHIFT_RIGHT,
                 recv_buffer,nrows+2,MPI_INT,source,SHIFT_RIGHT,
                 grid_world,&status);
    for (j=1;j<=nrows;j++) {
        grid[0][j]=recv_buffer[j];
    }
    // pass left
    MPI_Cart_shift(grid_world,0,-1,&source,&destination);
    for (j=1;j<=nrows;j++) {
        send_buffer[j]=grid[1][j];
    }
    MPI_Sendrecv(send_buffer,nrows+2,MPI_INT,destination,SHIFT_LEFT,
                 recv_buffer,nrows+2,MPI_INT,source,SHIFT_LEFT,
                 grid_world,&status);
    for (j=1;j<=nrows;j++) {
        grid[ncols+1][j]=recv_buffer[j];
    }
    // pass up
    MPI_Cart_shift(grid_world,1,1,&source,&destination);
    for (i=0;i<=ncols+1;i++) {
        send_buffer[i]=grid[i][1];
    }
    MPI_Sendrecv(send_buffer,ncols+2,MPI_INT,destination,SHIFT_UP,
                 recv_buffer,ncols+2,MPI_INT,source,SHIFT_UP,
                 grid_world,&status);
    for (i=0;i<=ncols+1;i++) {
        grid[i][nrows+1]=recv_buffer[i];
    }
    // pass down
    MPI_Cart_shift(grid_world,1,-1,&source,&destination);
    for (i=0;i<=ncols+1;i++) {
        send_buffer[i]=grid[i][nrows];
    }
    MPI_Sendrecv(send_buffer,ncols+2,MPI_INT,destination,SHIFT_DOWN,
                 recv_buffer,ncols+2,MPI_INT,source,SHIFT_DOWN,
                 grid_world,&status);
    for (i=0;i<=ncols+1;i++) {
        grid[i][0]=recv_buffer[i];
    }



	//update
	for (i=1;i<=ncols;i++) {
		for (j=1;j<=nrows;j++) {
			neighbors=0;
			for (k=i-1;k<=i+1; k++) {
				for (l=j-1;l<=j+1; l++) {
					if (!(k==i&&l==j)&&grid[k][l]>0) {
						neighbors++;
					}
				}
				if (neighbors>3) continue;
			}
			if (neighbors<2||neighbors>3) {
				next_grid[i][j]=0;
			} else if (grid[i][j]>0||neighbors==3) {
				next_grid[i][j]=grid[i][j]+1;
			}
		}
	}
    free(send_buffer);
    free(recv_buffer);
}

void allocate_grid(int ncols, int nrows, int *** grid){
    int i,j;
    (*grid) = (int **) calloc(ncols+2,sizeof(int *));
    for (i=0; i<ncols+2;i++) {
        (*grid)[i] = (int*)calloc(nrows+2,sizeof(int));
        for (j=0;j<nrows+2;j++) {
            (*grid)[i][j]=0;
        }
    }
}
void cleanup_grid(int ncols, int nrows, int *** grid){
    int i;
    for (i=0;i<ncols+2;i++) {
        free((*grid)[i]);
    }
    free(*grid);
}
void randomize_grid(int ncols, int nrows, int ** grid, double prob){
    int i,j;
    for (i=1;i<=ncols;i++) {
        for (j=1;j<=nrows;j++) {
            if (rand_double()<prob) {
                grid[i][j]=1;
            }
        }
    }
}

double rand_double() {
    return (double)rand()/(double)RAND_MAX;
}



// X information, at some point this should be cleaned up so
// that it does not use global variables

// setupWindow modified from the tutorial on
// http://tronche.com/gui/x/xlib-tutorial/
// by Christophe Tronche


Display *dpy;
int blackColor;
int whiteColor;
Window w;
GC gc;
Pixmap buffer;
Colormap theColormap;
int numXGrayscale=10;
XColor Xgrayscale[10];
int IMAGE_WIDTH=LIFE_IMAGE_WIDTH;
int IMAGE_HEIGHT=LIFE_IMAGE_HEIGHT;

void setupWindow(int ncols, int nrows) {
      // Open the display
      int i;
      int color;

      dpy = XOpenDisplay(NIL);
      assert(dpy);

      // Get some colors

      blackColor = BlackPixel(dpy, DefaultScreen(dpy));
      whiteColor = WhitePixel(dpy, DefaultScreen(dpy));

      // Create the window
      if (nrows>ncols) {
         IMAGE_WIDTH = (int)((double)LIFE_IMAGE_WIDTH*(double)ncols/(double)nrows);
         IMAGE_HEIGHT = LIFE_IMAGE_HEIGHT;
      } else {
         IMAGE_HEIGHT = (int)((double)LIFE_IMAGE_HEIGHT*(double)nrows/(double)ncols);
         IMAGE_WIDTH = LIFE_IMAGE_WIDTH;
      }
      

      w = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0, 0, 
				     IMAGE_WIDTH, IMAGE_HEIGHT, 0, blackColor,
				     blackColor);
      buffer = XCreatePixmap(dpy,DefaultRootWindow(dpy),
          IMAGE_WIDTH,IMAGE_HEIGHT,DefaultDepth(dpy,
          DefaultScreen(dpy)));
          
      theColormap = XCreateColormap(dpy, DefaultRootWindow(dpy),
          DefaultVisual(dpy,DefaultScreen(dpy)), AllocNone);
          
      for (i=0;i<numXGrayscale;i++) {
          color = (int)((double)i*35535.0/(double)numXGrayscale)+30000;
          Xgrayscale[i].red=color;
          Xgrayscale[i].green=color;
          Xgrayscale[i].blue=color;
          XAllocColor(dpy,theColormap,&(Xgrayscale[i]));
      }

      // We want to get MapNotify events

      XSelectInput(dpy, w, StructureNotifyMask);

      // "Map" the window (that is, make it appear on the screen)

      XMapWindow(dpy, w);

      // Create a "Graphics Context"

      gc = XCreateGC(dpy, w, 0, NIL);

      // Tell the GC we draw using the white color

      XSetForeground(dpy, gc, whiteColor);

      // Wait for the MapNotify event

      for(;;) {
	    XEvent e;
	    XNextEvent(dpy, &e);
	    if (e.type == MapNotify)
		  break;
      }

}

void doDraw(int rank, int ncols, int nrows, int ** grid) {

    int i,j;
    int x1,x2,y1,y2; 
    char string[16];
    int coords[2];
    int rect_width=(int)((double)IMAGE_WIDTH/(double)(ncols+1));
    int rect_height=(int)((double)IMAGE_HEIGHT/(double)(nrows+1));
    int boxXMin;
    int boxYMin;
    int boxXMax;
    int boxYMax;

    MPI_Cart_coords(grid_world,rank,2,coords);
    sprintf(string,"%2d (%2d,%2d)",rank,coords[0],coords[1]);
    
    XSetForeground(dpy, gc, blackColor);
    XFillRectangle(dpy,buffer,gc,0,0,IMAGE_WIDTH,IMAGE_HEIGHT);
    for (i=1;i<=ncols;i++) {
        x1 = (int)((double)(i-1)/(double)(ncols+1)*(double)IMAGE_WIDTH);
        for (j=1;j<=nrows;j++) {
            y1 = (int)((double)(j-1)/(double)(nrows+1)*
                (double)IMAGE_HEIGHT);
            if (grid[i][j]>0) {
                int life =grid[i][j];
                if (life>numXGrayscale-1) life=numXGrayscale-1;
                XSetForeground(dpy, gc, Xgrayscale[life].pixel);
            } else {
                XSetForeground(dpy, gc, blackColor);
            }
            XFillRectangle(dpy,buffer,gc,x1,y1,rect_width,rect_height);
         }
     }
     boxXMin=10;
     boxYMin=10;
     boxXMax=boxXMin+6*strlen(string);
     boxYMax=15;
     XSetForeground(dpy,gc,blackColor);
     XFillRectangle(dpy,buffer,gc,boxXMin,boxYMin,boxXMax,boxYMax);
     XSetForeground(dpy,gc,whiteColor);
     XDrawRectangle(dpy,buffer,gc,boxXMin,boxYMin,boxXMax,boxYMax);
     XDrawString(dpy,buffer,gc,12,23,string,strlen(string));
     
     XCopyArea(dpy, buffer, w, gc, 0, 0,
         IMAGE_WIDTH, IMAGE_HEIGHT,  0, 0);
     XFlush(dpy);
	  
}

void random_initByTime(int rank) {
    time_t ltime;

    time(&ltime);
    srand((unsigned) ltime + 100*rank);
}
