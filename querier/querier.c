/* 
 * querier.c - CS50 'querier' module
 *
 * Reads the index file produced by the TSE Indexer, and page 
 * files produced by the TSE Querier, and answers search queries 
 * submitted via stdin.
 *
 * Adrienne Ko, Feb 24 2021 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "pagedir.h"
#include "counters.h"
#include "hashtable.h"
#include "file.h"
#include "index.h"
#include "word.h"

/**************** local types ****************/
typedef struct twocounters { 
    counters_t *counters1, *counters2; 
} twocounters_t;

typedef struct id_score { 
    int docID, score; 
} id_score_t;

typedef struct arr_sort { 
    int index; 
    id_score_t **array; 
} arr_sort_t;


/**************** function declarations ****************/
int clean_query(char *input);
int parse_query(char *input, char *words[], int size);
counters_t *combine(char *words[], hashtable_t *wordindex, int size);
void rank_results(counters_t *results, const char *pagedir);

twocounters_t *twocounters_new(counters_t *counters1, counters_t *counters2);
id_score_t *id_score_new(int docID, int score);

/**************** local functions ****************/
static void intersect(counters_t *countersA, counters_t *countersB, counters_t *countersC);
static void intersect_helper(void *arg, const int key, const int count);
static void merge(counters_t *countersA, counters_t *countersB);
static void merge_helper(void *arg, const int key, const int count);
static void *get_counter(twocounters_t *counters, const int type);
static void copy_counter(counters_t *copy, counters_t *orig);
static void copy_helper(void *arg, const int key, const int count);
static void find_num_docs(void *arg, const int key, const int count);
static void sort_docs(void *arg, const int key, const int count);
static void *get_index(arr_sort_t *to_sort);
static void *get_array(arr_sort_t *to_sort);
static void prompt(void);

int fileno(FILE *stream);


/**************** main ****************/
int main(const int argc, const char *argv[]) 
{
    if (argc == 3) {    // there are exactly two command-line arguments given
        const char *pagedir = argv[1];
        const char *indexfile = argv[2];

        // validate and parse the second argument (pageDirectory)
        if (!isCrawlerDirectory(pagedir)) {
            // pagedir is not a path to an existing directory
            fprintf(stderr, "error: invalid pageDirectory given\n");
            exit(2);
        }

        // validate and parse the third argument (indexFilename)
        FILE* fp = fopen(indexfile, "r");
        if (fp == NULL) {
            // indexFilename is not a path to a readeable file
            fprintf(stderr, "error: invalid indexFilename given\n");
            exit(3);
        } 
        int n = lines_in_file(fp);
        fclose(fp);

        // read search queries from stdin, one per line, until EOF
        prompt();

        char *line;
        while ((line = freadlinep(stdin)) != NULL) {
            // load the index from indexFilename into an internal data structure
            hashtable_t *wordindex = hashtable_new(n*2/3);
            index_load(wordindex, indexfile);

            // clean/normalize the line and find the number of words
            int query_size = clean_query(line);
            if (query_size <= 0) {
                index_delete(wordindex);
                free(line);
                prompt();
                continue;
            }

            // validate and parse the query into an array of words
            char *words[query_size];
            if (parse_query(line, words, query_size) != 0) {
                fprintf(stderr, "invalid query\n");
                index_delete(wordindex);
                free(line);
                prompt();
                continue;
            }

            // display cleaned query
            printf("Query: ");
            for (int i = 0; i < query_size; i++) {
                printf("%s ", words[i]);
            }
            printf("\n");

            // create a new counterset with all of the documents containing the target words
            counters_t *results = combine(words, wordindex, query_size);

            // sort the documents in decreasing order by score, and print to stdout
            rank_results(results, pagedir);

            // free memory
            counters_delete(results);
            index_delete(wordindex);
            free(line);

            printf("\n");
            prompt();
        }
        
    } else {
        fprintf(stderr, "usage: ./querier pageDirectory indexFilename; invalid arguments given\n");
        exit(1);
    }

    return 0;   // exit status
}

/**************** clean_query ****************/
/* Checks for non-alphabet and non-space characters. If they exist,
 * returns -1. Otherwise, normalizes all of the words and returns
 * the number of words in the query.
 *
 * Caller provides:
 *   valid string input
 *
 * We return:
 *   -1 if there is a bad character
 *   the number of words, if it is a valid string input
 */
int clean_query(char *input)
{
    int size = 0;
    bool was_space = true;

    for (char *word = input; *word != '\0'; word++) {
        if (isalpha(*word) != 0) {   // is a letter
            if (was_space) {
                size++;
                was_space = false;
            }
        } else {    // not a letter
            if (isspace(*word) == 0 && *word!='\0') {    // not a space
                fprintf(stderr, "bad character '%c' in query.\n", *word);
                return -1;
            }
            was_space = true;
        }
    }

    NormalizeWord(input);
    return size;
}

/**************** parse_query ****************/
/* Creates an array of words and validates the array. That is, 
 * it ensures that the literal ‘and’ and the literal ‘or’ do 
 * not appear at the beginning or end of a query, and that the 
 * the literals (‘and’ & ‘or’) are not adjacent
 *
 * Caller provides:
 *   valid string input, empty arry of strings, integer indicating
 *   the number of words in the string
 *
 * We return:
 *   0 if terminates successfully
 *   non-zer0 (1) if string input is not valid
 */
int parse_query(char *input, char *words[], int size)
{
    int index = 0;

    // loop through each character i
    for (char *word = input; *word != '\0'; word++) {
        if (isalpha(*word) != 0) {   // is a letter
            char *rest = word+1;
            while (isalpha(*rest) != 0) {
                rest++;
            }
            *rest = '\0';

            words[index] = word;
            index++;
            word = rest;
        }
    }

    // validate the array
    if (strcmp(words[0], "and") == 0 || strcmp(words[0], "or") == 0 
        || strcmp(words[size-1], "and") == 0 || strcmp(words[size-1], "or") == 0) 
    {
        return 1;
    }

    for (int i = 0; i < size-1; i++) {
        if ((strcmp(words[i], "and") == 0 || strcmp(words[i], "or") == 0) 
            && (strcmp(words[i+1], "and") == 0 || strcmp(words[i+1], "or") == 0))
        {
            return 1;
        }
    }

    return 0;
}


/**************** combine ****************/
/* Creates and returns a counterset (docID, count) of all of the documents 
 * in the index data structure that contain the appropriate words from the 
 * query.
 *
 * Caller provides:
 *   array of strings, valid hashtable pointer, integer indicating
 *   the size of the array (number of words)
 *
 * We return:
 *   a counterset of (docID, score)
 */
counters_t *combine(char *words[], hashtable_t *wordindex, int size)
{
    counters_t *result = counters_new();
    counters_t *temp;
    bool first = true;

    if (size > 0) {     // not an empty array of words
        temp = hashtable_find(wordindex, words[0]);
    }

    for (int i = 1; i < size; ) {
        if (strcmp(words[i], "or") == 0) {
            // result = union between result and temp
            merge(result, temp);

            if (!first)
                counters_delete(temp);
            temp = counters_new();

            counters_t *next;
            if ((next = hashtable_find(wordindex, words[i+1])) != NULL) {
                copy_counter(temp, next);
            }

            i+=2;
            first = false;
        } else {
            counters_t *intersection = counters_new();
            if (strcmp(words[i], "and") == 0) {
                // temp = intersection between temp and words[i+1]
                intersect(temp, hashtable_find(wordindex, words[i+1]), intersection);
            } else {
                // temp = intersection between temp and words[i]
                intersect(temp, hashtable_find(wordindex, words[i]), intersection);
            }

            if (!first)
                counters_delete(temp);
            temp = counters_new();

            copy_counter(temp, intersection);
            counters_delete(intersection);
            
            if (strcmp(words[i], "and") == 0) {
                i+=2;
            } else {
                i++;
            }
            first = false;
        }
    }

    // result = union between result and temp
    merge(result, temp);

    // counters_print(result, stdout);
    // printf("\n");

    if (size > 1)
        counters_delete(temp);
    // free(temp);

    return result;
}

/**************** intersect ****************/
/* Finds the intersections between two countersets and saves it
 * to a different counterset
 */
static void intersect(counters_t *temp, counters_t *other, counters_t *intersection)
{
    twocounters_t counters = { intersection, temp };
    counters_iterate(other, &counters, intersect_helper);
}

static void intersect_helper(void *arg, const int key, const int count)
{
    counters_t *intersection = get_counter(arg, 0);
    counters_t *temp = get_counter(arg, 1);
    int other_count = count;
    
    // find the same key in the result counterset
    int temp_count = counters_get(temp, key);

    if (temp_count != 0) {
        // found: add key to result
        if (temp_count < other_count) {
            counters_set(intersection, key, temp_count);
        } else {
            counters_set(intersection, key, other_count);
        }
    }
}

/**************** merge ****************/
/* Finds the union between two countersets and saves it
 * to the first
 */
static void merge(counters_t *result, counters_t *other)
{
    counters_iterate(other, result, merge_helper);
}

static void merge_helper(void *arg, const int key, const int count)
{
    counters_t *result = arg;
    int other_count = count;
    
    // find the same key in the result counterset
    int result_count = counters_get(result, key);
    if (result_count == 0) {
        // not found: insert it
        counters_set(result, key, other_count);
    } else {
        // add to the existing value
        counters_set(result, key, result_count + other_count);
    }
}

/**************** get_counter ****************/
/* Returns the variable values of a twocounters_t object 
*/
static void *get_counter(twocounters_t *counters, const int type)
{
    if (type==0) {
        return counters->counters1;
    } else {
        return counters->counters2;
    }
}

/**************** copy_counter ****************/
/* Copies the contents of one counterset to another
*/
static void copy_counter(counters_t *copy, counters_t *orig) 
{
    counters_iterate(orig, copy, copy_helper);
}

static void copy_helper(void *arg, const int key, const int count) 
{
    counters_t *copy = arg;
    counters_set(copy, key, count);
}

/**************** rank_results ****************/
/* Sorts the documents in decreasing order by the score, 
 * then displays in to the user on the screen in order.
 *
 * Caller provides:
 *   valid counterset pointer containing all the different
 *   documents, string representing path to an existing 
 *   crawler-produced directory
 */
void rank_results(counters_t *results, const char *pagedir)
{
    // count the number of items
    int num = 0;
    counters_iterate(results, &num, find_num_docs);

    if (num == 0) {
        printf("No documents match.\n");
        return;
    }

    // create a sorted array of the documents
    int index = 0;
    id_score_t **sorted = calloc(num, sizeof(id_score_t*));
    arr_sort_t to_sort = { index, sorted };
    counters_iterate(results, &to_sort, sort_docs);

    // print out documents in order
    printf("Matches %d documents (ranked):\n", num);
    for (int i = 0; i < num; i++) {
        int score = sorted[i]->score;
        int id = sorted[i]->docID;

        int len_id = snprintf(NULL, 0, "%d", id);
        char *str_id = malloc(len_id + 1);
        if (str_id == NULL) {
            return;
        } else {
            snprintf(str_id, len_id + 1, "%d", id);
        }

        char *filename = malloc(strlen(pagedir) + strlen(str_id) + 2);
        if (filename == NULL) {
            return;
        } else {
            filename = strcpy(filename, pagedir);
            filename = strcat(filename, "/");
            filename = strcat(filename, str_id);
        }
        free(str_id);
        
        FILE *fp = fopen(filename, "r");
        char *url = freadlinep(fp); 
        fclose(fp);
        free(filename);

        printf("score\t%d\tdoc\t%d: %s\n", score, id, url);

        free(url);
        free(sorted[i]);  // free the memory allocated for each id_score_t object
    }

    free(sorted);
}

/**************** find_num_docs ****************/
/* itemfunc called by counters_iterate() that counts the number 
 * of items in the counterset so we can allocate an appropriate-sized array
*/
static void find_num_docs(void *arg, const int key, const int count) 
{
    int *num = arg;

    if (num != NULL)
        (*num)+=1;
}

/**************** sort_docs ****************/
/* itemfunc called by counters_iterate() that uses an insertion-sort 
 * approach to drop the new item into the array so the array is sorted 
 * in decreasing-score order.
 *
 * Creates a id_score_t object for each document in the counterset as well.
*/
static void sort_docs(void *arg, const int key, const int count) 
{
    int *index = get_index(arg);
    id_score_t **sorted = get_array(arg);
    id_score_t *doc = id_score_new(key, count);

    int i = (*index) -1;
    while (i >= 0 && sorted[i]->score < count) {
        sorted[i + 1] = sorted[i]; 
        i--;
    }
    sorted[i+1] = doc;

    if (index != NULL)
        (*index)+=1;
}

/**************** id_score_new ****************/
/* Create a new id_score_t object initialized with
 * docID and score.
*/
id_score_t *id_score_new(int docID, int score) 
{
    id_score_t *doc = malloc(sizeof(id_score_t));
    
    doc->docID = docID;
    doc->score = score;

    return doc;
}

static void *get_index(arr_sort_t *to_sort)
{
    return &(to_sort->index);
}

static void *get_array(arr_sort_t *to_sort)
{
    return (to_sort->array);
}

/**************** prompt ****************/
/* print a prompt iff stdin is a tty (terminal)
*/
static void prompt(void)
{
  if (isatty(fileno(stdin))) {
    printf("\nQuery? ");
  }
}