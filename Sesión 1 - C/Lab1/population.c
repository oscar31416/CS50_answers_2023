#include <cs50.h>
#include <stdio.h>

/* Custom auxiliary function.
   Defined after the main function. */
int return_time(int i, int f);

/* Main function. */
int main(void)
{
    // DONE - Prompt for start size
    /* Requirements: the initial size must be...
    - An integer number.
    - Greater than, or equal to, 9. */
    int ini;
    do
    {
        ini = get_int("Initial size: ");
    }
    while (ini < 9);

    // DONE - Prompt for end size
    /* Requirements: the final size must be...
    - An integer number.
    - Greater than, or equal to, the initial size. */
    int end;
    do
    {
        end = get_int("Final size: ");
    }
    while (end < ini);

    // DONE - Calculate number of years until we reach threshold
    int years = return_time(ini, end);

    // DONE - Print number of years
    printf("Years: %i\n", years);
}

/* Auxiliary function that calculates how many years have elapsed.
   Requires initial and final sizes. */
int return_time(int i, int f)
{
    /* We need a variable that keeps track of the current size.
       Its initial value will be 'ini'. */
    int size = i;

    /* Another variable will tell us how many years have elapsed.
       Its initial value will be 0 (zero). */
    int yrs = 0;

    /* We're now ready to start the calculation process. */
    // If the initial and final sizes are equal,
    // we need to calculate nothing.
    if (i == f)
    {
        return yrs;
    }
    // Otherwise...
    else
    {
        while (size < f)
        {
            // Considering that 'n' represent the current size,
            // n/3 llamas are born and n/4 die every year.
            size = size + (size / 3) - (size / 4);
            yrs++;
        }
        /* Ready to return the result. */
        return yrs;
    }
}