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
            // ... Then it's time to start dumping data to a new file.
            if(a_file_is_open == 1)
            {
                // We will start by closing the current output file.
                fclose(output_file);

                // Now we have no output files open.
                a_file_is_open = 0;

            }

            // Then we must increase 'pic_count' by 1
            // and use its value to update 'out_name'.
            pic_count++;
            sprintf(pic, "%0.3i", pic_count);
            out_name = strcat(pic, extension);

            // Time to open a new file an inform about it.
            output_file = fopen(out_name, "w");

            if (output_file == NULL)
            {
                return 1;
            }

            a_file_is_open = 1;
        }
        if (a_file_is_open)
        {
            // Write the block to the output file.
            fwrite(&b, sizeof(BYTE), block_size, output_file);
        }
    }

    /* End of main function. */
    fclose(input_file);
    fclose(output_file);
    return 0;
}
