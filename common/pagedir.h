/* 
 * pagedir.h - header file for CS50 'pagedir' module
 *
 * a suite of functions to help the crawler write pages to the 
 * pageDirectory and help the indexer read them back in
 *
 * Adrienne Ko, Feb 10 2021
 */

#ifndef __PAGEDIR_H
#define __PAGEDIR_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "bag.h"
#include "counters.h"
#include "webpage.h"

/**************** functions ****************/

/**************** pagefetcher ****************/
/* Fetch the contents (HTML) of a page from a URL, and store it into page->html.
 *
 * Caller provides:
 *   Valid pointer to a webpage, with the page->html pointer is expected to be NULL
 * We return:
 *   True if success; page->html will contain the content retrieved.
 *   False if some error fetching page.
 * Assumptions:
 *   1. page has been allocated by caller
 *   2. page->url contains the url to fetch
 *   3. page->html is NULL at call time
 * Caller is responsible for:
 *   later freeing page->html via webpage_delete(page).
 */
bool pagefetcher(webpage_t *page);

/**************** pagescanner ****************/
/* Extract URLs from a page and process each one.
 *
 * Caller provides:
 *   Valid pointer to a webpage with a valid page->html pointer, 
 *   valid pointer to a bag, valid pointer to a hashtable
 * Assumptions:
 *   1. page, toExplore, and seen have been allocated by caller
 *   2. page->html is a pointer to a non-NULL html
 * Result:
 *   Each internal url embedded in page->html will be extracted and
 *   inserted into the hashtable of urls. If a url is successfully added
 *   to the hashtable, a new webpage for that url, at depth+1, will be created
 *   and added to the bag of webpages to be crawled. Otherwise, nothing is changed.
 * Caller is responsible for:
 *   later freeing page, toExplore, seen, as well as each extracted url by
 *   calling webpage_delete() on every newly created webpage
 */
void pagescanner(webpage_t *page, bag_t *toExplore, hashtable_t *seen);

/**************** pagesaver ****************/
/* Output a webpage to the appropriate file.
 *
 * Caller provides:
 *   Valid pointer to a webpage, string for directory path, and integer for ID.
 * We return:
 *   True if success; creates a filename of form pageDirectory/id.
 *      with the first line of the file being the URL, the second being the depth,
 *      and the rest being the page content (the HTML)
 *   False if some error saving page (error allocating memory).
 * Assumptions:
 *   1. page has been allocated by caller
 *   2. page contains a non-null url, depth, and html
 *   3. dir is the pathname for an existing directory
 * Caller is responsible for:
 *   later freeing page.
 */
bool pagesaver(webpage_t *page, const char *dir, int id);

/**************** isCrawlerDirectory ****************/
/* Verify whether dir is indeed a Crawler-produced directory
 *
 * Caller provides:
 *   String for directory path.
 * We return:
 *   True if it can open a file dir/.crawler for reading; 
 *      indicates that it is a Crawler-produced directory
 *   False if either the dir is not a valid pathname, 
 *      not a directory, not a readable directory, it’s 
 *      not a Crawler produced directory, or some other error
 */
bool isCrawlerDirectory(const char *dir);

#endif // __PAGEDIR_H