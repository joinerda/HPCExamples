
CC        = mpicc

LIBS      = -lX11 -lm

CFLAGS    = -I/usr/X11R6/include -DWITH_X11
#CFLAGS    = -I/usr/X11R6/include
LDFLAGS   = -L/usr/X11R6/lib  $(LIBS)

PROGRAM   = travel			# name of the binary
SRCS      = travel.c X-graph.c
OBJS      = $(SRCS:.c=.o)		# object file

.SUFFIXES: .c .o

.c.o: 
	$(CC) -c $(CFLAGS) $<

default: all

all: $(PROGRAM) 

$(PROGRAM): $(OBJS)
	$(CC) -o $(PROGRAM) $(SRCS) $(CFLAGS) $(LDFLAGS)

clean:
	/bin/rm -f $(OBJS) $(PROGRAM)
