/* 
 * pagedir.c - CS50 'pagedir' module
 *
 * see pagedir.h for more information.
 *
 * Adrienne Ko, Feb 10 2021
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "bag.h"
#include "counters.h"
#include "webpage.h"


/**************** local functions ****************/
inline static void logr(const char *word, const int depth, const char *url);


/**************** pagefetcher ****************/
bool
pagefetcher(webpage_t *page)
{
    if (webpage_fetch(page)) {
        logr("Fetched", webpage_getDepth(page), webpage_getURL(page));
        return true;
    } else {
        logr("Invalid", webpage_getDepth(page), webpage_getURL(page));
        return false;
    }
}

/**************** pagescanner ****************/
void 
pagescanner(webpage_t *page, bag_t *toExplore, hashtable_t *seen)
{
    int depth = webpage_getDepth(page);
    logr("Scanning", depth, webpage_getURL(page));

    int pos = 0;
    char *url;

    while ((url = webpage_getNextURL(page, &pos)) != NULL) {
        logr("Found", depth, url);
        if (IsInternalURL(url)) {
            if (hashtable_insert(seen, url, "")) {
                // url has not been seen yet
                webpage_t* newpage = webpage_new(url, depth+1, NULL);
                bag_insert(toExplore, newpage);
                logr("Added", depth, url);
            } else {
                logr("IgnDupl", depth, url);
                free(url);
            }
        } else {
            logr("IgnExtrn", depth, url);
            free(url);
        }
    }
}

/**************** pagesaver ****************/
bool
pagesaver(webpage_t *page, const char *dir, int id)
{
    int len_id = snprintf(NULL, 0, "%d", id);
    char *str_id = malloc(len_id + 1);
    if (str_id == NULL) {
        // error allocating memory for str_id; return false
        return false;
    } else {
        snprintf(str_id, len_id + 1, "%d", id);
    }

    char *filepath = malloc(strlen(dir) + strlen(str_id) + 2);
    if (filepath == NULL) {
        // error allocating memory for filepath; return false
        return false;
    } else {
        filepath = strcpy(filepath, dir);
        filepath = strcat(filepath, "/");
        filepath = strcat(filepath, str_id);
    }
    free(str_id);
    
    FILE *fp = fopen(filepath, "w");
    fprintf(fp, "%s\n%d\n%s", webpage_getURL(page), webpage_getDepth(page), webpage_getHTML(page));
    fclose(fp);
    free(filepath);

    logr("Saved", webpage_getDepth(page), webpage_getURL(page));

    return true;
}

/**************** isCrawlerDirectory ****************/
bool 
isCrawlerDirectory(const char *dir)
{
    char *filename = malloc(strlen(dir) + strlen("/.crawler") + 1);
    if (filename == NULL) {
        // error allocating memory for filename; return false
        return false;
    } 

    filename = strcpy(filename, dir);
    filename = strcat(filename, "/.crawler");

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        // cannot open a file dir/.crawler for reading 
        return false;
    } 
    fclose(fp);
    free(filename);

    return true;
}

/**************** logr ****************/
/* log one word (1-9 chars) about a given url */                              
inline static void logr(const char *word, const int depth, const char *url)
{
  printf("%2d %*s%9s: %s\n", depth, depth, "", word, url);
}