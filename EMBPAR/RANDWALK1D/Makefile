CC            = mpicc
CFLAGS	      = -Wall -O3 -msse3
LDFLAGS	      = -msse3  -o 
LIBS          = -lm

OBJS	      = rw.o\
                timer.o

PROGRAM		= rw

all:		$(PROGRAM)

$(PROGRAM):	$(OBJS)
		$(CC) $(OBJS) $(LDFLAGS) $(PROGRAM) $(LIBS)


clean:;		rm -f $(OBJS) $(PROGRAM)
