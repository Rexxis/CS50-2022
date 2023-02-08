// Implements a dictionary's functionality
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

void free_memory(node *n);

// TODO: Choose number of buckets in hash table
const unsigned int N = 5000;

// Hash table
node *table[N];

// size counter
unsigned int dict_size = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // get hash value
    int hash_value = hash(word);

    // go to hash value bucket
    node *traverse = table[hash_value];

    // if no value in table bucket
    if (traverse == NULL)
    {
        return false;
    }

    // go thorough linkedlist in the table bucket
    while (traverse != NULL)
    {
        if (strcasecmp(word, traverse->word) == 0)
        {
            return true;
        }

        traverse = traverse->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // initialize variable to store value
    int character = 0;

    for (int i = 0; i < strlen(word); i++)
    {
        // get each character value
        character += (int) toupper(word[i]);
    }

    // modulo 5000 to accomodate max table array
    return character % 5000;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // get dictionary file pointer
    FILE *inptr = fopen(dictionary, "r");
    if (inptr == NULL)
    {
        return false;
    }

    // buffer to store word
    char dword[LENGTH + 1];

    // read dictionary until EOF
    while (fscanf(inptr, "%s", dword) != EOF)
    {
        // allocate memory for new node
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            printf("Out of memory");
            return false;
        }

        // copy the word to node word
        strcpy(n->word, dword);

        // get hash value of word
        int hash_value = hash(dword);

        // inserting node to table

        // if it's the first value in the LL
        if (table[hash_value] == NULL)
        {
            // initialize next node as null
            n-> next = NULL;
            // insert the node
            table[hash_value] = n;
            dict_size++;
        }
        else
        {
            // assign next node to LL head node
            n->next = table[hash_value];
            // move the pointer to the new head node
            table[hash_value] = n;
            dict_size++;
        }
    }

    fclose(inptr);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return dict_size;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *traverse = table[i];
        if (traverse != NULL)
        {
            free_memory(traverse);
        }
    }

    return true;
}

void free_memory(node *n)
{
    if (n == NULL)
    {
        return;
    }

    // recursively free next node
    free_memory(n->next);

    // free self
    free(n);

}