#ifndef HW2_H
#define HW2_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "../include/mispelling.h"
//#include "hw2.h"

#define MAX_SIZE 256
#define WORDLENGTH 50
#define MAX_MISSPELLED_WORDS 5

#define USAGE(return_code) do { \
    fprintf(stderr, "%s\n", \
        "Usage: spell [-h] [-o OUT_FILE] [-i IN_FILE] [-d DICT_FILE] [-An]\n" \
        "Spell Checker using a custom dictionary. Auto corrects any known misspellings in the text.\n" \
        "Additional function to add new words and randomized misspellings to the dictionary.\n\n" \
        "\t-h\tDisplays this usage.\n" \
        "\t-o\tOUT_FILE filename, if omitted output to stdout\n" \
        "\t-i\tIN_FILE filename, if omitted input comes from stdin (CTRL+D to end input)\n" \
        "\t-d\tfor the dictionary filename, if omitted use default 'rsrc/dictionary.txt'\n" \
        "\t-An\tAutomatically add n (in range 0-5) random misspellings for any word not in the dictionary.\n"); \
} while (0);

struct dictionary* dict;
struct misspelled_word* m_list;
#endif

struct Args{
    bool d;
    bool i;
    bool o;
    bool aFlag;

    char dictFile[MAX_SIZE];
    char input[MAX_SIZE];
    char output[MAX_SIZE];
};

struct dictionary{
    int num_words;
    struct dict_word* word_list;
};

struct dict_word{
    char word[WORDLENGTH];
    int misspelled_count; // initialimakeze to 0
    int num_misspellings; // initialize to 0
    struct misspelled_word* misspelled[MAX_MISSPELLED_WORDS];
    struct dict_word* next;
};

struct misspelled_word{
    char word[WORDLENGTH];
    int misspelled; // initialize to 0
    struct dict_word* correct_word;
    struct misspelled_word* next;
};
int totalMisspellings(struct dict_word* currWord);



/**
 * @brief      read in dictionary
 *
 * @param      f     opened dictionary file
 */
void processDictionary(FILE* f);


/**
 * @brief      Adds a word.
 *
 * @param      dWord  The d word
 * @param      word   The word
 */
void addWord(struct dict_word* dWord, char* word);



/**
 * @brief      Adds a misspelled word.
 *
 * @param      misspelledWord  The misspelled word
 * @param      correctWord     The correct word
 * @param      word            The word
 */
void addMisspelledWord(struct misspelled_word * misspelledWord, struct dict_word* correctWord, char* word);


/**
 * @brief      free the words
 *
 * @param      word  The word
 */
void freeMWords(struct misspelled_word* currWord);
void freeWords(struct dict_word* word);

char* theMisspelledWord(char* inputWord);
void nullifySpace(char* arr, int size);
void freeSpace(struct Args* args);

void printDictionary(struct dict_word* currWord, FILE* f);
struct dict_word* topMisspelled(struct dict_word* currWord);
struct dict_word* topTwoMisspelled(struct dict_word* currWord, struct dict_word* no);
struct dict_word* topThreeMisspelled(struct dict_word* currWord, struct dict_word* no, struct dict_word* no2);
void printTopThreeMispelledWords(struct dict_word* a);




/**
 * @brief      Print the words
 *
 * @param      word  The word
 * @param      f     { parameter_description }
 */
void printWords(struct dict_word* word, FILE* f);



/**
 * @brief      Process a word
 *
 * @param      inputWord  The input word
 */
void processWord(char* inputWord, int numMisspellings);



/**
 * @brief      find the misspelling
 *
 * @param      inputWord  The input word
 *
 * @return     boolean
 */
bool foundMisspelledMatch(char* inputWord);



/**
 * @brief      find match in dictionary
 *
 * @param      inputWord  The input word
 *
 * @return     boolean
 */
bool foundDictMatch(char* inputWord);
