// Implements a dictionary's functionality

#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
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

// DONE - Choose number of buckets in hash table

/* The first character of a word is one of 26 possible letters.
-> ABCDE FGHIJ KLMNO PQRST UVWXY Z

   The following characters can be either a letter,
   an apostrophe or \0. The total is 28.
-> ABCDE FGHIJ KLMNO PQRST UVWXY Z '

   If we use four characters to decide the position in the hash table,
   the resulting number of possible positions increases to 614656
   if we use the 28-character set, so most positions would include
   just one word from the large dictionary.
*/
#define HASH_LIMIT 4
#define N 614656

/* As part of the hashing function, I could make the programme calculate powers of 28 again and again.
   However, that takes valuable time, so I'll give it the values it'll need.
   powers_of_28[i] = pow(28, HASH_LIMIT - i). */
const unsigned int powers_of_28[] = {21952, 784, 28, 1};

/* These three variables are declared here for later use in the 'hash' function.
   This way, they are not allocated for every call of 'hash'. */
unsigned int hash_sum;
char hash_new_character;
unsigned int hash_new_digit;

// Hash table
// It can be described as an array of nodes.
node *table[N];

// Number of words that the dictionary contains.
// Declared here for easier use.
unsigned int num_words_in_dict = 0;

/* Some of the variables that were part of the 'check' function
   will be declared here to prevent the allocation and freeing process
   from taking place too many times. */
char check_lowercase[LENGTH + 1];
unsigned int check_position;
node *check_item;

// DONE - Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Because we don't know what the pointers in 'table' point at,
    // we need to initialise that array before doing anything else.
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    // We're now ready to attempt to open the dictionary,
    // and we must do so in read-only mode.
    FILE *dict;
    dict = fopen(dictionary, "r");

    // Wait, has the file opened correctly?
    // We must check the pointer!
    if (dict == NULL)
    {
        fclose(dict);
        printf("FATAL ERROR. Unable to open the dictionary.\n");
        return false;
    }

    // At this point, we're ready to load the dictionary.
    // First, an auxiliary array of characters with a length of
    // 'LENGTH + 1' has to be created: its length is 46 characters
    // (remember the '\0'!!) so we don't need to worry too much
    // about word length.
    char room_for_new_word[LENGTH + 1];

    // The loading process can be done with a 'while' loop whose
    // repetition condition can be defined with 'fscanf'. This
    // function will read a string (delimited by \0, \n or \t)
    // from the input file and dump the result to our character array.
    // It returns the number of characters that have been read,
    // so a 0 means that the end of 'dict' has been reached.
    while (fscanf(dict, "%s", room_for_new_word) > 0)
    {
        // We first must assign some memory to the new node,
        // and we do so with 'malloc'.
        node *new_node = malloc(sizeof(node));

        // Let's initialise this new node with a NULL 'next' pointer
        // and with a 'word' equal to 'room_for_new_word'.
        strcpy(new_node->word, room_for_new_word);

        // The node's word must be assigned to a position in the hash table;
        // in othe words, a 'bucket'. The 'hash' function will do it.
        unsigned int position = hash(new_node->word);

        // We now need to check whether there are other words assigned to
        // 'position' (that is, check if table[position] == NULL). If not...
        if (table[position] != NULL)
        {
            // ... We can take the linked list located there
            // as what the new node's 'next' pointer points at.
            // Translated that into code...
            new_node->next = table[position];
        }
        // Then, we now do what we would do if table[position] == NULL.
        table[position] = new_node;

        // Remember to increase by 1 the number of words that have been read!
        num_words_in_dict++;
    }

    // Now we can safely close the dictionary file
    // and inform that no error has arisen.
    fclose(dict);
    return true;
}

// DONE - Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Because both dictionaries contain exclusively non-capital letters,
    // we must start by converting 'word' to lowercase. However, we first
    // need to know the length of the input.
    unsigned int wl = strlen(word);

    // (A COPY OF) The input will be converted to lowercase.
    // Remember the required size: LENGTH + 1 (WE MUST NOT FORGET '\0'!!)
    // Only capital letters can be converted to lowercase.
    for (unsigned int i = 0; i < wl; i++)
    {
        if (isupper(word[i]))
        {
            check_lowercase[i] = tolower(word[i]);
        }
        else
        {
            check_lowercase[i] = word[i];
        }
    }

    // We must NOT forget '\0'!
    check_lowercase[wl] = '\0';

    // Now let's find the correct position of the input word in the hash table
    // and set it as our starting point.
    check_position = hash(check_lowercase);
    check_item = table[check_position];

    // If the table is large enough, there will be few items to which
    // the input can be compared.
    while (check_item != NULL)
    {
        // If the converted word is identical to the item's 'word',
        // then there are no spelling mistakes according to the dictionary.
        if (strcmp(check_lowercase, check_item->word) == 0)
        {
            return true;
        }
        // Otherwise, we need to look at the 'next' item.
        check_item = check_item->next;
    }

    // If we reach a NULL pointer,
    // that means the word is not in our dictionary.
    return false;
}

// DONE - Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return num_words_in_dict;
}

// DONE - Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // For each position in the hash table...
    for (int i = 0; i < N; i++)
    {
        // Let's create a pointer for it.
        node *pointer_for_position = table[i];

        // And while it is not a NULL pointer...
        while (pointer_for_position != NULL)
        {
            // Let's create a copy of it before assigning the original one to the
            // 'next' node and freeing the copy.
            node *auxiliary_pointer = pointer_for_position;
            pointer_for_position = pointer_for_position->next;
            free(auxiliary_pointer);
        }
    }

    // Finally, we need to inform that we have finished.
    return true;
}

// TODO - Hashes word to a number
// The hashing function takes a word as input and returns a number:
// that word's appropriate position in the hash table.
unsigned int hash(const char *word)
{
    /* Our main goal is to improve this hash function:

       return toupper(word[0]) - 'A';

       The problem with the original hashing function is that
       the original table uses only 26 positions for 143092 words
       (large dictionary), which means that, on average,
       just over 5500 words are assigned to each position.
       That ratio is too high, and we can improve this. */

    // Let's start by retrieving the word's length
    // (final '\0' included).
    unsigned int wl = strlen(word);

    // If 'wl' is less than 'HASH_LIMIT', we'll take its value.
    // This way, we can perform one single comparison instead of two.
    if (wl > HASH_LIMIT)
    {
        wl = HASH_LIMIT;
    }

    // We will need some auxiliary variables:
    // - One that keeps track of the current sum, ...
    // - One that records the new character, and ...
    // - One that is initialised depending on what the new character is.
    hash_sum = 0;

    // For the first four characters of a word...
    for (unsigned int j = 0; j < wl; j++)
    {
        // If it's a letter, 'new_digit' will be given
        // its position in the English alphabet.
        // It first needs to be converted to uppercase, however.
        if (isalpha(word[j]))
        {
            hash_new_digit = toupper(word[j]) + 1 - 'A';
        }
        // If it's an apostrophe, it'll be given the number 27.
        else if (word[j] == '\'')
        {
            hash_new_digit = 27;
        }
        // Other characters will interrupt the calculation process.
        else
        {
            break;
        }

        // 'sum' will have its value increased according to this formula.
        // Remember the 'powers_of_28' array!
        hash_sum += hash_new_digit * powers_of_28[j];
    }

    // Once we're done, the function will return the value of 'sum'.
    return hash_sum;
}
