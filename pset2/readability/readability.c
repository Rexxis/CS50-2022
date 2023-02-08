#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int count_letters(string str);
int count_words(string str);
int count_sentences(string str);

int main(void)
{
    // get text input from user
    string text = get_string("Text: ");

    // getting letters, words and sentences amount
    int letters = count_letters(text);
    int word_count = count_words(text);
    int sentence_count = count_sentences(text);

    // getting letters per 100 words
    float L = ((float) letters / word_count) * 100;

    // getting sentence per 100 words
    float S = ((float) sentence_count / word_count) * 100;

    // calculating coleman-liau index, rounding to nearest integer
    int index = round((0.0588 * L) - (0.296 * S) - 15.8);

    // printing the grade accordingly
    if (index >= 1 && index < 16)
    {
        printf("Grade %i\n", index);
    }
    else if (index < 1) // if grade is less than 1
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade 16+\n"); // if grade 16 or higher
    }

}

// function to count letters in the text
int count_letters(string str)
{
    int length = strlen(str);
    int count = 0;

    for (int i = 0; i < length; i++)
    {
        // initialize the string to lower case for checking
        char str_lower = tolower(str[i]);

        // check if the char is lowercase alphabet a-z (excluding spaces and other characters)
        if (str_lower >= 97 && str_lower <= 122)
        {
            count += 1;
        }
    }

    return count;
}

// function to count words in the text
int count_words(string str)
{
    int length = strlen(str);
    int word_count = 1; // starting to count from 1, because the input must contain atleast 1 word

    for (int i = 0; i < length; i++)
    {
        if (str[i] == 32) // if text is separated by space, it is considered as 1 word
        {
            word_count += 1;
        }
    }

    return word_count;

}

// function to count sentences in the text
int count_sentences(string str)
{
    int length = strlen(str);
    int sentence_count = 0;

    for (int i = 0; i < length; i++)
    {
        if (str[i] == 46 || str[i] == 33 || str[i] == 63) // if separated by period / ! / ? considered as 1 sentence
        {
            sentence_count += 1;
        }
    }

    return sentence_count;
}