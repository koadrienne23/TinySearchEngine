# CS50 Tiny Search Engine (TSE) Querier: Design Spec

The TSE Querier is a standalone program that reads the index file produced by the TSE Indexer, and page files produced by the TSE Querier, and answers search queries submitted via stdin.

The following are details of the design of my TSE Querier.

## User interface

The querier's initial interface with the user is on the command-line; it must always have two arguments.

```bash
$ ./querier pageDirectory indexFilename
```

- `pageDirectory` is pathname of a directory produced by the Crawler
- `indexFilename` is the pathname of a file from which the index can be read

For example, to read the index from a file called `indexfile` in the `../data` directory created by the TSE indexer using the document files produced by the TSE crawler in (1) the subdirectory `pagedir` within the `../data` directory, we would use this command-line:

``` bash
$ ./querier ../data/pagedir/ ../data/indexfile
```

Once the command-line arguments are validated and parsed, the querier will read search queries from stdin, one per line, until EOF. The user will be asked to input a query and will be shown the results of their search through the interface.

### Inputs and outputs

*Input:* the inputs are command-line parameters as detailed in the User Interface above, as well as search query inputs from stdin.

*Output:* Querier will use the index to identify the set of documents that satisfy the query, rank the resulting set of documents according to its score, and print the set of documents in decreasing rank order; for each, list the score, document ID and URL.

### Functional decomposition into modules

We anticipate the following to be the main modules or functions:

 1. *main*, which parses arguments and initializes other modules
 2. *clean_query*, which disallows any characters other than letters or spaces and returns the number of words in the query
 3. *parse_query*, which creates an array of words and ensures that the literals ‘and’ and ‘or’ are used properly
 4. *combine*, which creates and returns a counterset (docID, count) of all of the documents that satisfy the query
 4. *rank_results*, which ranks the documents that satisfy the query in decreasing order by score and prints the results to stdout

And some helper modules that provide data structures:

  1. ***hashtable*** of `counters` of words mapping to (documentID, count) pairs
  2. ***counters*** of all the documents that satisfy the query in the form `(docID, count)`

### Pseudo code for logic/algorithmic flow

The querier will run as follows:

 1. start from a command line as shown in the User Interface
 2. parse the command line and validate parameters
 4. read search queries from stdin, one per line, until EOF
 5. for each query, load the index from indexFilename into an internal data structure
 6. check the query for any characters other than letters or spaces

	6.1 if none, get the number of words in the query
	6.2 if there are any bad characters, prompt the user to input a new search query

 7. add each word of the query into an array of strings in order
 8. check the query for any incorrect used of the literals ‘and’ and ‘or’

	8.1 if used incorrectly, print error message and prompt the use to input a new search query

 9. loop through each word in the array of strings

	9.1 keep track of the 'and's and 'or's and combine the countersets for each word accordingly
	9.2 when processing `wordA AND wordB`, find the intersection of the two sets
	9.3 when processing `wordA OR wordB`, find the union of the two sets
	9.4 return a counterset (docID, count) of all of the documents that satisfy the query

 10. find the number of documents in the counterset
 11. create a array of the documents, and sort it using insertion sort
 12. loop through the array and print each document in decreasing order (by score)

### Dataflow through modules

 1. *main* parses parameters and the stdin lines, and passes them to *clean_query* and then to *parse_query*. It also calls on *combine* and *rank_results*
 2. *clean_query* uses ctype functions and `NormalizeWord` to clean the query entry
 3. *parse_query* uses ctype functions and `strcmp()` ensure that the literals ‘and’ and ‘or’ are used properly and to create an array of words
 4. *combine* uses the index data structure (*hashtable*) and *counters* to return a counterset (docID, count) of all of the documents that satisfy the query
 5. *rank_results* uses `counters_iterate()` to loop through the *counters* and sort them by score

### Major data structures

Data structure that will be used:

 1. *counters* of all the documents that satisfy the query in the form `(docID, count)`
 2. *twocounters* used to pass in information to a helper function for finding the intersection between two countersets. Holds pointers to two countersets.
 3. *id_score* holds two integers: document ID and score
 4. *arr_sort* used to pass in information to a helper function for sorting the counterset of documents. Holds a pointers to list of `id_score_t` objects and an integer keeping track of an index.

### Testing plan

1. Test the program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly.

2. Test with an incorrect number of arguments

3. Test with `pageDirectory` being a path to a non-existent directory

4. Test with `pageDirectory` being a path to a non-Crawler-produced directory

5. Test with `indexFilename` being a path to a non-writeable file

7. Test the program with various forms of invalid search queries: the literal ‘and’ must come between words, and the literal ‘or’ must come between andsequences, so they cannot appear at the beginning or end of a query,
the literals (‘and’ & ‘or’) cannot be adjacent characters other than letters or spaces are disallowed.

8. Test the program with valid common-line arguments