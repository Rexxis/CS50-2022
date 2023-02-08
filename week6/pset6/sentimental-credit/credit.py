from sys import exit
from cs50 import get_string

while True:
    card = get_string("Number: ")
    if len(card) >= 10:
        break

# saves card length
card_length = len(card)

# card prefixes data
mastercard = ["51", "52", "53", "54", "55"]
amex = ["34", "37"]

# store digits that added directly
added_digits_total = 0

# store other digits that already multiplied by 2 as string
multiplied_digits = ""

# variable to store sum of multiplied_digits
multiplied_total = 0

# loop card number
for i in range(card_length):
    # determine if digit to be added directly, type cast to int
    if i % 2 == 0:
        added_digits_total += int(card[(card_length - i - 1):(card_length - i)])

    # if digit is not added directly, then multiplied by 2, saved as string
    else:
        multiplied_digits += str(2 * int(card[(card_length - i - 1):(card_length - i)]))

# get multiplied total
for multiplied in multiplied_digits:
    multiplied_total += int(multiplied)

# added all total
grand_total = multiplied_total + added_digits_total

# if grand total ended with 0, check card prefix
if grand_total % 10 == 0:
    # check if visa
    if card[0] == "4" and (card_length >= 13 and card_length <= 16):
        print("VISA")
        exit(0)

    # check if mastercard
    elif card[0:2] in mastercard and card_length == 16:
        print("MASTERCARD")
        exit(0)

    # check if amex
    elif card[0:2] in amex and card_length == 15:
        print("AMEX")
        exit(0)

    else:
        print("INVALID")
        exit(0)

# if grand total does not end with 0
else:
    print("INVALID")