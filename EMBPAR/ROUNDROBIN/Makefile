CC            = mpicc
CFLAGS	      = -Wall -O3 -msse3
LDFLAGS	      = -msse3  -o 
LIBS          = -lm

OBJS	      = rr.o

PROGRAM		= rr

all:		$(PROGRAM)

$(PROGRAM):	$(OBJS)
		$(CC) $(OBJS) $(LDFLAGS) $(PROGRAM) $(LIBS)


clean:;		rm -f $(OBJS) $(PROGRAM)
