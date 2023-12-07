#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// Auxiliary functions. Defined later.
int check_key(string in_key);
string replace(string in_string, string in_key);
int find_char_in_str(char c, string s);

/* Main function. */
int main(int argc, string argv[])
{
    /* 1 - Check whether there's just one argument. */
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    /* 2 - Take the key, convert it to uppercase (to make things a bit easier)
       and make sure it's appropriate. */
    string key = argv[1];
    int key_len = strlen(key);
    for (int i=0; i < key_len; i++)
    {
        key[i] = toupper(key[i]);
    }
    int check_value = check_key(key);
    if (check_value == 1)
    {
        printf("ERROR - Invalid key.\n\n Requirements:\n");
        printf("- Length: exactly 26 characters.\n");
        printf("- Avoid non-alphabetic characters.\n");
        printf("- No letter must be repeated.\n");
        printf("- The letters' case is irrelevant.\n");
        return 1;
    }

    /* 3 - Ask the user to type a string. */
    string input_string = get_string("plaintext: ");

    /* 4 - Alter the input using the key. */
    string output_string = replace(input_string, key);

    /* 5 - Show the modified text. */
    printf("ciphertext: %s\n", output_string);

    return 0;
}

/* Auxiliary function that checks the key.
   Returns 0 if the key is valid, or 1 otherwise. */
int check_key(string in_key)
{
    // Check length.
    int key_length = strlen(in_key);
    if ((key_length) != 26)
    {
        return 1;
    }

    // Make sure it's all alphabetical characters.
    for (int i = 0; i < key_length; i++)
    {
        if (isalpha(in_key[i]) == 0)
            {
                return 1;
            }
    }

    // Make sure no letter is repeated.
    // Converting the key to uppercase becomes useful here.
    for (int i = 0; i < (key_length - 1); i++)
    {
        for (int j = (i + 1); j < key_length; j++)
        {
            if (in_key[i] == in_key[j])
                {
                    return 1;
                }
        }
    }
    return 0;
}

/* Auxiliary function that modifies a string using a key. */
string replace(string in_str, string in_key)
{
    /* We need some local variables. */
    string out_str = in_str;                        // Output string.
    string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; // Reference key.
    int n_str = strlen(in_str);                     // Length of the input string.
    int n_key = strlen(in_key);                     // Length of the input key.
    int upper_or_lower = 0;                         // Control variable. 1 if lowercase letter, 2 if uppercase, 0 if not a letter.
    int loc = 0;                                    // Control variable used to locate letters in 'alphabet'.
    int aux = 0;

    /* Now let's modify 'in_str'. For each character... */
    for (int i = 0; i < n_str; i++)
    {
        // We must make sure it's a letter.
        if (isupper(in_str[i]) != 0)
        {
            upper_or_lower = 2;
        }
        else if(islower(in_str[i]) != 0)
        {
            upper_or_lower = 1;
        }
        else
        {
            upper_or_lower = 0;
        }

        // If it's a lowercase letter, we need to convert it to uppercase.
        if (upper_or_lower == 1)
        {
            out_str[i] = toupper(in_str[i]);
        }
        else
        {
            out_str[i] = in_str[i];
        }

        // We must locate the letter in the 'alphabet' string before attempting to modify it.
        loc = find_char_in_str(out_str[i], alphabet);

        // Now we can convert the letter.
        if (loc != -1)
        {
            out_str[i] = in_key[loc];
        }

        // If it was a lowercase letter, we need to undo the conversion.
        if (upper_or_lower == 1)
        {
            out_str[i] = tolower(out_str[i]);
        }
    }
    return out_str;
}

/* Auxiliary function that looks for a character in a string.
   Returns the location of the character's first iteration if found, or -1 otherwise. */
int find_char_in_str(char c, string s)
{
    int loc;
    int s_len = strlen(s);

    for (loc = 0; loc < s_len; loc++)
    {
        if (c == s[loc])
        {
            return loc;
        }
    }
    return -1;
}
