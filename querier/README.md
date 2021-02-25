# CS50 Tiny Search Engine (TSE) Querier

The TSE Querier is a standalone program that reads the index file produced by the TSE Indexer, and page files produced by the TSE Querier, and answers search queries submitted via stdin.

## Compilation

To compile `querier.c`, run `make`. Run `make clean` to remove all files and directories produced by Make.

## Implementation

See `IMPLEMENTATION.md` for details of the implementation of the indexer.

## Assumptions

No other assumptions beyond those that are clear from the spec.

## Files

* `querier.c` - the querier program
* `Makefile` - compilation procedure
* `DESIGN.md` - the design spec
* `IMPLEMENTATION.md` - the implementation spec
* `testing.sh` - test driver
* `testing.out` - result of `make test &> testing.out`
* `invalid_queries_test.txt` - text file passed in for testing
* `valid_queries_test.txt` - text file passed in for testing

## Testing

The `testing.sh` bash script tests `querier.c` for a variety of different cases. It tests the program with various forms of incorrect command-line arguments as well as many valid cases as well, to ensure that everything works correctly. 

To test, simply `make test`.
See `testing.out` for details of testing and the output of an example test run.

To check for memory leaks with valgrind, run `make valgrind`.