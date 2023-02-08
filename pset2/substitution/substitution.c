#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


int main(int argc, string argv[])
{
    int shift[26]; // variable to store key shift respective to their index

    if (argc == 1) // check if user does not enter any argument
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    else if (argc > 2) // check if user input more than 1 argument
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    else if (strlen(argv[1]) != 26) // check if the key argument is not 26 characters
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    else if (strlen(argv[1]) == 26) // check if the key argument is 26 characters
    {
        string key = argv[1]; // store user argument in 'key' variable

        // checking if input is not alphabet and if there is duplicate letter
        for (int n = 0; n < 26; n++)
        {
            if (isalpha(argv[1][n]) == 0) // if not alpha
            {
                return 1; // exit from main with error code 1
            }

            // check for duplicate value
            for (int i = 0; i < n; i++)
            {
                if (key[n] == key[i]) //comparing current value with all value within key
                {
                    printf("Duplicate value!\n");

                    return 1;
                }
            }

            // making shift array that containts character shift respective to their letter
            char key_lower = tolower(key[n]);
            shift[n] = key_lower - (97 + n);
        }
    }

    // get plaintext from user
    string plaintext = get_string("plaintext: ");

    // get plaintext length
    int plain_length = strlen(plaintext);

    // initiate empty char array to hold ciphered character
    char ciphertext[100];

    // start ciphering
    for (int k = 0; k < plain_length; k++)
    {
        int index = 0;
        char kar = plaintext[k];

        if (islower(kar)) // check if plaintext character is lowercase
        {
            index = kar - 97; // find character index between 0-25
            kar = kar + shift[index]; // update the character to ciphered character according to it's shift
            ciphertext[k] = kar; // append the ciphered character to ciphertext

        }
        else if (isupper(kar)) // check if plaintext character is uppercase
        {
            index = kar - 65; // find character index between 0-25
            kar = kar + shift[index]; // update the character to ciphered character according to it's shift
            ciphertext[k] = kar; // append the ciphered character to ciphertext
        }
        // if character does not qualify as uppercase or lowercase character then it's appended directly
        else
        {
            ciphertext[k] = kar;
        }

    }

    printf("ciphertext: %s\n", ciphertext);

    return 0;

}
