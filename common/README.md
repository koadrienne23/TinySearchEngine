# CS50 Tiny Search Engine (TSE) 'common' library

This `common` directory comprises of code for initializing the “pageDirectory” and saving webpages there. Specifically, it contains the `pagedir` file which defines a suite of functions to help the crawler write pages to the “pageDirectory” and help the indexer read them back in.

## Usage

To build the `common.a` library, simply run `make`. Run `make clean` to remove all files produced by Make.

```c
bool pagefetcher(webpage_t *page);
void pagescanner(webpage_t *page, bag_t *toExplore, hashtable_t *seen);
bool pagesaver(webpage_t *page, const char *dir, int id);
```