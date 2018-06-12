CC            = mpicc
CFLAGS	      = -Wall -O3 -msse3
LDFLAGS	      = -msse3  -o 
LIBS          = -lm

###### LIBGD OPTIONS
CFLAGS += -I/share/apps/libgd/include
LIBS += -L/share/apps/libgd/lib/ -ljpeg -lpng -lz -lgd

OBJS	      = rr.o\
                 timer.o

PROGRAM		= rr

all:		$(PROGRAM)

$(PROGRAM):	$(OBJS)
		$(CC) $(OBJS) $(LDFLAGS) $(PROGRAM) $(LIBS)


clean:;		rm -f $(OBJS) $(PROGRAM)
