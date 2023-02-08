from cs50 import get_int

# get input from user
while True:
    n = get_int("Enter height: ")
    # if value is not 1-8, reprompt the user
    if n >= 1 and n <= 8:
        break

# create a loop with i starting with 1
for i in range(1, n + 1):
    # get variable to print white space
    c = n - i
    # print white space
    print(" " * c, end="")
    # print hash
    print("#" * i, end="")
    # print middle spaces
    print(" " * 2, end="")
    # print hash
    print("#" * i)