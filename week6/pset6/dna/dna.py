import csv
import sys


def main():

    # Check for command-line usage
    if len(sys.argv) != 3:
        print("usage python ./dna.py [database.csv] [sequences.txt]")
        sys.exit(1)

    # Read database file into a variable
    database = []

    with open(sys.argv[1], "r") as db_path:
        reader = csv.DictReader(db_path)
        for data in reader:
            database.append(data)

    # get list of subsequences only
    subsequences = list(database[0].keys())[1:]

    # load sequences data to memory
    seq_file = open(sys.argv[2], "r")
    sequence = seq_file.read().rstrip()

    # get longest match per subsequences from sequences input data
    scan_result = {}
    for seq in subsequences:
        scan_result[seq] = longest_match(sequence, seq)

    # loop through every person
    for i in range(len(database)):
        # variable to count match
        subseq_match = 0

        # compare every person with every subsequences
        for seq in subsequences:

            # if person's longest subsequence count match with scan result
            if int(database[i][seq]) == scan_result[seq]:
                subseq_match += 1

                # check if all the subsequences match
                if subseq_match == len(subsequences):
                    # if match, print name
                    print(database[i]["name"])
                    sys.exit(0)

    # if no match found
    print("No match")
    sys.exit(0)


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
