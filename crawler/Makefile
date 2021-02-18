# Makefile for the "crawler" program that uses the "bag", "hashtable",
#	"webpage", and "pagedir" modules.
#
# Adrienne Ko, Feb 17 2021

L = ../libcs50
C = ../common

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L -I$C
PROG = crawler
OBJS = crawler.o
LIBS = -lm
LLIBS = $L/libcs50.a $C/common.a

# executable depends on object files
$(PROG): $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $(OBJS) $(LLIBS) $(LIBS) -o $(PROG)

# object files depend on include files
crawler.o: $L/bag.h $L/hashtable.h $L/webpage.h $C/pagedir.h

.PHONY: all test valgrind clean

all: $(PROG)

# expects a file `testing.sh` to exist.
test: 
	mkdir -p ../data
	mkdir -p ../data/pageDirectory
	bash -v testing.sh $@

valgrind: $(PROG)
	mkdir -p ../data
	mkdir -p ../data/pageDirectory
	valgrind ./$(PROG) http://cs50tse.cs.dartmouth.edu/tse/letters/ ../data/pageDirectory/ 2

clean:
	rm -f $(PROG)
	rm -f *~ *.o
	rm -rf *.dSYM
	rm ../data/pageDirectory/*