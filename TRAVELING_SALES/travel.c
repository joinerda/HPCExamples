#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#ifdef WITH_X11
    #include "X-graph.h"
    #define has_x11 1
#else
    #define has_x11 0
#endif

#define TRUE 1
#define FALSE 0

typedef struct path {
    // number of places
    int n;
    // position of places
    double * x;
    double * y;
    // ordering of places
    int * o;
    int * o_best;
    double pathlength_best;
    // workspace
    int * stack;
    // simulated annealing
    int anneal;
    double temperature;
    double cooling_factor;
} Path;


void random_initByTime(int rank) {
    time_t ltime;

    time(&ltime);
    srand((unsigned) ltime + 100*rank);
    //srand(rank);
}

void alloc_path(Path * thePath,int n) {
    thePath->n=n;
    thePath->x = (double *)malloc(sizeof(double)*n);
    thePath->y = (double *)malloc(sizeof(double)*n);
    thePath->o = (int *)malloc(sizeof(int)*n);
    thePath->o_best = (int *)malloc(sizeof(int)*n);
    thePath->stack = (int *)malloc(sizeof(int)*n);
    thePath->anneal=TRUE;
    thePath->temperature=1;
    thePath->cooling_factor=0.999;
}

void free_path(Path * thePath) {
    free(thePath->x);
    free(thePath->y);
    free(thePath->o);
    free(thePath->o_best);
    free(thePath->stack);
}

double path_length(Path * thePath, int * o) {
    int i;
    double length;
    int num_steps = thePath->n-1;
    length=0.0;
    for(i=0;i<num_steps;i++) {
        length+=sqrt(pow((thePath->x[o[i]]-
            thePath->x[o[i+1]]),2)+
            pow((thePath->y[o[i]]-
            thePath->y[o[i+1]]),2));
    }

    return length;
}

void randomize_order(Path * thePath,int first_city,int last_city) {
    // pick a random starting point,
    // then go through remaining options until all are selected.

    // easiest choice, create a list, and pick randomly from list,
    // modifying list as one goes.
    int i,j;
    int pick;
    int nstack;

    nstack = last_city-first_city+1;
    for (i=0;i<nstack;i++) {
        thePath->stack[i]=thePath->o[first_city+i];
    }

    for(i=first_city;i<=last_city;i++) {
        //choose random city from stack;
        pick = (int)((double)nstack*(double)rand()/(double)RAND_MAX);
        thePath->o[i]=thePath->stack[pick];
        //remove city from stack and move everyone else up
        nstack--;
        for(j=pick;j<nstack;j++) {
            thePath->stack[j]=thePath->stack[j+1];
        }
    }
}

void reverse_order(Path * thePath,int first_city,int last_city) {
    // pick a random starting point,
    // then go through remaining options until all are selected.

    // easiest choice, create a list, and pick randomly from list,
    // modifying list as one goes.
    int i;
    int nstack;

    nstack = last_city-first_city+1;
    for (i=0;i<nstack;i++) {
        thePath->stack[nstack-i-1]=thePath->o[first_city+i];
    }

    for(i=first_city;i<=last_city;i++) {
        thePath->o[i]=thePath->stack[i-first_city];
    }
}

void random_swap(Path * thePath) {
    // pick two cities at random and swap them.

    int first_city;
    int second_city;
    int temp_o;

    first_city=1+
        (int)((double)(thePath->n-2)*(double)rand()/(double)RAND_MAX);
    second_city=1+
        (int)((double)(thePath->n-2)*(double)rand()/(double)RAND_MAX);
    while(second_city==first_city) {
        second_city=1+
            (int)((double)(thePath->n-2)*(double)rand()/(double)RAND_MAX);
    }

    temp_o = thePath->o[first_city];
    thePath->o[first_city]=thePath->o[second_city];
    thePath->o[second_city]=temp_o;
}

void random_swap2(Path * thePath) {
    // pick two cities at random and swap them.
    // the second city is after the first

    int first_city;
    int second_city;
    int temp_o;

    first_city=1+
        (int)((double)(thePath->n-3)*(double)rand()/(double)RAND_MAX);
    second_city=first_city+1;

    temp_o = thePath->o[first_city];
    thePath->o[first_city]=thePath->o[second_city];
    thePath->o[second_city]=temp_o;
}

void random_swap3(Path * thePath) {
    // pick one cities at random and move it to a random spot.

    int start;
    int end;
    int i;
    int temp_o;

    start=1+
        (int)((double)(thePath->n-3)*(double)rand()/(double)RAND_MAX);
    end=1+
        (int)((double)(thePath->n-2)*(double)rand()/(double)RAND_MAX);
    while(end==start) {
        end=1+
            (int)((double)(thePath->n-2)*(double)rand()/(double)RAND_MAX);
    }

    if(start<end) {
        //temp_o = thePath->o[start];
        //for(i=start;i<end;i++) {
            //thePath->o[i]=thePath->o[i+1];
        //}
        //thePath->o[end]=temp_o;
    } else {
        temp_o = thePath->o[start];
        for(i=start;i>end;i--) {
            thePath->o[i]=thePath->o[i-1];
        }
        thePath->o[end]=temp_o;
    }

}


void random_swap4(Path * thePath) {
    // pick two cities and reverse everything between them
    int first_city;
    int second_city;
    int temp;

    first_city=1+
        (int)((double)(thePath->n-2)*(double)rand()/(double)RAND_MAX);
    second_city=1+
        (int)((double)(thePath->n-2)*(double)rand()/(double)RAND_MAX);
    while(second_city==first_city) {
        second_city=1+
            (int)((double)(thePath->n-2)*(double)rand()/(double)RAND_MAX);
    }
    if(first_city>second_city) {
        temp=first_city;
        second_city=first_city;
        first_city=temp;
    }

    reverse_order(thePath,first_city,second_city);
}

void random_swap5(Path * thePath) {
    // pick two cities at random, the second
    // greater than the first, but cannot include
    // the first or last city, randomize between them

    int first_city;
    int second_city;
    int temp;

    first_city=1+
        (int)((double)(thePath->n-2)*(double)rand()/(double)RAND_MAX);
    second_city=1+
        (int)((double)(thePath->n-2)*(double)rand()/(double)RAND_MAX);
    while(second_city==first_city) {
        second_city=1+
            (int)((double)(thePath->n-2)*(double)rand()/(double)RAND_MAX);
    }
    if(first_city>second_city) {
        temp=first_city;
        second_city=first_city;
        first_city=temp;
    }

    randomize_order(thePath,first_city,second_city);
}

void random_swap6(Path * thePath) {
    //swap the endpoints

    int temp_o;
    temp_o = thePath->o[0];
    thePath->o[0]=thePath->o[thePath->n-1];
    thePath->o[thePath->n-1]=temp_o;
}


void regular_path(Path * thePath) {
    // create a path based on a regular polygon
    int i;
    double theta;
    double rad;
    double step;

    rad=5.0;
    step=6.28/(double)(thePath->n);
    
    for(i=0;i<thePath->n;i++) {
        theta=i*step;
        thePath->x[i] = rad*cos(theta);
        thePath->y[i] = rad*sin(theta);
    }
    // randomize order and reset best path
    for(i=0;i<thePath->n;i++) {
        thePath->o[i]=i;
    }
    randomize_order(thePath,1,thePath->n-2);
    for(i=0;i<thePath->n;i++) {
        thePath->o_best[i]=thePath->o[i];
    }
    thePath->pathlength_best=path_length(thePath,thePath->o_best);
}

void randomize_path(Path * thePath,int neighbors) {
    // create an initial random path
    int i;
    double plusminus;
    for(i=0;i<thePath->n;i++) {
        plusminus = (double)rand()/(double)RAND_MAX;
        thePath->x[i] = 4.0*((double)rand()/(double)RAND_MAX-0.5);
        if (neighbors>1) {
            if(plusminus<0.5) {
                thePath->x[i]+=5.0;
            } else {
                thePath->x[i]-=5.0;
            }
        }
        plusminus = (double)rand()/(double)RAND_MAX;
        thePath->y[i] = 4.0*((double)rand()/(double)RAND_MAX-0.5);
        if(neighbors>2) {
            if(plusminus<0.5) {
                thePath->y[i]+=5.0;
            } else {
                thePath->y[i]-=5.0;
            }
        }
    }
    // randomize order and reset best path
    for (i=0;i<thePath->n;i++) {
        thePath->o[i]=i;
        thePath->o_best[i]=thePath->o[i];
    }
    thePath->pathlength_best=path_length(thePath,thePath->o);
}

int check_length(Path * thePath) {
    // check path length and update
    double length,diff,test;
    int i;
    // use best length, if annealing maybe use worse length and cool
    length = path_length(thePath,thePath->o);
    if (length<thePath->pathlength_best) {
        thePath->pathlength_best=length;
        for(i=0;i<thePath->n;i++) {
            thePath->o_best[i]=thePath->o[i];
        }
        if(thePath->anneal) thePath->temperature *= thePath->cooling_factor;
        return TRUE;
    } else {
        if(thePath->anneal) {
            diff = fabs(length-thePath->pathlength_best);
            test = (double)rand()/(double)RAND_MAX;
            if(test<exp(-diff/thePath->temperature)) {
                thePath->pathlength_best=length;
                for(i=0;i<thePath->n;i++) {
                    thePath->o_best[i]=thePath->o[i];
                }
                thePath->temperature *= thePath->cooling_factor;
                return TRUE;
            } else {
                for(i=0;i<thePath->n;i++) {
                    thePath->o[i]=thePath->o_best[i];
                }
                thePath->temperature *= thePath->cooling_factor;
                return FALSE;
            }
        } else {
            for(i=0;i<thePath->n;i++) {
                thePath->o[i]=thePath->o_best[i];
            }
            return FALSE;
        }
    }
}

#ifdef WITH_X11
    void draw(xgraph * theGraph, Path * thePath,int is_random) {
        xgraphPre(theGraph);
        int i;
    
        // draw best path in white, if not random, draw correct path
        // in green
        XSetForeground(theGraph->dpy,theGraph->gc,theGraph->whiteColor);
        for(i=0;i<thePath->n-1;i++) {
            if((thePath->o_best[i+1]==thePath->o_best[i]+1 ||
                thePath->o_best[i+1]==thePath->o_best[i]-1) && !is_random) {
                XSetForeground(
                    theGraph->dpy,theGraph->gc,theGraph->greenPixel->pixel);
            }
            XDrawLine(theGraph->dpy,theGraph->buffer,theGraph->gc,
                xgraphXReal2Disp(theGraph,thePath->x[thePath->o_best[i]]),
                xgraphYReal2Disp(theGraph,thePath->y[thePath->o_best[i]]),
                xgraphXReal2Disp(theGraph,thePath->x[thePath->o_best[i+1]]),
                xgraphYReal2Disp(theGraph,thePath->y[thePath->o_best[i+1]]));
            if((thePath->o_best[i+1]==thePath->o_best[i]+1 ||
                thePath->o_best[i+1]==thePath->o_best[i]-1) && !is_random) {
                XSetForeground(theGraph->dpy,theGraph->gc,theGraph->whiteColor);
            }
        }
        // draw cities in red, end points are larger
        XSetForeground(theGraph->dpy,theGraph->gc,theGraph->redPixel->pixel);
        XFillRectangle(theGraph->dpy,theGraph->buffer,theGraph->gc,
            xgraphXReal2Disp(theGraph,thePath->x[0])-4,
            xgraphYReal2Disp(theGraph,thePath->y[0])-4,
            8,8);
        for(i=1;i<thePath->n-1;i++) {
            XFillRectangle(theGraph->dpy,theGraph->buffer,theGraph->gc,
                xgraphXReal2Disp(theGraph,thePath->x[i])-2,
                xgraphYReal2Disp(theGraph,thePath->y[i])-2,
                4,4);
        }
        XFillRectangle(theGraph->dpy,theGraph->buffer,theGraph->gc,
            xgraphXReal2Disp(theGraph,thePath->x[thePath->n-1])-4,
            xgraphYReal2Disp(theGraph,thePath->y[thePath->n-1])-4,
            8,8);
    
        xgraphPost(theGraph);
    }
#endif

int check_regular(Path * thePath,int * o) {
    // for regular polygons, check for convergence
    int i;
    int in_order=1;
    for(i=0;i<thePath->n-1&&in_order;i++) {
        if(thePath->o_best[i+1]==thePath->o_best[i]+1 ||
            thePath->o_best[i+1]==thePath->o_best[i]-1) {
        } else {
            in_order=0;
        }
    }
    return in_order;
}

int main(int argc, char ** argv) {
    // Traveling salesman path
    Path * thePath;
    // number of cities
    int n=500;
    // iteration max values
    int itmax_outer=100000;
    int itmax_inner=10;
    // temporary value for path length
    double length;
    // test value to choose a mutation of the path
    int random_type;
    // tests to apply simulated annealing to nodes when syncing the nodes
    int anneal_flag;
    double diff;
    double test;
    // variable to store whether path is random or regular
    int is_random;
    // variable to store type of neighbors, -1 is regular, 1 is
    // random block, 2 is two random blocks, 4 is 4 random blocks
    int neighbors=1;
    // convergence test for regular case
    int converged;
    // loop counters
    int i,j,k;

    // whether to do X display
    int do_display=1;
    #ifdef WITH_X11
        // X Variables
        xgraph * theGraph;
    #endif
    int draw_counter=0;
    int draw_repeat=10;
    // whether to print output to screen
    int do_print=0;

    // MPI variables
    MPI_Status status;
    int rank;
    int size;
    int * ibuffer;
    double * xybuffer;
    double dbuffer;

    // set up MPI variables
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    // pritn usage
    if(rank==0) {
        printf("Usage: travel n_cities n_neighbors i_outer i_inner temp cooling do_display do_print\n");
    }

    // start checking command line arguments
    if(argc>1) {
        sscanf(argv[1],"%d",&n);
    }
    if(argc>2) {
        sscanf(argv[2],"%d",&neighbors);
    }
    if(neighbors!=-1&&neighbors!=1&&neighbors!=2&&neighbors!=4) {
        neighbors=1;
        printf("Problem with # neighborhoods, set to one\n");
    }
    is_random=1;
    if(neighbors==-1) {
        is_random=0;
    }
    if(argc>3) {
        sscanf(argv[3],"%d",&itmax_outer);
    }
    if(argc>4) {
        sscanf(argv[4],"%d",&itmax_inner);
    }

    //allocate memory, allocate path before setting path variables
    thePath = (Path *)malloc(sizeof(Path));
    alloc_path(thePath,n);
    ibuffer = (int *)malloc(sizeof(int)*n);
    xybuffer = (double *)malloc(sizeof(double)*n*2);
    #ifdef WITH_X11
        theGraph = (xgraph*)malloc(sizeof(xgraph));
    #endif

    // continue reading command line arguments
    if(argc>5) {
        sscanf(argv[5],"%lf",&thePath->temperature);
    }
    if(argc>6) {
        sscanf(argv[6],"%lf",&thePath->cooling_factor);
    }
    if(argc>7) {
        sscanf(argv[7],"%d",&do_display);
    }
    if(argc>8) {
        sscanf(argv[8],"%d",&do_print);
    }

    #ifdef WITH_X11
    // setup x display
        if (rank==0&&do_display) {
            xgraphSetup(theGraph,600,600);
            xgraphSetXRange(theGraph,-10.0,10.0);
            xgraphSetYRange(theGraph,-10.0,10.0);
        }
    #endif

    // seed random number generator to zero so that all models
    // with the same number and configuration of cities will
    // have the same path
    srand(0);
    // set up initial path
    if (rank==0) {
        if(is_random) {
            randomize_path(thePath,neighbors);
        } else {
            regular_path(thePath);
        }
        for(j=0;j<thePath->n;j++) {
            xybuffer[j]=thePath->x[j];
            xybuffer[j+thePath->n]=thePath->y[j];
            ibuffer[j]=thePath->o[j];
        }
    }
    // Broadcast path to all nodes so that all nodes solve the
    // same problem
    MPI_Bcast(xybuffer,2*thePath->n,MPI_DOUBLE,0,MPI_COMM_WORLD);
    MPI_Bcast(ibuffer,thePath->n,MPI_DOUBLE,0,MPI_COMM_WORLD);
    if (rank!=0) {
        for(j=0;j<thePath->n;j++) {
            thePath->x[j]=xybuffer[j];
            thePath->y[j]=xybuffer[j+thePath->n];
            thePath->o[j]=ibuffer[j];
        }
    }
    for(j=0;j<thePath->n;j++) {
        thePath->o_best[j]=thePath->o[j];
    }
    thePath->pathlength_best=path_length(thePath,thePath->o_best);
    // really seed random number generator, randomize by time and rank
    random_initByTime(rank);


    // set convergenve variable
    converged=0;
    // outer iteration loop
    for(i=0;i<itmax_outer&&!converged;i++){
        // inner iteration loop
        for(j=0;j<itmax_inner;j++) {
            if(!thePath->anneal) {
                // Monte Carlo option, just try random paths
                randomize_order(thePath,1,thePath->n-2);
            } else {
                // Use multiple mutation options, chosen at random
                random_type = (int)(15.0*(double)rand()/(double)RAND_MAX);
                switch(random_type) {
                    case 14:
                        random_swap6(thePath);
    //swap the endpoints
                     break;
                    case 13:
                        random_swap5(thePath);
    // pick two cities at random, the second
    // greater than the first, but cannot include
    // the first or last city, randomize between them
                     break;
                    case 12:
                    case 11:
                    case 10:
                        random_swap4(thePath);
    // pick two cities and reverse everything between them
                     break;
                    case 8:
                    case 7:
                    case 6:
                        random_swap3(thePath);
    // pick one cities at random and move it to a random spot.
                     break;
                    case 5:
                    case 4:
                    case 3:
                        random_swap2(thePath);
    // pick two cities at random and swap them.
    // the second city is after the first
                     break;
                    case 2:
                    case 1:
                    case 0:
                    default:
                        random_swap(thePath);
    // pick two cities at random and swap them.
                     break;
                }
            }
            // check mutation and update pathlength
            check_length(thePath);
        }
        // compare nodes
        if(rank!=0) {
            // workers send values to root
            for(j=0;j<thePath->n;j++) ibuffer[j]=thePath->o_best[j];
            dbuffer = path_length(thePath,thePath->o_best);
            MPI_Send(&dbuffer,1,MPI_DOUBLE,0,100,MPI_COMM_WORLD);
            MPI_Send(ibuffer,thePath->n,MPI_INT,0,200,MPI_COMM_WORLD);
        } else {
            // root checks to see if anyone has a better solution
            for(k=1;k<size;k++) {
                MPI_Recv(&dbuffer,1,MPI_DOUBLE,k,100,MPI_COMM_WORLD,&status);
                MPI_Recv(ibuffer,thePath->n,MPI_INT,k,
                    200,MPI_COMM_WORLD,&status);
                // If annealing, allow for acceptance of worse solution
                diff = fabs(length-thePath->pathlength_best);
                test = (double)(size+1)/(double)size*
                    (double)rand()/(double)RAND_MAX;
                anneal_flag=0;
                if(test<exp(-diff/thePath->temperature)&&thePath->anneal) {
                    anneal_flag=1;
                }
                if(dbuffer<thePath->pathlength_best||anneal_flag) {
                    for(j=0;j<thePath->n;j++) {
                        thePath->o_best[j]=ibuffer[j];
                        thePath->o[j]=ibuffer[j];
                    }
                    thePath->pathlength_best=path_length(thePath,
                        thePath->o_best);
                    thePath->pathlength_best=dbuffer;
                }
            }
        }
        // Broadcast out best solution to all nodes
        if(rank==0) {
            for(j=0;j<thePath->n;j++) ibuffer[j]=thePath->o_best[j];
            dbuffer = thePath->pathlength_best;
        }
        MPI_Bcast(&dbuffer,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
        MPI_Bcast(ibuffer,thePath->n,MPI_INT,0,MPI_COMM_WORLD);
        if(rank!=0) {
            for(j=0;j<thePath->n;j++) thePath->o_best[j]=ibuffer[j];
            for(j=0;j<thePath->n;j++) thePath->o[j]=ibuffer[j];
            thePath->pathlength_best=path_length(thePath,ibuffer);
        }
        // for regular path, check for convergence
        if(!is_random) {
            if(check_regular(thePath,thePath->o_best)){
                printf("EXACT PATH FOUND\n");
                converged=1;
            }
        }
        // output display
        #ifdef WITH_X11
            if(rank==0&&do_display&&draw_counter++%draw_repeat==0) draw(theGraph,thePath,is_random);
        #else
            if(rank=0&&do_display&&draw_counter++%draw_repeat==0) {
                printf("#");
            }
        #endif
        if(rank==0&&do_print) {
            printf("ITER: %d LENGTH: %lf TEMP: %lf \n",i,
                thePath->pathlength_best,thePath->temperature);
        }
    }
    // final output
    if (rank==0) printf("FINAL LENGTH: %lf \n",thePath->pathlength_best);

    // free alocated memory
    free_path(thePath);
    free(thePath);
    #ifdef WITH_X11
        free(theGraph);
    #endif
    free(ibuffer);
    free(xybuffer);

    // close out MPI 
    MPI_Finalize();

    return 0;
}
