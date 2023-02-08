#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Asking the user for name
    string name = get_string("What is your name? ");

    // Printing the name with string hello
    printf("hello, %s\n", name);
}