/* 
 * crawler.c - CS50 'crawler' module
 *
 * crawls a website and retrieves webpages starting with a specified URL. 
 * It parses the initial webpage, extracts any embedded URLs and retrieves 
 * those pages, and crawls the pages found at those URLs, but limits itself 
 * to maxDepth hops from the seed URL and to URLs that are ‘internal’ to 
 * the designated CS50 server.
 *
 * Adrienne Ko, Feb 10 2021 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include "pagedir.h"
#include "bag.h"
#include "hashtable.h"
#include "webpage.h"

int crawler(const char *pathname, int maxDepth, bag_t *toExplore, hashtable_t *seen);

/**************** main ****************/
int main(const int argc, const char *argv[]) 
{
    if (argc == 4) {    // there are exactly three command-line arguments given
        char *seedURL = calloc(strlen(argv[1])+1, sizeof(char));
        const char *pathname;
        int maxDepth;
        const int hashSize = 20;

        // validate and parse the command line

        strcpy(seedURL, argv[1]);
        if (!IsInternalURL(seedURL)) {
        // seedURL does not begin with 'http://cs50tse.cs.dartmouth.edu/tse/'
            fprintf(stderr, "error: invalid seedURL given\n");
            exit(1);
        }

        pathname = argv[2];
        char *filename = malloc(strlen(pathname) + strlen("/.crawler") + 1);
        if (filename == NULL) {
            // error allocating memory for filename; return false
            fprintf(stderr, "error: no memory to allocate\n");
            exit(3);
        } else {
            filename = strcpy(filename, pathname);
            filename = strcat(filename, "/.crawler");
        }

        FILE* fp = fopen(filename, "w");
        if(fp == NULL) {
            // pathname is not a path to an existing directory
            fprintf(stderr, "error: invalid pathname given\n");
            exit(2);
        } 
        fclose(fp);
        free(filename);


        int check;  // keeps track of the number of floating-point digits in the argument
        if (sscanf(argv[3], "%d%n", &maxDepth, &check) != 1 || check != strlen(argv[3]) || maxDepth < 0 || maxDepth > 10) {
        // maxDepth is not an integer in the range [0..10]
            fprintf(stderr, "error: invalid maxDepth given\n");
            exit(3);
        }

        // initialize bag and hashtable modules
        bag_t *toExplore = bag_new();
        hashtable_t *seen = hashtable_new(hashSize);

        // make a webpage for the seedURL
        webpage_t* seedpage = webpage_new(seedURL, 0, NULL);
        bag_insert(toExplore, seedpage);
        hashtable_insert(seen, seedURL, "");

        // pass the parameters to the crawler function; exit non-zero if it fails
        if (crawler(pathname, maxDepth, toExplore, seen) != 0) {
            printf("crawler failed\n");
            exit(4);
        }
    } else {
        fprintf(stderr, "usage: ./crawler seedURL pageDirectory maxDepth; invalid arguments given\n");
        exit(5);
    }

    return 0;   // exit status
}

// return 0 if successful
int crawler(const char *pathname, int maxDepth, bag_t *toExplore, hashtable_t *seen) 
{
    int id = 1;     // initial id
    webpage_t* page;

    while ((page = bag_extract(toExplore)) != NULL) {
        int depth = webpage_getDepth(page);

        if (pagefetcher(page)) {
            if (pagesaver(page, pathname, id)) {
                id++;
            } else {   
                // error saving webpage / allocating memory
                bag_delete(toExplore, NULL);
                hashtable_delete(seen, NULL);
                return 1;
            }

            if (depth < maxDepth) {
                pagescanner(page, toExplore, seen);
            }
        }
        
        webpage_delete(page);
    }

    bag_delete(toExplore, NULL);
    hashtable_delete(seen, NULL);

    return 0;
}