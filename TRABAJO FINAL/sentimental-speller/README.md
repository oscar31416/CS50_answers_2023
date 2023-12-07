# AN ATTEMPT TO BEAT "SPELLER50" IN PYTHON

## Video demo.

https://drive.google.com/file/d/1igkeVxfJLFtaQ8X2tmvcH3VFdddHnHJp/view?usp=drive_link

## Description.

I had failed to defeat "speller50" in C, so I decided to try again but in Python. Just like "speller50", the resulting code might not work appropriately if single quotation marks are used incorrectly, but I have managed to make it FLY!


### Required files.

· speller.py - The Python code.

· A plain text file that will be used as a dictionary. Its words MUST be separated from each other by new-line characters.

· A plain text file whose spelling will be checked.

### How is the programme used?

Its usage is simple.

#### On Windows.

If you have a Python 3 interpreter installed, you only need to open a command prompt, move to the directory where the Python code is located and type the following:



python speller.py C:\\path\\to\\dictionary C:\\path\\to\\file



In order to avoid possible problems, please type the full path to each text file and remember to use two backslashes instead of just one when writing paths; for example, "C:\\Users\\username\\dictionary".

#### On Linux.

The command is a bit different, and you might want to make sure you have a Python 3 interpreter installed. If you want to use your default interpreter, you need to open a terminal, move to the directory where the Python code is located an type the following:



python3 speller.py path/to/dictionary path/to/file



However, if you have installed a different interpreter - for example, Python 3.12 while your distro's default version is 3.10 -, you can use it by typing, continuing with this example,



python3.12 speller.py path/to/dictionary path/to/file



### Why did you...

#### ... load the dictionary as a list but convert it to a set afterwards?

The conversion to a set works like the hashing function we had to deal with as part of the 5th problem set. If we chose to keep the dictionary as a list, the process of looking for a word would consist of, literally, comparing the word to each and every single item in the dictionary... And that's very slow. By converting the dictionary to a set, we're using a hashing function behind the scenes, so it is a lot faster.

#### ... load the target text in such a strange way?

Each line of text is separated from the next by a new-line character (\n), and words are assumed to be separated by blank spaces.

If the question is related to the usage of a generator expression that returns iterators, the decision was made after experimenting with lists and iterators: I simply found out it was faster.

#### ... define a function that contained the process of checking each string instead of simply writing that code inside the nested 'for' loops?

After a few experiments, I noticed that it worked visibly faster that way.

#### ... place the instructions that open the dictionary and target files inside a 'try-except' block?

If I didn't, the user wouldn't know why the programme crashed while attempting to open files.
