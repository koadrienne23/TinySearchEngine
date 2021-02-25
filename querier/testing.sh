#!/bin/bash
#
# testing.sh
#
# Adrienne Ko, Feb 24 2021
#
# Description: a sequence of commands that demonstrate how 
# 'querier.c' works.
# 
# Usage: bash -v testing.sh &> testing.out


################# Test invalid command-line arguments #################

# incorrect number of arguments
./querier ../data/pageDirectory/ ../data/indexfile bad_test

# pageDirectory is not a path to an existing directory
./querier ../data/nonexistent/ ../data/indexfile

# pageDirectory is not path to a directory that was produced by the Crawler
./querier ../data/ ../data/indexfile

# indexFilename is not a path to a readable file
chmod -r ../data/indexfile
./querier ../data/pageDirectory/ ../data/indexfile


#################### Test invalid queries #####################

# valid command-line arguments, but invalid stdin queries
chmod +r ../data/indexfile
./querier ../data/pageDirectory/ ../data/indexfile < invalid_queries_test.txt


#################### Test valid queries #####################

# valid command-line arguments and valid stdin queries
./querier ../data/pageDirectory/ ../data/indexfile < valid_queries_test.txt
