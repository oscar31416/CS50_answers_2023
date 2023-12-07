#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef uint8_t BYTE;

/* Main function. */
int main(int argc, char *argv[])
{
    int n_args = argc - 1;

    /* Interruption condition #1
       The number of arguments is not 1. */
    if (n_args != 1)
    {
        printf("Usage: ./recover path/to/file\n");
        return 1;
    }

    /* Interruption condition #2
       Unable to open the input file. */
    FILE *input_file = fopen(argv[1], "r");
    if (input_file == NULL)
    {
        printf("ERROR. Unable to open the input file.\n");
        return 1;
    }

    /* Some variables that I will need:
       - pic_count - Updated every time
         the programme starts to dump data
         to a new JPEG file.
       - block_size - It is stated that the size
         of each data block is 512 bytes.
       - b - An array of 512 bytes that records
         the sequence that is read in each iteration
         of the 'while' loop.
       - pic - A 4-character array that will be used
         for the conversion from integer to string.
       - out_name - Name of an output file.
       - a_file_is_open - A variable that tells
         whether a file is open for output (1) or not (0).
       - output_file - Not initalised before the loop
         as we start with no output files open. */
    int pic_count = -1;
    int block_size = 512;
    BYTE b[block_size];
    char pic[4];
    char *out_name;
    char *extension = ".jpg";
    uint8_t a_file_is_open = 0;
    FILE *output_file;

    /* Loop that will help us fetch blocks of data
       and separate them correctly. */
    while (fread(&b, sizeof(BYTE), block_size, input_file) == block_size)
    {
        /* Identification of the starting sequence of a JPEG image. */
        // If the first three bytes of a chunk are 0xff, 0xd8 and 0xff,
        // and the fourth is between 0xe0 and 0xef...
        if ((b[0] == 0xff) && (b[1] == 0xd8) && (b[2] == 0xff) && (b[3] >= 0xe0) && (b[3] <= 0xef))
        {
            printf("Beginning of picture.\n");

            // ... Then it's time to start dumping data to a new file.
            if(a_file_is_open == 1)
            {
                // We will start by closing the current output file.
                fclose(output_file);
                printf("    Previous file is closed.\n");
                // Now we have no output files open.
                a_file_is_open = 0;
                printf("    Marked.\n");
            }
            // Then we must increase 'pic_count' by 1
            // and use its value to update 'out_name'.
            pic_count++;
            printf("    'pic_count' increased.\n");
            sprintf(pic, "%0.3i", pic_count);
            printf("    'pic' rewritten.\n");
            out_name = strcat(pic, extension);
            printf("    Created name of new file.\n");

            // Time to open a new file an inform about it.
            output_file = fopen(out_name, "w");
            printf("    New file open.\n");
            if (output_file == NULL)
            {
                printf("        UNABLE TO OPEN NEW FILE.\n");
                return 1;
            }
            a_file_is_open = 1;
            printf("    Marked.\n");
        }
        else
        {
            printf("Not a beginning.\n");
        }
        if (a_file_is_open)
        {
            // Write the block to the output file.
            printf("    Dumping data...");
            fwrite(&b, sizeof(BYTE), block_size, output_file);
            printf(" Done.\n");
        }
    }
    /*
    char *c[3];
    c[0] = &out_name[0];
    int x = (int) strtol(c[0], NULL, 10);
    printf("%i\n", x);
    */

/*
    int a = 12;

    int size = (ceil(log10(a)) + 1) * sizeof(char);

    char c[4];
    sprintf(c, "%0.3i", a);

    char *d = "aaa";
    char *f;
    f = strcat(c, d);

    printf("%s\n", f);
*/

    // Number of characters in array of characters: (int)((ceil(log10(number))+1)*sizeof(char)

    // Join strings with 'strcat'.

    /* End of main function. */
    fclose(input_file);
    fclose(output_file);
    return 0;
}
