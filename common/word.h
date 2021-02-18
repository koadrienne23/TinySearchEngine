/* 
 * word.h - header file for CS50 'word' module
 *
 * implements NormalizeWord (common between the Indexer and Querier)
 *
 * Adrienne Ko, Feb 17 2021
 */

#ifndef __WORD_H
#define __WORD_H

#include <stdio.h>
#include <ctype.h>
#include <string.h>

/**************** functions ****************/

/**************** NormalizeWord ****************/
/* "Normalize" the string passed by the caller. That is, convert all letters to lowercase.
 *
 * Notes:
 *   Iterates through all the characters in the string and converts each one to
 *   lowercase using tolower().
 */
void NormalizeWord(char *word);

#endif // __WORD_H