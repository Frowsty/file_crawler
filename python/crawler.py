import os
import sys
import getopt

# Current working directory
cwd = os.getcwd()

recursive = False
sensitive = False
display_line = False

#Relative directory
search_path = os.path.join(cwd,"..",'TestData')

# Search for the user specified word
def search_for_word(path, word):
    # Loop files in the directory / path specified to listdir function
    for filename in os.listdir(path):
        # Rename filename from just filename to complete path + filename
        filename = os.path.join(path, filename)
        # Check if it's a file we're working with
        if os.path.isfile(filename):
            line = 0
            # Open the file in order to read the contents of it
            with open(filename, 'r', encoding='utf-8') as f:
                # Loop line by line instead of grabbing all the content at once
                for l in f:
                    line += 1
                    # Loop word by word by splitting at space
                    for w in l.split(" "):
                        # If it's a case insensitive search, default to lower case everything for easy string comparisons
                        if not sensitive:
                            w = w.lower()
                            word = word.lower()
                        if w == word:
                            # change / for \ in the below lines if you run windows :)
                            print('.' + filename.split('/.')[1] + ' - ' + str(line) if display_line else '.' + filename.split('/.')[1]) # will only work for linux since windows uses \ instead of /
        # Check if we're working with a directory, if so recursively call this function again with the new directory as function argument
        elif os.path.isdir(filename):
            if recursive:
                search_for_word(filename, word)
            

if __name__ == "__main__":
    try:
        opts, args = getopt.getopt(sys.argv[1:], "rcil?")
    except getopt.GetoptError as err:
        print("ERROR: " + str(err))
        print("~Usage~\n\t-r {Recursive search}\n\t-c {Case sensitive}\n\t-i {Case insensitive}\n\t-l {Display line number}")
        exit(-1)

    for opt, arg in opts:
        if opt == '-r':
            recursive = Tru
        e
        if opt == '-c':
            sensitive = True
        if opt == '-i':
            sensitive = False
        if opt == '-l':
            display_line = True
        if opt == '-?':
            print("~Usage~\n\t-r {Recursive search}\n\t-c {Case sensitive}\n\t-i {Case insensitive}\n\t-l {Display line number}")
            exit(-1)

    print("------------------File Crawler----------------------")
    word = input("Please enter word to search for: ")
    search_for_word(search_path, word);
