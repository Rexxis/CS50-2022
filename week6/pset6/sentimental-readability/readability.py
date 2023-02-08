from cs50 import get_string
from sys import exit


def main():
    # get text from user
    text = get_string("Text: ")

    # get letters, words and sentences count
    letters = count_letters(text)
    word_count = count_words(text)
    sentence_count = count_sentences(text)

    # get letter per 100 words
    L = (letters / word_count) * 100

    # get sentence per 100 words
    S = (sentence_count / word_count) * 100

    # calculate coleman-liau index
    index = round((0.0588 * L) - (0.296 * S) - 15.8)

    # print grade according to index
    if index >= 1 and index < 16:
        print(f"Grade {index}")
    elif index < 1:
        print("Before Grade 1")
    else:
        print("Grade 16+")

    exit(0)


def count_letters(raw):
    # variable to store occurences
    count = 0

    # loop through every character
    for kar in raw:
        # check if a-z or A-Z
        if kar.isalpha():
            count += 1

    return count


def count_words(raw):
    # variable to store occurences
    word_count = 1

    # loop through every character
    for kar in raw:
        # check if space
        if kar.isspace():
            word_count += 1

    return word_count


def count_sentences(raw):
    # variable to store occurences
    sentence_count = 0

    # loop through every character
    for kar in raw:
        # check if character is period / ! / ?
        if kar == "." or kar == "!" or kar == "?":
            sentence_count += 1

    return sentence_count


# run main
main()
