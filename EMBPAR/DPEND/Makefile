CC            = mpicc
CFLAGS	      = -Wall -g -msse3
LDFLAGS	      = -msse3  -o 
LIBS          = -Wl,-rpath,${LD_LIBRARY_PATH} -lm 

###### LIBGD OPTIONS
CFLAGS += -I/share/apps/libgd/include
LIBS += -L/share/apps/libgd/lib/ -ljpeg -lpng -lz -lgd

###### LIBGSL OPTIONS
CFLAGS += -I/share/apps/gsl/gnu/include
LIBS += -L/share/apps/gsl/gnu/lib/ -lgsl -lcblas -latlas

OBJS	      = dpend.o\
                timer.o

PROGRAM		= dpend

all:		$(PROGRAM)

$(PROGRAM):	$(OBJS)
		$(CC) $(OBJS) $(LDFLAGS) $(PROGRAM) $(LIBS)


clean:;		rm -f $(OBJS) $(PROGRAM)
