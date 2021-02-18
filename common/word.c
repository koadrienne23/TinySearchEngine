/* 
 * word.c - CS50 'word' module
 *
 * see word.h for more information.
 *
 * Adrienne Ko, Feb 17 2021
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>

/**************** NormalizeWord ****************/
void
NormalizeWord(char *word)
{
    for(int i = 0; i < strlen(word); i++){
        word[i] = tolower(word[i]);
    }
}
