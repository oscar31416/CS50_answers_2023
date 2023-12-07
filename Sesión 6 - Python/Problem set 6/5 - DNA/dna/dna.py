import csv
import sys


def main():

    # DONE - Check for command-line usage
    if len(sys.argv) != 3:
        print("Correct usage: python (or 'python3') dna.py path/to/database.csv path/to/sequence")
        sys.exit(1)

    # DONE - Read database file into a variable
    try:
        database_file = open(sys.argv[1], "r")
    except:
        print("ERROR. Unable to open the database file.")
        sys.exit(1)

    data = csv.DictReader(database_file)
    data_list = []
    for row in data:
        new_person = {}
        for item in row.keys():
            try:
                new_person[item] = int(row[item])
            except:
                new_person[item] = row[item]
        data_list.append(new_person)

    database_file.close()
    del database_file
    del data
    del new_person

    # DONE - Read DNA sequence file into a variable
    try:
        sequence_file = open(sys.argv[2], "r")
    except:
        print("ERROR. Unable to open sequence file.")
        sys.exit(1)

    try:
        sequence = sequence_file.read()
        sequence.replace("\n", "")
        sequence_file.close()
        del sequence_file
    except:
        sequence_file.close()
        print("ERROR while trying to read the sequence file.")
        sys.exit(1)

    # DONE - Find longest match of each STR in DNA sequence

    # Marker that tells, after the loop, if a match has been detected.
    match_found = False
    for person in data_list:

        # The following counter is decreased by 1 for every sequence count
        # that matches the person's expected count.
        auxiliary_counter = len(data_list[0].keys()) - 1
        for subsequence in person.keys():
            if subsequence != "name":
                if longest_match(sequence, subsequence) == person[subsequence]:
                    auxiliary_counter -= 1

        # If the auxiliary counter reaches 0, there's a match.
        if auxiliary_counter == 0:
            print(person["name"])
            match_found = True

    # If no matches have been detected, inform.
    if not (match_found):
        print("No match")

    return


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
