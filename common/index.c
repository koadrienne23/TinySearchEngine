/* 
 * index.c - CS50 'index' module
 *
 * see index.h for more information.
 *
 * Adrienne Ko, Feb 17 2021
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "hashtable.h"
#include "counters.h"
#include "file.h"

/**************** local functions ****************/
/* not visible outside this file */
static void save_word(FILE* fp, const char *word, counters_t *wordcount);
static void save_counts(FILE* fp, const int id, const int count);
static void countersdelete(counters_t *wordcount);


/**************** index_load ****************/
void
index_load(hashtable_t *wordindex, const char *indexfile)
{
    FILE* fp = fopen(indexfile, "r");

    char *word;
    while((word = freadwordp(fp)) != NULL) {
        int id;
        int count;

        // loop pulling off one pair (id, count) at a time
        while(fscanf(fp, " %d %d", &id, &count) == 2) {
            counters_t *wordcount;
            if ((wordcount = hashtable_find(wordindex, word)) != NULL) {
                counters_set(wordcount, id, count);
            } else {
                wordcount = counters_new();
                counters_set(wordcount, id, count);
                hashtable_insert(wordindex, word, wordcount);
            }
        }
        free(word);
    }

    fclose(fp);
}

/**************** index_save ****************/
void
index_save(hashtable_t *wordindex, const char *indexfile)
{
    FILE* fp = fopen(indexfile, "w");
    
    hashtable_iterate(wordindex, fp, save_word);

    fclose(fp);
}

/**************** save_word ****************/
/* Write given word to the index file */
static void save_word(FILE* fp, const char *word, counters_t *wordcount)
{
    fputs(word, fp);
    counters_iterate(wordcount, fp, save_counts);
    fputc('\n', fp);
}

/**************** save_counts ****************/
/* Write given (id, count) pair to the index file */
static void save_counts(FILE* fp, const int id, const int count)
{
    int len_id = snprintf(NULL, 0, "%d", id);
    char *str_id = calloc(len_id + 1, sizeof(char));
    if (str_id == NULL) {
        // error allocating memory for str_id; exit non-zero
        printf("error");
    } else {
        snprintf(str_id, len_id + 1, "%d", id);
    }

    int len_count = snprintf(NULL, 0, "%d", count);
    char *str_count = calloc(len_count + 1, sizeof(char));
    if (str_count == NULL) {
        // error allocating memory for str_count; exit non-zero
        printf("error");
    } else {
        snprintf(str_count, len_count + 1, "%d", count);
    }

    fprintf(fp, " %s", str_id);
    fprintf(fp, " %s", str_count);

    free(str_id);
    free(str_count);
}

/**************** index_delete ****************/
void
index_delete(hashtable_t *wordindex)
{
    hashtable_delete(wordindex, countersdelete);
}

/**************** countersdelete ****************/
/* Delete the given counterset */
static void countersdelete(counters_t *wordcount)
{
    if (wordcount != NULL) {
        counters_delete(wordcount); 
    }
}