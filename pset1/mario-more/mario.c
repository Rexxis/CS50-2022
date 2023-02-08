#include <cs50.h>
#include <stdio.h>

// initialize the function
void hash(int num, string str);

int main(void)
{
    // initialize variable for input storage and do while loop
    int n;
    do
    {
        // asking the user for input
        n = get_int("Height: ");
    }
    // validate to accept input only between 1-8 (inclusive)
    while (n <= 0 || n >= 9);

    // for each row
    for (int i = 0; i < n; i++)
    {
        int c = n - i - 2;

        // print empty space, left column
        hash(c, " ");

        // print left column
        hash(i, "#");

        // print empty space
        hash(1, " ");

        // print right column
        hash(i, "#");

        // go to next line
        printf("\n");
    }

}


// function to write columns with variable input
void hash(int num, string str)
{
    // initialize starting variable to 0
    int n = 0;

    while (n <= num)
    {
        // print column
        printf("%s", str);

        // increment the counter
        n += 1;
    }
}
