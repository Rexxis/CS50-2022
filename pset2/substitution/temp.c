#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


int main(int argc, string argv[])
{
    if (argc == 1)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    else if (argc > 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    else if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    else if (strlen(argv[1]) == 26)
    {
        for (int n = 0; n < 26; n++)
        {
            if (isalpha(argv[1][n]) == 0)
            {
                return 1;
            }


        }
    }

    string plaintext = get_string("plaintext: ");
    string key = argv[1];

    int plain_length = strlen(plaintext);
    char ciphertext[100];
    int shift[26];

    for (int i = 0; i < 26; i++)
    {
        char key_lower = tolower(key[i]);
        shift[i] = key_lower - (97 + i);

    }

    for (int k = 0; k < plain_length; k++)
    {
        int index = 0;
        char kar = plaintext[k];

        if (islower(kar))
        {
            index = kar - 97;
            kar = kar + shift[index];
            ciphertext[k] = kar;

        }
        else if (isupper(kar))
        {
            index = kar - 65;
            kar = kar + shift[index];
            ciphertext[k] = kar;
        }
        else
        {
            ciphertext[k] = kar;
        }

    }

    printf("ciphertext: %s\n", ciphertext);

    return 0;

}
