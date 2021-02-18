#!/bin/bash
#
# testing.sh
#
# Adrienne Ko, Feb 10 2021
#
# Description: a sequence of commands that demonstrate how 'crawler.c' works
# Usage: bash -v testing.sh &> testing.out



############### Test with incorrect command-line arguments #################

# incorrect number of arguments
./crawler http://cs50tse.cs.dartmouth.edu/tse/ 3

# seedURL that points to a non-existent server
./crawler http://cs50tse.cs.dart ../data/pageDirectory 1

# seedURL that points to a non-internal server
./crawler https://en.wikipedia.org/wiki/Breadth-first_search ../data/pageDirectory 1

# seedURL that points to a valid server but non-existent page
./crawler http://cs50tse.cs.dartmouth.edu/tse/non-existent ../data/pageDirectory 1

# pageDirectory is the pathname to a non-existent directory
./crawler http://cs50tse.cs.dartmouth.edu/tse/ ../directory 1

# maxDepth is out of range (<0)
./crawler http://cs50tse.cs.dartmouth.edu/tse/ ../data/pageDirectory -2

# maxDepth is out of range (>10)
./crawler http://cs50tse.cs.dartmouth.edu/tse/ ../data/pageDirectory 11



############ Crawl a simple, closed set of cross-linked web pages (letters) ############

# maxDepth set to 0; expected # of pages saved is 1
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/pageDirectory 0

# maxDepth set to 1; expected # of pages saved is 2
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/pageDirectory 1

# maxDepth set to 2; expected # of pages saved is 3
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/pageDirectory 2

# maxDepth set to 3; expected # of pages saved is 6
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/pageDirectory 3

# maxDepth set to 4; expected # of pages saved is 8
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/pageDirectory 4

# maxDepth set to 5; expected # of pages saved is 9
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/pageDirectory 5

# maxDepth set to 5, start at page C; expected # of pages saved is still 9
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/pageDirectory 5

# print the first two lines of the most recently created files
cd ../data/pageDirectory
for ((i=1;i<=9;i++)); do
    head -2 $i
done

cd ../../crawler



############ Crawl http://cs50tse.cs.dartmouth.edu/tse/ ############

# maxDepth set to 0; expected # of pages saved is 1
./crawler http://cs50tse.cs.dartmouth.edu/tse/ ../data/pageDirectory 0

# maxDepth set to 1; expected # of pages saved is 4
./crawler http://cs50tse.cs.dartmouth.edu/tse/ ../data/pageDirectory 1

# # maxDepth set to 2
# ./crawler http://cs50tse.cs.dartmouth.edu/tse/ ../data/pageDirectory 2



############ Crawl http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ ############

# maxDepth set to 0; expected # of pages saved is 1
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ ../data/pageDirectory 0

# maxDepth set to 1; expected # of pages saved is 7
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ ../data/pageDirectory 1

# # maxDepth set to 2
# ./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ ../data/pageDirectory 2



############ Crawl http://cs50tse.cs.dartmouth.edu/tse/toscrape/ ############

# maxDepth set to 0; expected # of pages saved is 1
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/ ../data/pageDirectory 0

# maxDepth set to 1
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/ ../data/pageDirectory 1

# # maxDepth set to 2
# ./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/ ../data/pageDirectory 2
