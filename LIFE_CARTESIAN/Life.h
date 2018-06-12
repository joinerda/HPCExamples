////////////////////////////////////////////
// MPI Life 0.9
// Copyright 2002, David Joiner and
//   The Shodor Education Foundation, Inc.
////////////////////////////////////////////


#define LIFE_IMAGE_WIDTH 250
#define LIFE_IMAGE_HEIGHT 250

#define true 1
#define false 0
typedef int bool;

void allocate_grid(int ncols, int nrows, int *** grid);
void cleanup_grid(int ncols, int nrows, int *** grid);
void randomize_grid(int ncols, int nrows, int ** grid, double prob);
double rand_double();

void setupWindow(int , int);
void doDraw(int rank, int ncols, int nrows, int ** grid);
void doPrint(int rank, int ncols, int nrows, int ** grid);
void do_step(int rank,int size,int ncols,int nrows,int ** grid,int ** next_grid);


void random_initByTime(int rank) ;
