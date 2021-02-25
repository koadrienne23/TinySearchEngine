# CS50 Tiny Search Engine (TSE) Querier: Implementation Spec

The following are details of the implementation of my TSE Querier.

## Modules/functions

### main
```c
int main(const int argc, const char *argv[]);
```
1. The `main` function takes in `argc` and `argv` as parameters
2. Check if `argc == 3` to ensure that there are exactly two command-line arguments given. If not, exit with a non-zero status indicating an error with the number of arguments given.
3. Call `isCrawlerDirectory` from the `pagedir` module to verify whether `pageDirectory` is indeed a Crawler-produced directory. If it returns `false`, exit with a non-zero status indicating an invalid `pageDirectory`.
4. Try to open `indexFilename` for reading using `fopen`. If it equals `NULL`, exit with a non-zero status indicating an invalid `indexFilename`.
5. Enter loop that continues reading lines from stdin until it equals `NULL`.

    5.1 Call `index_load()` from the `index.c` module and save the contents of `indexFilename` into a new *hashtable*
    5.2 Pass in the `line` to `clean_query()` to find the number of words in the query. If it is less than or equal to 0, free any allocated memory and `continue;`
    5.3 Create a new array of strings which will hold each word in the query. Pass this array, along with `line` and the size of the query to `parse_query`. If it does not return 0, free memory and `continue`.
    5.4 Create a `counters` object that holds all of the documents that satisfy the query by calling `combine()`
    5.5 Then call `rank_results` to sort the documents in decreasing order by score, and print to stdout
    5.6 Free any allocated memory

### clean_query
```c
int clean_query(char *input);
```
1. Loop through each character of `input`
2. Initialize an int variable `size` to 0, which will keep track of the number of words.
3. If it is a letter (check with `isalpha()`), check a boolean to determine whether the last character was a space. If so, increment `size` by 1.
4. If not a letter nor a space, print an error message indicating that the query has a bad character.
5. If it is a space, set the boolean to `true`.
6. Call `NormalizeWord()` on `input`
7. Return `size`

### parse_query
```c
int parse_query(char *input, char *words[], int size);
```
1. Loop through each character of `input`.
2. At the start of each new word, loop through `input` starting from that point and set the first non-letter space to be `'\0'`.
3. Check whether the first and last words of the array is either equal to 'and' or 'or'. If so, return non-zero.
4. Check whether there are any adjacent words that are equal to 'and' or 'or'. If so, return non-zero.
5. Return 0;

### combine
```c
counters_t *combine(char *words[], hashtable_t *wordindex, int size);
```
1. Initialize a new *counters* called `result`
2. Initialize a new *counters* called `temp` to equal the *counters* object mapped to by the first word in the array, if array is not empty.
3. Start from `i=1` and enter a loop while `i<size`.
4. During each iteration of the loop, check whether the word is equal to 'or', 'and', or some other word.
5. If it is equal to 'or', set `result` to equal the union between `result` and `temp` and set `temp` to be the *counters* object mapped by the next word.
6. Otherwise, set `temp` to equal the intersection between `temp` and `words[i+1]` if the current word is 'and' and `words[i]` if the current word is something else.
7. After the loop terminates, call `merge()` on `result` and `temp` one last time.
8. Return `result`

### rank_results
```c
void rank_results(counters_t *results, const char *pagedir);
```
1. Initialize `num=0` and then pass it in to `counters_iterate()` as the `arg` to find the size of the `results` using the `find_num_docs` helper function.
2. If `num==0`, print that there was no match found.
3. Call `counters_iterate()` again but pass in `sort_docs` instead to sort the documents in order of decreasing score. 
4. Loop through each of the documents and print out the appropriate information.

## Data structures

### hashtable
The *hashtable* module, defined in `hashtable.h` and implemented in `hashtable.c`, implements a set of `void*` with `char*` *keys *and exports the following functions:

```c
hashtable_t *hashtable_new(const int num_slots);
bool hashtable_insert(hashtable_t *ht, const char *key, void *item);
void *hashtable_find(hashtable_t *ht, const char *key);
void hashtable_print(hashtable_t *ht, FILE *fp, void (*itemprint)(FILE *fp, const char *key, void *item));
void hashtable_iterate(hashtable_t *ht, void *arg, void (*itemfunc)(void *arg, const char *key, void *item) );
void hashtable_delete(hashtable_t *ht, void (*itemdelete)(void *item) );
```

### counters
The *counters* module, defined in `counters.h` and implemented in `counters.c`, implements a set of integer counters with `int` *keys* (where *keys* must be non-negative) and export the following functions:

```c
counters_t *counters_new(void);
int counters_add(counters_t *ctrs, const int key);
int counters_get(counters_t *ctrs, const int key);
bool counters_set(counters_t *ctrs, const int key, const int count);
void counters_print(counters_t *ctrs, FILE *fp);
void counters_iterate(counters_t *ctrs, void *arg, void (*itemfunc)(void *arg, const int key, const int count));
void counters_delete(counters_t *ctrs);
```

## Error handling and recovery

Exit status 1: error caused by incorrect number of arguments

Exit status 2: error caused by invalid `pageDirectory`

Exit status 3: error caused by invalid `indexFilename`

Exit status 0: program terminated successfully without error