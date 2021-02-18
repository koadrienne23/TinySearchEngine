/* 
 * indextest.c - CS50 'indextest' module
 *
 * Loads an index file produced by the indexer 
 * and saves it to another file.
 *
 * Adrienne Ko, Feb 17 2021 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "index.h"
#include "file.h"

/**************** main ****************/
int main(const int argc, const char *argv[]) 
{
    if (argc == 3) {    // there are exactly two command-line arguments given
        const char *oldIndexFile = argv[1];
        const char *newIndexFile = argv[2];

        FILE *fp;
        
        if ((fp = fopen(oldIndexFile, "r")) == NULL) {
            // oldIndexFilename is not a path to a readable file
            fprintf(stderr, "error: invalid oldIndexFilename given\n");
            exit(2);
        }
        int n = lines_in_file(fp);
        fclose(fp);

        if ((fp = fopen(newIndexFile, "w")) == NULL) {
            // newIndexFilename is not a path to a writeable file
            fprintf(stderr, "error: invalid newIndexFilename given\n");
            exit(3);
        }
        fclose(fp);

        // create/initialize index data structure (hashtable mapping word to counters)
        hashtable_t *wordindex = hashtable_new(n*2/3);

        index_load(wordindex, oldIndexFile);

        index_save(wordindex, newIndexFile);

        index_delete(wordindex);
        
    } else {
        fprintf(stderr, "usage: ./indextest oldIndexFilename newIndexFilename; invalid arguments given\n");
        exit(1);
    }

    return 0;   // exit status
}
