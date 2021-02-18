# CS50 Tiny Search Engine (TSE) Crawler

The TSE crawler is a standalone program that crawls the web and retrieves webpages starting from a “seed” URL. It parses the seed webpage, extracts any embedded URLs, then retrieves each of those pages, recursively, but limiting its exploration to a given “depth”.

## Compilation

To compile `crawler.c`, run `make`. Run `make clean` to remove all files and directories produced by Make.

## Implementation

See `IMPLEMENTATION.md` for details of the implementation of the crawler.

## Assumptions

* The `pageDirectory` does not contain any files whose name is an integer (i.e., `1`, `2`, ...) before running a any test.
* `pageDirectory` does not exist before running `make test` or `make valgrind`.

No other assumptions beyond those that are clear from the spec.

## Files

* `crawler.c` - the crawler program
* `Makefile` - compilation procedure
* `IMPLEMENTATION.md` - the implementation spec
* `testing.sh` - test driver
* `testing.out` - result of `make test &> testing.out`

## Testing

The `testing.sh` bash script tests `crawler.c` for a variety of different cases. It tests the program with various forms of incorrect command-line arguments as well as many valid cases as well, to ensure that everything works correctly. It prints “progress” indicators from the crawler as it proceeds.

To test, simply `make test`. Make sure that `pageDirectory` does not exist beforehand.
See `testing.out` for details of testing and the output of an example test run.

To check for memory leaks with valgrind, run `make valgrind`.