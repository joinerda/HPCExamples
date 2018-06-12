CC            = mpicc
CFLAGS	      = -Wall -O3 -msse3 -DUSE_MPI
#CC            = gcc
#CFLAGS	      = -Wall -O3 -msse3
LDFLAGS	      = -msse3  -o 
LIBS          = -lm -lrt

OBJS	      = trap.o \
		timer.o

PROGRAM		= trap

all:		$(PROGRAM)

$(PROGRAM):	$(OBJS)
		$(CC) $(OBJS) $(LDFLAGS) $(PROGRAM) $(LIBS)


clean:;		rm -f $(OBJS) $(PROGRAM)
