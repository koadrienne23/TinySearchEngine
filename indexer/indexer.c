/* 
 * indexer.c - CS50 'indexer' module
 *
 * Reads the document files produced by the TSE crawler, 
 * builds an index, and writes that index to a file.
 *
 * Adrienne Ko, Feb 17 2021 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pagedir.h"
#include "counters.h"
#include "hashtable.h"
#include "webpage.h"
#include "file.h"
#include "index.h"
#include "word.h"

int indexer(const char *pagedir, const char *indexfile, hashtable_t *wordindex);

/**************** main ****************/
int main(const int argc, const char *argv[]) 
{
    if (argc == 3) {    // there are exactly two command-line arguments given
        const char *pagedir = argv[1];
        const char *indexfile = argv[2];
        const int hashSize = 500;

        // validate and parse the second argument (pageDirectory)
        if (!isCrawlerDirectory(pagedir)) {
            // pagedir is not a path to an existing directory
            fprintf(stderr, "error: invalid pageDirectory given\n");
            exit(2);
        }

        // validate and parse the third argument (indexFilename)
        FILE* fp = fopen(indexfile, "w");
        if (fp == NULL) {
            // indexFilename is not a path to a writeable file
            fprintf(stderr, "error: invalid indexFilename given\n");
            exit(3);
        } 
        fclose(fp);
        
        // create/initialize index data structure (hashtable mapping word to counters)
        hashtable_t *wordindex = hashtable_new(hashSize);

        // pass the parameters to the indexer function; exit non-zero if it fails
        if (indexer(pagedir, indexfile, wordindex) != 0) {
            printf("error: indexer failed\n");
            exit(4);
        }
        
    } else {
        fprintf(stderr, "usage: ./indexer pageDirectory indexFilename; invalid arguments given\n");
        exit(1);
    }

    return 0;   // exit status
}

// return 0 if successful
int indexer(const char *pagedir, const char *indexfile, hashtable_t *wordindex) 
{
    int id = 1;                                 // keeps track of unique document ID within the pageDirectory
    char filename[2048];                        // filename of form pageDirectory/id
    sprintf(filename, "%s%d", pagedir, id);     // set initial filename (pageDirectory/1)

    FILE* fp;

    // loop through each document in pageDirectory
    while ((fp = fopen(filename, "r")) != NULL) {
        char *url = freadlinep(fp); 
        char *depth_str = freadlinep(fp);
        char *html = freadfilep(fp); 
        fclose(fp);
        
        int depth;
        sscanf(depth_str, "%d", &depth); 

        webpage_t* page = webpage_new(url, depth, html);
        
        int pos = 0;
        char *word;

        // loop through all the words in the webpage and update the hashtable accordingly
        while ((word = webpage_getNextWord(page, &pos)) != NULL) {
            if (strlen(word) >= 3) {
                NormalizeWord(word);
                counters_t *wordcount;
                if ((wordcount = hashtable_find(wordindex, word)) != NULL) {
                    counters_add(wordcount, id);
                } else {
                    wordcount = counters_new();
                    counters_add(wordcount, id);
                    hashtable_insert(wordindex, word, wordcount);
                }
            }
            free(word);
        }

        // free memory
        free(depth_str);
        webpage_delete(page);

        // update the document ID and filename
        id++;
        sprintf(filename, "%s%d", pagedir, id);
    }

    // read the index data structure and save it to indexfile
    index_save(wordindex, indexfile);

    // delete/free the index data structure
    index_delete(wordindex);

    return 0;
}