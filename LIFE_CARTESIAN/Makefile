
#
# Laboratory for Scientific Computing
# http://www.lam-mpi.org/tutorials/
# University of Notre Dame
#
# MPI Tutorial
# Sample Makefile
#
# "mpicc" adds the directories for the include and lib files.  Hence,
# -I and -L for MPI stuff is not necessary
#

CC        = mpicc

#
# Modify TOPDIR if you use your own include files and library files
#

LIBS      = -lX11 -lm


CFLAGS    = 
LDFLAGS   = -L/usr/X11R6/lib  $(LIBS)

PROGRAM   = Life
SRCS      = Life.c cart_tools.c
OBJS      = Life.o cart_tools.o

#
# Rules
#

.SUFFIXES: .c .o

.cpp.o: 
	$(CC) -c $(CFLAGS) $<

#
# Targets
#

default: all

all: $(PROGRAM) 

$(PROGRAM): $(OBJS)
	$(CC) -o $(PROGRAM) $(SRCS) $(CFLAGS) $(LDFLAGS)

clean:
	/bin/rm -f $(OBJS) $(PROGRAM)

