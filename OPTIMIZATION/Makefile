CC        = gcc

LIBS      =  -lm

CFLAGS    = -O0
LDFLAGS   = $(LIBS)

PROGRAM1   = arrayfunc
SRCS1      = arrayfunc.c
OBJS1      = $(SRCS:.c=.o)		# object file

PROGRAM2   = arraystride
SRCS2      = arraystride.c
OBJS2      = $(SRCS:.c=.o)		# object file

PROGRAM3   = inlining
SRCS3      = inlining.c
OBJS3      = $(SRCS:.c=.o)		# object file

PROGRAM4   = loopcondition
SRCS4      = loopcondition.c
OBJS4      = $(SRCS:.c=.o)		# object file

PROGRAM5   = loopinvariance
SRCS5      = loopinvariance.c
OBJS5      = $(SRCS:.c=.o)		# object file

PROGRAM6   = strengthred
SRCS6      = strengthred.c
OBJS6      = $(SRCS:.c=.o)		# object file

PROGRAM7   = unitstride
SRCS7      = unitstride.c
OBJS7      = $(SRCS:.c=.o)		# object file

PROGRAM8   = plusplus
SRCS8      = plusplus.c
OBJS8      = $(SRCS:.c=.o)		# object file

.SUFFIXES: .c .o

default: all

all: $(PROGRAM1) $(PROGRAM2) $(PROGRAM3) $(PROGRAM4) $(PROGRAM5) $(PROGRAM6) $(PROGRAM7) $(PROGRAM8)

$(PROGRAM1): $(OBJS1) $(SRCS1)
	$(CC) -o $(PROGRAM1) $(SRCS1) $(CFLAGS) $(LDFLAGS)

$(PROGRAM2): $(OBJS2) $(SRCS2)
	$(CC) -o $(PROGRAM2) $(SRCS2) $(CFLAGS) $(LDFLAGS)

$(PROGRAM3): $(OBJS3) $(SRCS3)
	$(CC) -o $(PROGRAM3) $(SRCS3) $(CFLAGS) $(LDFLAGS)

$(PROGRAM4): $(OBJS4) $(SRCS4)
	$(CC) -o $(PROGRAM4) $(SRCS4) $(CFLAGS) $(LDFLAGS)

$(PROGRAM5): $(OBJS5) $(SRCS5)
	$(CC) -o $(PROGRAM5) $(SRCS5) $(CFLAGS) $(LDFLAGS)

$(PROGRAM6): $(OBJS6) $(SRCS6)
	$(CC) -o $(PROGRAM6) $(SRCS6) $(CFLAGS) $(LDFLAGS)

$(PROGRAM7): $(OBJS7) $(SRCS7)
	$(CC) -o $(PROGRAM7) $(SRCS7) $(CFLAGS) $(LDFLAGS)

$(PROGRAM8): $(OBJS8) $(SRCS8)
	$(CC) -o $(PROGRAM8) $(SRCS8) $(CFLAGS) $(LDFLAGS)

clean:
	/bin/rm -f $(OBJS1) $(PROGRAM1) $(OBJS2) $(PROGRAM2) $(OBJS3) $(PROGRAM3) $(OBJS4) $(PROGRAM4) $(OBJS5) $(PROGRAM5) $(OBJS6) $(PROGRAM6) $(OBJS7) $(PROGRAM7) $(PROGRAM8)
