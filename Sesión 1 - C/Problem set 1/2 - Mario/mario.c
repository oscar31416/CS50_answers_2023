#include <cs50.h>
#include <stdio.h>

// Auxiliary functions. Defined later.
int aux_getnumber(void);
void print_pyramid(int height);
void aux_print_pyramid(int j);

/* Main function. */
int main(void)
{
    // Get the desired height for the pyramid.
    int h = aux_getnumber();
    print_pyramid(h);
}

/* Auxiliary function that asks the user
   for an integer number between 1 and 8. */
int aux_getnumber(void)
{
    int n;
    do
    {
        n = get_int("Height of the pyramid:\n>>> ");
    }
    while ((n < 1) || (n > 8));
    return n;
}

/* Auxiliary function that prints the pyramid. */
void print_pyramid(int height)
{
    /* We need a local variable that stores,
       for each 'layer' (so to say), how many blocks
       the layer must have. The number of blank spaces
       can be calculated. */
    int o; // Number of blocks.
    int a; // Auxiliary variable that helps print each layer correctly.


    for (int i = 1; i <= height; i++)
    {
        a = height - i;
        /* We start with the blank spaces.
           If o == 0, we don't need to print blanks first. */
        for (o = a; o > 0; o--)
        {
            printf(" ");
        }
        /* Now we print the first set of blocks. */
        aux_print_pyramid(i);
        /* Two blank spaces come next... */
        printf("  ");
        /* ... And we repeat the same set of blocks
           before starting a new line. */
        aux_print_pyramid(i);
        printf("\n");
    }
}

void aux_print_pyramid(int j)
{
    for (int i = 1; i <= j; i++)
    {
        printf("#");
    }
}