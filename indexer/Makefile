# Makefile for both the "indexer" and "indextest" programs 
#
# Adrienne Ko, Feb 17 2021

L = ../libcs50
C = ../common

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$C -I$L
PROG1 = indexer
OBJS1 = indexer.o
PROG2 = indextest
OBJS2 = indextest.o
LIBS = -lm
LLIBS = $C/common.a $L/libcs50.a

.PHONY: all test valgrind clean

all: $(PROG1) $(PROG2)

# build "indexer"
$(PROG1): $(OBJS1) $(LLIBS)
	$(CC) $(CFLAGS) $(OBJS1) $(LLIBS) $(LIBS) -o $(PROG1)

# build "indextest"
$(PROG2): $(OBJS2) $(LLIBS)
	$(CC) $(CFLAGS) $(OBJS2) $(LLIBS) $(LIBS) -o $(PROG2)

# object files depend on include files
indexer.o: $L/counters.h $L/hashtable.h $L/webpage.h $L/file.h $C/pagedir.h $C/index.h $C/word.h
indextest.o: $L/hashtable.h $L/file.h $C/index.h

# expects a file `testing.sh` to exist.
test: 
	bash -v testing.sh $@

valgrind:
	valgrind ./$(PROG1) ../data/pageDirectory/ ../data/indexfile
	valgrind ./$(PROG2) ../data/indexfile ../data/indexcopy

clean:
	rm -f $(PROG1)
	rm -f $(PROG2)
	rm -f *~ *.o
	rm -rf *.dSYM
	rm -f ../data/indexfile
	rm -f ../data/indexcopy
	rm -f ../data/*.sorted