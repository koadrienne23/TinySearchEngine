# Makefile for the "querier" program
#
# Adrienne Ko, Feb 24 2021

L = ../libcs50
C = ../common

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$C -I$L
PROG = querier
OBJS = querier.o
LIBS = -lm
LLIBS = $C/common.a $L/libcs50.a

.PHONY: all test valgrind clean

all: $(PROG)

# build "querier"
$(PROG): $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $(OBJS) $(LLIBS) $(LIBS) -o $(PROG)

# object files depend on include files
querier.o: $L/counters.h $L/hashtable.h $L/webpage.h $L/file.h $C/pagedir.h $C/index.h $C/word.h

# expects a file `testing.sh` to exist.
test: 
	bash -v testing.sh $@

valgrind:
	valgrind ./$(PROG) ../data/pageDirectory/ ../data/indexfile

clean:
	rm -f $(PROG)
	rm -f *~ *.o
	rm -rf *.dSYM