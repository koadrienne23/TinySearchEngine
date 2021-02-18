# CS50 Tiny Search Engine (TSE) Implementation 

The following are details of the implementation of my TSE Crawler.

## Modules/functions

### main
```c
int main(const int argc, const char *argv[]);
```
1. The `main` function takes in `argc` and `argv` as parameters
2. Check if `argc == 4` to ensure that there are exactly three command-line arguments given. If not, exit with a non-zero status indicating an error with the number of arguments given.
3. Allocate a string `seedURL` to hold memory for the length of the given url using `calloc` and copy `argv[1]` to it.
4. Check if `seedURL` is 'internal' using the `IsInternalURL` function given by the `webpage` module. If it is not internal, exit with non-zero status indicating an error caused by an invalid url.
5. Check if `pathname` points to an existing directory by calling `opendir()`. If it results in an error indicating that it is not in fact an existing directory, exit with non-zero status.
6. Check if `maxDepth` is an integer in the range [0,10] using `sscanf`. If it is not, exit with non-zero status.
7. Allocate and initialize a new bag_t structure and a new hashtable_t structure.
8. Create a new webpage_t structure, initialized with the `seedURL`, `0`, and `NULL`, and insert it into the bag of pages we have yet to explore. Also insert the `seedURL` to the hashtable of URL's we've seen.
9. Pass `pathname`, `maxDepth`, `toExplore` and `seen` to the `crawler` function. If it returns a non-zero value, exit with non-zero status indicating an error with the crawling process. Otherwise, `return 0`.

### crawler
```c
int crawler(const char *pathname, int maxDepth, bag_t *toExplore, hashtable_t *seen) 
```
1. The `crawler()` function returns 0 if it successfully loops over all the pages to explore.
2. Initialize `id = 1` to name our files containing the webpages.
3. Enter a while-loop that loops until there are no more webpages left in our bag of pages. That is, while `(page = bag_extract(toExplore)) != NULL`. Every iteration of the loop, `page` points to a new webpage.
4. Call `pagefetcher(page)`, which returns a boolean. If it returns `false`, do nothing. If it returns `true`, that means that the content of the webpage (the html) was successfully retrieved and stored into page->html, so we can move forward with saving the page.
   
    4a. If `true`, call `pagesaver(page, pathname, id)`, which returns another boolean. If it returns `true`, that means that `page` was successfully saved as a file with the filename `pathname/id` in the directory given by `pathname`. In this case, increment `id` by 1. If it returns `false`, delete the bag structure and the hashtable stucture, and return a non-zero integer.
   
    4b. If the `depth` of `page` is less than `maxDepth`, call `pagescanner(page, toExplore, seen)`.
5. At the end of the while-loop, call `webpage_delete(page)` to free both the url and the html, if not NULL.
6. After the while-loop, delete the bag structure (`bag_delete(toExplore, NULL)`) and the hashtable stucture (`hashtable_delete(seen, NULL)`) and `return 0`.

### pagefetcher
```c
bool pagefetcher(webpage_t *page);
```
1. `pagefetcher` returns a boolean. It returns `true` if the page is successfully "fetched", and `false` otherwise.
2. Given a valid pointer to a webpage, with the page->html pointer being NULL, call `webpage_fetch(page)` (given by the `webpage` module) to retrieve the content of the page (html). If this function call returns `true`, indicate with a print statement that the content has been "fetched" and return `true`. Otherwise, return `false`.

### pagesaver
```c
bool pagesaver(webpage_t *page, const char *dir, int id);
```
1. `pagesaver` returns a boolean. It returns `true` if it successfully creates a new file, and `false` otherwise.
2. First, determine the length of the `id` (number of digits) using `snprintf` and use that integer value to allocate `len_id+1` blocks of memory of size `char` to `str_id `. If `str_id == NULL`, there was an error allocating the memory so return `false`. Otherwise, set `str_id ` to be the string representation of `id` using `snprintf` again.
3. Allocate `strlen(dir) + strlen(str_id) + 2` blocks of memory of size `char` to `filepath`, again returning `false` if it fails to do so. Otherwise, copy `dir/str_id` to `filepath`. 
4. Free `str_id`.
5. Write the appropriate file to the directory given by `filepath` using `fopen()` and `fprintf()`. Don't forget to `fclose()` afterwards.
6. Free `filepath`.
7. Return `true`.

### pagescanner
```c
void pagescanner(webpage_t *page, bag_t *toExplore, hashtable_t *seen);
```
1. Enter while-loop that extracts a url embedded in the html of `page` until there are no more by setting the loop condition to `(url = webpage_getNextURL(page, &pos)) != NULL`.
2. Check if the extracted url is 'internal' by calling the `IsInternalURL()` function given by the `webpage` module. If it is not 'internal', free the url. 
3. Otherwise, try inserting the url into our hashtable `seen`. If successful, create a new `webpage` with that url and `depth+1` and insert it into the bag of pages to explore. If it is unsuccessful (that is, that url has already been added to the hashtable), free the url.

## Data structures

### bag
The *bag* module, defined in `bag.h` and implemented in `bag.c`, implements a bag of `void*`, and exports the following functions:

```c
bag_t *bag_new(void);
void bag_insert(bag_t *bag, void *item);
void *bag_extract(bag_t *bag);
void bag_print(bag_t *bag, FILE *fp, 
	       void (*itemprint)(FILE *fp, void *item));
void bag_iterate(bag_t *bag, void *arg, void (*itemfunc)(void *arg, void *item) );
void bag_delete(bag_t *bag, void (*itemdelete)(void *item) );
```

### set
The *set* module, defined in `set.h` and implemented in `set.c`, implements a set of `void*` with `char*` *keys* and exports the following functions:

```c
set_t *set_new(void);
bool set_insert(set_t *set, const char *key, void *item);
void *set_find(set_t *set, const char *key);
void set_print(set_t *set, FILE *fp, void (*itemprint)(FILE *fp, const char *key, void *item) );
void set_iterate(set_t *set, void *arg, void (*itemfunc)(void *arg, const char *key, void *item) );
void set_delete(set_t *set, void (*itemdelete)(void *item) );
```

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

Exit status 1: error caused by invalid `seedURL`

Exit status 2: error caused by invalid `pathname`

Exit status 3: error caused by invalid `maxDepth`

Exit status 4: error caused by a failure in crawling (most likely due to errors in allocating memory)
