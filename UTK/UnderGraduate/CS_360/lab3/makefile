# CS360 Class Manager
# August 29, 2005
CC = gcc
CLASSDIR = /home/huangj/cs360
CDIR = /home/cs360
INCLUDES = $(CLASSDIR)/include
CFLAGS = -g -Wall -Werror -I$(INCLUDES)
#CFLAGS = -g -Wall -I$(INCLUDES)
LIBDIR = $(CDIR)/pub
LIBS = $(LIBDIR)/libfdr.a
EXECUTABLES = fakemake

all: $(EXECUTABLES)

clean:
	rm -f core $(EXECUTABLES) *.o

.SUFFIXES: .c .o
.c.o:
	$(CC) $(CFLAGS) -c $*.c

fakemake: fakemake.o
	$(CC) -g -o fakemake fakemake.o $(LIBS)
