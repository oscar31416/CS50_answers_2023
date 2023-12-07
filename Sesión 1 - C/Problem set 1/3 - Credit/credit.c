#include <cs50.h>
#include <stdio.h>

/* Auxiliary functons. Defined later. */
long get_ccn(void);
int sum1(long cn);
int sum2(long cn);
int cs(int cs1, int cs2);
void check_number(long cn);

/* Main function. */
int main(void)
{
    long ccn = get_ccn();      // Get the credit card number.
    long ccn_copy = ccn;       // Required to work with a copy of 'ccn'
                               // and avoid destroying the input by accident.
    int checksum1 = sum1(ccn); // Two variables are required to check sums.
    int checksum2 = sum2(ccn);
    int checksum3 = cs(checksum1, checksum2); // The sum is checked here.
    if (checksum3 == 0)
    {
        check_number(ccn); // Attempt to find out to which company the card belongs.
    }
    else
    {
        printf("INVALID\n");
    }
}

/* Auxiliary function that gets the credit card number.
   The user is forced to write a positive number or, at least, zero. */
long get_ccn(void)
{
    long x;
    do
    {
        x = get_long("Number: ");
    }
    while (x < 0);
    return x;
}

/* First check.
   Requires 'ccn'.
   Returns 'checksum1'. */
int sum1(long cn)
{
    int s = 0;    // Sum.
    long n = cn;  // Copy of 'ccn'.
    int aux1 = 0; // Auxiliary variable.
    while (n > 0)
    {
        /* The following operation takes the current value of 'n',
           extracts the second-to-last digit and
           multiplies it by two. */
        aux1 = ((n / 10) % 10) * 2;

        /* We now add n's digits to the current value of 's'.*/
        while (aux1 > 0)
        {
            s += (aux1 % 10);
            aux1 /= 10;
        }

        /* Now we get rid of n's last two digits and repeat the cycle. */
        n /= 100;
    }
    return s;
}

/* Second check. Slightly different from 'sum2'.
   Requires 'ccn'.
   Returns 'checksum2'. */
int sum2(long cn)
{
    int s = 0;   // Sum.
    long n = cn; // Copy of 'ccn'.
    while (n > 0)
    {
        /* The following operation takes the current value of 'n',
           extracts the last digit and adds it ot the current value of 's'. */
        s += (n % 10);

        /* Now we get rid of n's last two digits and repeat the cycle. */
        n /= 100;
    }
    return s;
}

/* Third check.
   Takes 'checksum1' and 'checksum2',
   adds them together and takes the remainder
   of the sum's division by 10. */
int cs(int cs1, int cs2)
{
    int sum = (cs1 + cs2) % 10;
    return sum;
}

/* The following function is intended to check whether the number corresponds to
   an American Express card number, a Visa one or a MasterCard one.
   "INVALID" will be returned if there's no match. */
void check_number(long cn)
{
    long n = cn; // Copy of 'ccn' that can be modified.

    /* Let's start with American Express.
       Its cards are identified with 15-digit numbers
       that always start with 34 or 37. */
    int amex = n / 1e13;
    int visa_short = n / 1e12;
    int visa_long = n / 1e15;
    int mastercard = n / 1e14;
    if ((n >= 1e14) && (n < 1e15) && ((amex == 34) || (amex == 37)))
    {
        printf("AMEX\n");
    }
    /* Now we'll move on to Visa's 13-digit cards.
       Visa's cards always start with 4. */
    else if ((n >= 1e12) && (n < 1e13) && (visa_short == 4))
    {
        printf("VISA\n");
    }
    /* Third, MasterCard's cards and some Visa cards
       are defined by 16-digit numbers. */
    else if ((n >= 1e15) && (n < 1e16) && (visa_long == 4)) // Visa
    {
        printf("VISA\n");
    }
    else if ((n >= 1e15) && (n < 1e16) && (mastercard >= 51) && (mastercard <= 55)) // MasterCard
    {
        printf("MASTERCARD\n");
    }
    /* Finally, if the input belongs matches none of the previous sets of conditions... */
    else
    {
        printf("INVALID\n");
    }
}