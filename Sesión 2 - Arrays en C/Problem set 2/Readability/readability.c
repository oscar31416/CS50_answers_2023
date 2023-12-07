#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// Declaration of functions. Defined later.
int count_letters(string in_str);
int count_words(string in_str);
int count_sentences(string in_str);
void cli(int let, int wor, int sen);
int round_custom(float real);

/* Main function. */
int main(void)
{
    // The programme needs a string as input.
    string input_string = get_string("Text: ");

    // With that input, the number of letters is calculated.
    int number_of_letters = count_letters(input_string);

    // Then, the number of words.
    int number_of_words = count_words(input_string);

    // Finally, the number of sentences.
    int number_of_sentences = count_sentences(input_string);

    // We can now calculate the Coleman-Liau index and tell to which grade the input corresponds.
    cli(number_of_letters, number_of_words, number_of_sentences);
}

// AUTHOR'S NOTE
//
// Judging by the requirements of the functions called 'count_letters',
// 'count_words' and 'count_sentences', it seems perfectly posible (and,
// perhaps, faster) to do all the counting simultaneously. We can use just
// one 'for' loop and one 'if···else if···else if···' block, and we'd have
// just one auxiliary function in addition to shorter, easier-to-read code.

/* Auxiliary function that counts the number of letters. */
int count_letters(string in_str)
{
    /* We need some local variables. */
    int len_str = strlen(in_str); // Length of the input string.
    int letters = 0;              // Variable that keeps track of the number of letters. Its initial value is 0.

    /* For each character, we'll do the following. */
    for(int i = 0; i < len_str; i++)
    {
        // If the character is either a lowercase or an uppercase letter...
        if ((isupper(in_str[i]) != 0) || (islower(in_str[i]) != 0))
        {
            // Add 1 to 'letters'.
            letters++;
        }
    }
    return letters;
}

/* Auxiliary function that counts the number of words. */
int count_words(string in_str)
{
    /* We need some local variables. */
    int len_str = strlen(in_str); // Length of the input string.
    int words = 1;                // Variable that keeps track of the number of words. Its initial value is assumed to be 1.

    /* For each character, we'll do the following. */
    for(int i = 0; i < len_str; i++)
    {
        // If the character is a blank space...
        if(in_str[i] == ' ')
        {
            // Add 1 to 'words'.
            words++;
        }
    }
    return words;
}

/* Auxiliary function that counts the number of sentences. */
int count_sentences(string in_str)
{
    /* We need some local variables. */
    int len_str = strlen(in_str); // Length of the input string.
    int sentences = 0;             // Variable that keeps track of the number of sentences. Its initial value is 0.

    /* For each character, we'll do the following. */
    for(int i = 0; i < len_str; i++)
    {
        // If the character is '.', '!' or '?'...
        if((in_str[i] == '.') || (in_str[i] == '!') || (in_str[i] == '?'))
        {
            // Add 1 to 'sentences'.
            sentences++;
        }
    }
    return sentences;
}

/* Auxiliary function that calculates the Coleman-Liau index and tells the appropriate grade. */
void cli(int let, int wor, int sen)
{
    // 'L' is the number of letters per 100 words, on average.
    float l = (100.0 * let) / wor;

    // 'S' is the number of sentences per 100 words, on average.
    float s = (100.0 * sen) / wor;

    // We can now calculate the index and round the value to the nearest integer.
    float index_aux = 0.0588 * l - 0.296 * s - 15.8;
    int index = 0;
    index = round_custom(index_aux);

    // Time to check the appropriate grade.
    if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}

/* Auxiliary function that rounds a 'float' to the nearest integer.
   My 'math.h' header file seems to lack a definition of a 'round' function. */
int round_custom (float real)
{
    int result;
    int real_to_int = (int) real;
    int diff_aux = (int) (2.0 * (real - real_to_int));
    result = real_to_int + diff_aux;
    return result;
}
