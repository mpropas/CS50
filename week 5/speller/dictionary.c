/**
 * Implements a dictionary's functionality.
 */
 
 //this code has passed the check50 2016.speller dictionary.c dictionary.h Makefile check

#include <stdbool.h>

#include "dictionary.h"

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    //lower the first letter, this is the bucket to look in
    char firstLetter = tolower(word[0]);
    //these vars will be used later 
    int wordSize, dictWordSize;
    //initiate match to false
    bool wordMatch = false;
    //make sure the letter is in the alphabet
    if(firstLetter >= 97 && firstLetter <= 122) {
        //map to linked list based on first letter
        node *head = letters[firstLetter - 97];
        //start going through linked list
        while(head != NULL && strlen(head->word) > 0) {
            //get lengths of 2 and compare
            wordSize = strlen(word);
            dictWordSize = strlen(head->word);
            if(wordSize == dictWordSize) {
                //set match to true then filter through for loop, if every not the same the loop will set match to false and break
                wordMatch = true;
                for(int i = 0; i < wordSize; i++) {
                    if(tolower(word[i]) != head->word[i]) {
                        wordMatch = false;
                        break;
                    }
                }
                if(wordMatch) {
                    //if we made it through the loop than it is a match!
                    return true;
                }
            }
            //set to next item in the linked list otherwise
            head = head->next;
        }
    }
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary) {

    //open file and check for any errors exiting if they are found
    char word[LENGTH];
    FILE *dictFile = fopen(dictionary, "r");
    if (dictFile == NULL)
    {
        fprintf(stderr, "Could not open %s\n", dictionary);
        return 2;
    }
    //setup dictLength and myLoaded
    dictLength = 0;
    myLoaded = 0;
    //setup my array of linked lists, one list for every letter !!!!this is being pulled from dictionary.h
    for(int i = 0; i < 26; i++) {
        //for every letter find the address of somewhere with enough space for a node
        letters[i] = malloc(sizeof(node));
        //then go to that address and set next to be NULL
        letters[i]->next = NULL;
    }

    //make new node and return false if an error occurs
    while(fscanf(dictFile, "%s", word) != EOF) {
        node *new_node = malloc(sizeof(node));
        if(new_node == NULL) {
            free(new_node);
            return false;
        }
        //this is where the length is calculated and saved for later
        dictLength ++;
        strcpy(new_node->word, word);
        new_node->next = letters[word[0] - 97];
        //head = new_node;
        letters[word[0] - 97] = new_node;
    }
    //set myLoaded signal to 1 for size function
    myLoaded = 1;
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    if(myLoaded == 1) {
        return dictLength;
    }
    else {
        return 0;
    }
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    //go through letters and clear each linked list
    node *cursor, *tmp;
    for(int i = 0; i < 26; i++) {
        cursor = letters[i];
        while(cursor != NULL) {
            tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }
    return true;
}
