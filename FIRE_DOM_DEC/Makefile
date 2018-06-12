
CC        = mpicc

LIBS      = -L/usr/X11R6/lib64 -lX11 -lm

CFLAGS    = 
LDFLAGS   = $(LIBS)

PROGRAM   = FireDomDec			# name of the binary
SRCS      = FireDomDec.c 
OBJS      = $(SRCS:.c=.o)		# object file

.SUFFIXES: .c .o

.cpp.o: 
	$(CC) -c $(CFLAGS) $<

default: all

all: $(PROGRAM) 

$(PROGRAM): $(OBJS)
	$(CC) -o $(PROGRAM) $(SRCS) $(CFLAGS) $(LDFLAGS)

clean:
	/bin/rm -f $(OBJS) $(PROGRAM)
