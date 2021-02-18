# CS50 Tiny Search Engine (TSE) Indexer: Implementation Spec

The following are details of the implementation of my TSE Indexer.

## Modules/functions

### main
```c
int main(const int argc, const char *argv[]);
```
1. The `main` function takes in `argc` and `argv` as parameters
2. Check if `argc == 3` to ensure that there are exactly two command-line arguments given. If not, exit with a non-zero status indicating an error with the number of arguments given.
3. Call `isCrawlerDirectory` from the `pagedir` module to verify whether `pageDirectory` is indeed a Crawler-produced directory. If it returns `false`, exit with a non-zero status indicating an invalid `pageDirectory`.
4. Try to open `indexFilename` for writing using `fopen`. If it equals `NULL`, exit with a non-zero status indicating an invalid `indexFilename`.
5. Create/initialize a *hashtable*, which will map words to *countersets*, and represent our index data structure.
6. Pass `pageDirectory`, `indexFilename`, and `wordindex` to the `indexer` function.

### indexer
```c
int indexer(const char *pagedir, const char *indexfile, hashtable_t *wordindex);
```
1. Initialize `id = 1` to identify each document in `pageDirectory`.
2. Initialize a variable `filename` to the form `pageDirectory/id` using `sscanf`.
3. Enter a while-loop that loops until `(fp = fopen(filename, "r")) == NULL`. That is, it loops through each readable document in our Crawler-produced `pageDirectory`.

    3.1 Use `freadlinep` and `freadfilep` from the `file` module to extract the url, depth and html content of the current file, and creat a new `webpage_t`.
   
    3.2 Call `webpage_getNextWord` to loop through each word of the html until there are no more words left. That is, `while ((word = webpage_getNextWord(page, &pos)) != NULL)`.

    3.2a Verify that the current word is at least three letters. If so, normalize the word using `NormalizeWord`
   
    3.2b Call `hashtable_find` on `word`. If it does not equall NULL, that means it already exists as a key in the table. In that case, increment the count of the given document by 1 by calling `counters_add` on the counterset to which `word` points.
   
    3.2c If it does not yet exist, create a new counterset with `counters_new` and initialize the count of the given document to 1 using `counters_add`. Insert this new counterset to the hashtable with the key being `word`.
   
    3.2d Free `word`

    3.3 Delete the webpage by calling `webpage_delete`
   
    3.4 Update `id` and `filename`

4. Call `index_save` to read the index data structure and save it to `indexFilename`.
5. Call `index_delete` on the index data structure to free the memory.

### index_save
```c
index_save(hashtable_t *wordindex, const char *indexfile);
```
1. Open `indexfile` for writing using `fopen(indexfile, "w")`.
2. Pass in the helper function `save_word` to `hashtable_iterate` to write each word in the index data structure to the file.

    2.1 In `save_word`, write the word to `indexfile` and then pass in the helper function `save_counts` to `counters_iterate` to write each (id, count) pair in the counterset to the index file.
    
    2.2 In `save_counts`, write each (id, count) pair to the file using `fprintf`, separated by a single space.

3. Close the file with `fclose`.

### index_delete
```c
index_delete(hashtable_t *wordindex);
```
1. Call `hashtable_delete` to delete the hashtable, and pass in the helper function `countersdelete` which will delete each counterset in the hashtable as well.

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

### webpage
The *webpage* module, defined in `webpage.h` and implemented in `webpage.c`, implements a webpage given by a `char *` *url*, a `const int` *depth* and `char *` *html*, and exports the following functions:

```c
webpage_t *webpage_new(char *url, const int depth, char *html);
void webpage_delete(void *data);
bool webpage_fetch(webpage_t *page);
char *webpage_getNextWord(webpage_t *page, int *pos);
char *webpage_getNextURL(webpage_t *page, int *pos);
bool NormalizeURL(char *url);
bool IsInternalURL(char *url);
```

## Error handling and recovery

Exit non-zero (with error) if:

 * there aren't exactly two command-line arguments given (`pageDirectory` and `indexFilename`)
 * `pageDirectory` is not a path to an existing directory
 * `indexFilename` is not a path to a writeable file
