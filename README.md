# Adrienne Ko
## CS50 Winter 2021, Tiny Search Engine: Querier

GitHub username: koadrienne23

To build, run `make`.

To test Indexer, first run Crawler and keep all documents that it creates. 

To get the exact `testing.out` file provided in the `indexer` subdirectory, first run `make valgrind` or `./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ ../data/pageDirectory/ 2` within the `crawler` subdirectory, then run `make test &> testing.out` within the `indexer` subdirectory.

To get the exact `testing.out` file provided in the `querier` subdirectory, follow the steps detailed above, then run `make test &> testing.out` within the `querier` subdirectory.

To clean up, run `make clean`.
