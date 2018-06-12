////////////////////////////////////////////
// MPI FireDomDec 0.9
// Copyright 2006, David Joiner
////////////////////////////////////////////

#define LIFE_IMAGE_WIDTH 600
#define LIFE_IMAGE_HEIGHT 600

void allocate_grid(int ncols, int nrows, int *** grid);
void cleanup_grid(int ncols, int nrows, int *** grid);
void free_video();
void start_fire(int **,int,int,int,int);
void init_forest(int **,int,int);
double rand_double();

void setupWindow(int , int);
void moveWindow(int , int,int,int);
void doDraw(int rank, int ncols, int nrows, int ** grid);
void do_step(int rank,int size,int ncols,int nrows,int ** grid,int ** next_grid);

void random_initByTime(int rank) ;
