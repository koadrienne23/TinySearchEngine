/* 
 * index.h - header file for CS50 'index' module
 *
 * A suite of functions to help the indexer saving and 
 * loading index files.
 * Common between the Indexer, Querier, and indextest.
 *
 * Adrienne Ko, Feb 17 2021
 */

#ifndef __INDEX_H
#define __INDEX_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "hashtable.h"
#include "counters.h"
#include "file.h"

/**************** functions ****************/

/**************** index_load ****************/
/* Read the contents of an index file and load the data into the "index" datastructure.
 *
 * Caller provides:
 *   Valid pointer to a hashtable and a string for index filename path
 * Assumptions:
 *   1. wordindex has been allocated by caller
 *   2. wordindex is an empty hashtable
 *   3. indexfile is in the proper format
 * Caller is responsible for:
 *   Later freeing the "index" datastructure via index_delete().
 */
void index_load(hashtable_t *wordindex, const char *indexfile);

/**************** index_save ****************/
/* Write the inverted index to a file in the proper format.
 *
 * Caller provides:
 *   Valid pointer to a hashtable and a string for index filename path
 * Assumptions:
 *   1. wordindex has been allocated by caller
 *   2. wordindex pointer to a non-NULL hashtable
 *   3. indexfile is the pathname to a writeable file
 * Caller is responsible for:
 *   Later freeing the "index" datastructure via index_delete().
 */
void index_save(hashtable_t *wordindex, const char *indexfile);

/**************** index_delete ****************/
/* Delete the whole index datastructure.
 *
 * Caller provides:
 *   a valid pointer to a hashtable (the index datastructure)
 * We do:
 *   we ignore NULL hashtables.
 *   we free all memory we allocate for this hashtable, including
 *      the countersets.
 */
void index_delete(hashtable_t *wordindex);

#endif // __INDEX_H