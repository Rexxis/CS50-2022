#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // get credit card number
    long card;
    do
    {
        card = get_long("Number: ");
    }
    while (card < 0); // checks if card number not negative

    long card2 = card; // create card duplicate for operations

    int total = 0; // initialize total from 0
    int counter = 0; // initialize counter for pointer

    while (card2 > 0)
    {
        if ((counter % 2) == 1) // if counter is odd (card -> even)
        {
            // luhn multiply by 2 every other number
            int even = (card2 % 10) * 2;
            total += even % 10;
            total += even / 10;
        }
        else if ((counter % 2) == 0) // if counter is even (card -> odd)
        {
            // other number that is directly added to total
            total += card2 % 10;
        }
        card2 /= 10; // remove the last digit of card for next iteration
        counter++; // added the counter to check even or odd
    }

    // check if sum ends with 0
    if (total % 10 == 0)
    {
        // check if number is visa, starting with digit 4, 13 or 16 digits
        if (card / 1000000000000000 == 4 || card / 1000000000000 == 4)
        {
            printf("VISA\n");
        }

        // check if number is mastercard by starting digit 51 / 52 / 53 / 54 / 55
        else if (card / 100000000000000 == 51 || card / 100000000000000 == 52 || card / 100000000000000 == 53)
        {
            printf("MASTERCARD\n");
        }
        else if (card / 100000000000000 == 54 || card / 100000000000000 == 55) // separate from top for style50
        {
            printf("MASTERCARD\n");
        }
        else if (card / 10000000000000 == 34 || card / 10000000000000 == 37) // check start with 34 / 37 and total digits 13
        {
            printf("AMEX\n");
        }
        // for other numbers that ends with 0 but does not qualify card types
        else
        {
            printf("INVALID\n");
        }
    }
    // if sum does not end with 0
    else
    {
        printf("INVALID\n");
    }
}