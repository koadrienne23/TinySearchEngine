# CS50 Tiny Search Engine (TSE) Indexer

The TSE indexer is a standalone program that reads the document files produced by the TSE crawler, builds an index, and writes that index to a file. Its companion, the index tester, loads an index file produced by the indexer and saves it to another file.

## Compilation

To compile `indexer.c` and `indextest`, run `make`. Run `make clean` to remove all files and directories produced by Make.

## Implementation

See `IMPLEMENTATION.md` for details of the implementation of the indexer.

## Assumptions

No other assumptions beyond those that are clear from the spec.

## Files

* `indexer.c` - the indexer program
* `indextest.c` - the indextest program
* `Makefile` - compilation procedure
* `DESIGN.md` - the design spec
* `IMPLEMENTATION.md` - the implementation spec
* `testing.sh` - test driver
* `testing.out` - result of `make test &> testing.out`

## Testing

The `testing.sh` bash script tests `indexer.c` and `indextest.c` for a variety of different cases. It tests the program with various forms of incorrect command-line arguments as well as many valid cases as well, to ensure that everything works correctly. 

To test, simply `make test`.
See `testing.out` for details of testing and the output of an example test run.

To check for memory leaks with valgrind, run `make valgrind`.