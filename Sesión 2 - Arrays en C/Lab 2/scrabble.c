#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet. There are 26 letters, remember.
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

// Perhaps adding a string would make things a bit easier...
string LETTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
int LETTERS_SIZE = 26;

int compute_score(string word);

int main(void)
{
    // Get input words from both players.
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // DONE - Compute the scores of both words.
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // TODO - Print the winner.
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score1 < score2)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

/* Auxiliary function that calculates the score that is associated to a word. */
int compute_score(string word)
{
    /* We'll use these variables */
    int score = 0;                  // An integer that records the current score as more and more letters are read.
    int string_size = strlen(word); // An integer that keeps the string's size.
    int different_strings = 0;      // An integer that equals 0 if two strings or characters are identical.
    char w[string_size];

    /* The programme first converts the string by rewriting all lowercase letters as uppercase ones. */
    for (int i = 0; i < string_size; i++)
    {
        w[i] = (char) toupper(word[i]);
    }

    /* We can now calculate the word's score.
       For each letter... */
    for (int i = 0; i < string_size; i++)
    {
        // The programme detects its index.
        // If 'j' reaches "LETTERS_SIZE",
        // the score will not be increased.
        int j = 0;
        for (j = 0; j < LETTERS_SIZE; j++)
        {
            // Comparison between two characters.
            // If they're the same, take the letter's value and add it to the current score.
            if (LETTERS[j] == w[i])
            {
                score += POINTS[j];
                break;
            }
        }
    }
    return score;
}
