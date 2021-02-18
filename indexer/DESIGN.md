# CS50 Tiny Search Engine (TSE) Indexer: Design Spec

The TSE indexer is a standalone program that reads the document files produced by the TSE crawler, builds an index, and writes that index to a file. Its companion, the index tester, loads an index file produced by the indexer and saves it to another file.

The following are details of the design of my TSE Indexer.

## User interface

The indexer's only interface with the user is on the command-line; it must always have two arguments.

```bash
$ ./indexer pageDirectory indexFilename
```

- `pageDirectory` is pathname of a directory produced by the Crawler
- `indexFilename` is the pathname of a file into which the index should be written; the indexer creates the file (if needed) and overwrites the file (if it already exists).

For example, to build an index from the document files produced by the TSE crawler in (1) the subdirectory `pagedir` within the `../data` directory and write it to (2) a file called `indexfile` in the `../data` directory, we would use this command-line:

``` bash
$ ./indexer ../data/pagedir/ ../data/indexfile
```

## Inputs and outputs

*Input:* the inputs are command-line parameters. See the User Interface above.

*Output:* Indexer will read through all of the documents in the Crawler-produced directory and build an inverted-index data structure mapping from words to (documentID, count) pairs, wherein each count represents the number of occurrences of the given word in the given document. 
This index file will have:

 * one line per word, one word per line
 * each line provides the word and one or more (docID, count) pairs, in the format word docID count [docID count]...
 * where word is a string of lower-case letters,
 * where docID is a positive non-zero integer,
 * where count is a positive non-zero integer,
 * where the word and integers are separated by spaces.

## Functional decomposition into modules

We anticipate the following modules or functions:

 1. *main*, which parses arguments and initializes other modules
 2. *indexer*, which loops over all of the Crawler-produced documents and adds the content to the inverted-index data structure
 3. *index_save*, which read the inverted-index data structure and writes it to a file in the proper format
 4. *index_delete*, which deletes the inverted-index data structure
 5. *isCrawlerDirectory*, verifies whether `dir` is indeed a Crawler-produced directory

And some helper modules that provide data structures:

  1. ***webpage*** which allows us to extract the words from the Crawler documents
  2. ***hashtable of countersets*** of words mapping to (documentID, count) pairs

## Pseudo code for logic/algorithmic flow

The crawler will run as follows:

1. start from a command line as shown in the User Interface
2. parse the command line, validate parameters, initialize other modules
3. loop through each document in `pageDirectory` and for each document,

	3.1 extract the url, depth, and html content to create a new *webpage* with these values
	3.2 loop through all the words in the webpage and for each word, if it consists of at least three characters,

	3.2a normalize the word
	3.2b if the word already exists as a key in the ***hashtable***, increment the count of the given document by 1
	3.2c if the word is not already in the table, create a new ***counterset***, initialize the count of the given document to 1, and insert that counterset into the table with the word as its key

4. use `index_save` to save this inverted-index data structure (the hashtable of countersets) to `indexFilename`

**normalizing a word** means to convert all letters to lowercase.

## Dataflow through modules

 1. *main* parses parameters and passes them to the indexer.
 2. *indexer* loops through each ***webpage*** and uses a ***hashtable*** of ***counters*** to build an inverted-index data structure; it then gives the resulting index datastructure to *index_save*.
 3. *index_save* writes the inverted index datastructure to a file in the proper format.

## Major data structures

Data structures:

 1. *webpage* which contains all the data read for a given webpage, plus the URL and the depth at which it was fetched
 2. *hashtable of countersets* which represents the inverted-index data structure, mapping from words to (documentID, count) pairs, wherein each count represents the number of occurrences of the given word in the given document

## Testing plan

1. Test the program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly.

2. Test with an incorrect number of arguments

3. Test with `pageDirectory` being a path to a non-existent directory

4. Test with `pageDirectory` being a path to a non-Crawler-produced directory

5. Test with `indexFilename` being a path to a non-writeable file

6. Test the program with valid common-line arguments
