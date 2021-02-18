# Makefile for the "common" library.
# 	Builds and compiles all these utility functions into a library (common.a).
#
# Adrienne Ko, Feb 17 2021

L = ../libcs50

OBJS = pagedir.o index.o word.o $L/hashtable.o $L/bag.o $L/counters.o $L/webpage.o
LIB = common.a
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L
CC = gcc
MAKE = make

# Build the library by archiving object files
$(LIB): $(OBJS)
		ar cr $(LIB) $(OBJS)

# Dependencies: object files depend on header files
pagedir.o: pagedir.h $L/hashtable.h $L/bag.h $L/counters.h $L/webpage.h
index.o: index.h $L/hashtable.h $L/counters.h $L/file.h
word.o: word.h
hashtable.o: $L/hashtable.h $L/set.h $L/jhash.h
bag.o: $L/bag.h
counters.o: $L/counters.h
webpage.o: $L/webpage.h

.PHONY: clean

# clean up after our compilation
clean:
	rm -f core
	rm -f $(LIB) *~ *.o
