# Ask the user to type a string.
s = input("Text: ")

# Length of the string.
s_len = len(s)

# Initialise counters.
number_of_letters = 0
number_of_words = 1
number_of_sentences = 0

# Loop.
for i in range(s_len):
    if s[i].isalpha():
        number_of_letters += 1
    if s[i] == ' ':
        number_of_words += 1
    if s[i] in ['.', '?', '!']:
        number_of_sentences += 1

# Calculate the Coleman-Liau index.
l = (100.0 * number_of_letters) / number_of_words
s = (100.0 * number_of_sentences) /number_of_words

cli_f = 0.0588 * l - 0.296 * s - 15.8

# Round the index to the nearest integer.
cli_i = int(cli_f)

if (cli_f - cli_i >= 0.5):
    cli_i += 1

# Print the grade.
if (cli_i >= 16):
    print("Grade 16+")
elif (cli_i < 1):
    print("Before Grade 1")
else:
    print("Grade", cli_i)
