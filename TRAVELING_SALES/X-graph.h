#ifdef WITH_X11
////////////////////////////////////////////
// X-graph
// Copyright 2004, David Joiner and
//   The Shodor Education Foundation, Inc.
////////////////////////////////////////////
#ifndef XGRAPH
#define XGRAPH

#include <X11/Xlib.h>
#include <assert.h>
#include <unistd.h>
#define NIL (0)
#include <stdlib.h>

typedef struct xgraph_mem {
    Display *dpy;
    int blackColor;
    int whiteColor;
    int redColor;
    int greenColor;
    int blueColor;
    int width,height;
    Window w;
    GC gc;
    Pixmap buffer;
    double xhigh;
    double xlow;
    double yhigh;
    double ylow;
    XColor * bluePixel;
    XColor * redPixel;
    XColor * greenPixel;
    Colormap theColormap;
} xgraph;



void xgraphSetup(xgraph * thegraph, int width_in, int height_in);
void xgraphPre(xgraph * thegraph);
void xgraphPost(xgraph * thegraph);
void xgraphSetXRange(xgraph * thegraph,double,double);
void xgraphSetYRange(xgraph * thegraph,double,double);
int xgraphXReal2Disp(xgraph * thegraph,double);
int xgraphYReal2Disp(xgraph * thegraph,double);

void xgraphDraw(xgraph * thegraph, int npoints, double xmin, double ymin,
    double xmax, double ymax, double * x, double * y);

#endif
#endif
