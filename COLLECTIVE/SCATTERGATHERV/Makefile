CC            = mpicc
CFLAGS	      = -Wall  -DUSE_MPI
LDFLAGS	      = -o  
LIBS          =  -lm 

###### DEBUG OPTIONS
#CFLAGS += -g
#CFLAGS += -pg -g

###### OPTIMIZATION OPTIONS
CFLAGS += -O3

OBJS	      = ScatterGatherV.o\
                BlockRange.o\

PROGRAM		= ScatterGatherV

all:		$(PROGRAM)

$(PROGRAM):	$(OBJS)
		$(CC) $(OBJS) $(LDFLAGS) $(PROGRAM) $(LIBS)


clean:;		rm -f $(OBJS) $(PROGRAM)
