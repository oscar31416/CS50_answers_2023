import sys

# Check command-line arguments.
if len(sys.argv) != 3:
    print("ERROR.")
    print("Usage: python3 speller.py /path/to/dictionary /path/to/text")
    sys.exit(1)

# Try to open the dictionary file and the target file.
try:
    dictionary_file = open(sys.argv[1], "r")
    text_file = open(sys.argv[2], "r")
except:
    print("ERROR.")
    print("Unable to open the dictionary and/or the target file.")
    sys.exit(2)

# Load content.
whole_text = (iter(line.split(" ")) for line in text_file.read().split("\n"))
dictionary = set(dictionary_file.read().split("\n"))
text_file.close()
dictionary_file.close()

# Function that uses the dictionary to check each word.
def check_word(word, dictionary):

    new_word = ""

    # Check each character.
    for c in word:

        # Allow letters and, if 'new_word' isn't empty, apostrophes.
        # An 'or' could be used here, but
        # letters are more common than apostrophes.
        if c.isalpha():
            new_word += c
        elif new_word and (c == "\'"):
            new_word += c

        # Ignore strings with numbers.
        elif c.isdigit():
            return None

        # In any other case,
        # consider the whole word to be collected.
        elif new_word:
            if not (new_word.lower() in dictionary):
                print(new_word)
            new_word = ""

    # We don't want any words to remain unchecked.
    if new_word:
        if not (new_word.lower() in dictionary):
            print(new_word)

# Check each word.
for line in whole_text:
    for word in line:

        # Ignore strings that are too long or blank.
        if word:
            if(len(word) < 45):
                check_word(word, dictionary)

# End programme.
sys.exit(0)
