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

def search_for_word(path, word):
    for filename in os.listdir(path):
        filename = os.path.join(path, filename)
        if os.path.isfile(filename):
            line = 0
            with open(filename, 'r', encoding='utf-8') as f:
                for l in f:
                    line += 1
                    for w in l.split():
                        if not sensitive:
                            w = w.lower()
                            word = word.lower()
                        if w == word:
                            # change / for \ in the below lines if you run windows :)
                            print('.' + filename.split('/.')[1] + ' - ' + str(line) if display_line else '.' + filename.split('/.')[1]) # will only work for linux since windows uses \ instead of /

        elif os.path.isdir(filename):
            if recursive:
                search_for_word(filename, word)
            

if __name__ == "__main__":
    opts, args = getopt.getopt(sys.argv[1:], "rcil?")
    for opt, arg in opts:
        if opt == '-r':
            recursive = True
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
