#!/bin/bash
#
# testing.sh
#
# Adrienne Ko, Feb 17 2021
#
# Description: a sequence of commands that demonstrate how 
# 'indexer.c' and 'indextest.c' work.
# 
# Usage: bash -v testing.sh &> testing.out


###################### Test "indexer" ######################

# incorrect number of arguments
./indexer ../data/pageDirectory/ ../data/indexfile bad_test

# pageDirectory is not a path to an existing directory
./indexer ../data/nonexistent/ ../data/indexfile

# pageDirectory is not path to a directory that was produced by the Crawler
./indexer ../data/ ../data/indexfile

# indexFilename is not a path to a writeable file
chmod -w ../data/indexfile
./indexer ../data/pageDirectory/ ../data/indexfile

# valid test
chmod +w ../data/indexfile
./indexer ../data/pageDirectory/ ../data/indexfile


###################### Test "indextest" ######################

# incorrect number of arguments
./indextest bad_test

# oldIndexFilename is not a path to a readable file
chmod -r ../data/indexfile
./indextest ../data/indexfile ../data/indexcopy

# newIndexFilename is not a path to a writeable file
chmod +r ../data/indexfile
touch ../data/indexcopy
chmod -w ../data/indexcopy
./indextest ../data/indexfile ../data/indexcopy

# valid test
chmod +w ../data/indexcopy
./indextest ../data/indexfile ../data/indexcopy

# compare the oldIndexFile and the newIndexFile; they should be identical after sorting
gawk -f indexsort.awk ../data/indexfile > ../data/indexfile.sorted
gawk -f indexsort.awk ../data/indexcopy > ../data/indexcopy.sorted

diff -s ../data/indexfile.sorted ../data/indexcopy.sorted

# print out the first few lines of the index file to ensure the format is correct
head ../data/indexfile.sorted

