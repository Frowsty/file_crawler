#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <unistd.h>
#include <algorithm>

#include <unordered_map> // For bonus exercise
#include <bits/stdc++.h> // For bonus exercise, string manipulation

using namespace std;
namespace fs = std::filesystem;

bool sensitive = false;
bool recursive = false;
bool output_file = false;
bool display_line = false;
bool depth_check = false;
bool sub_search = false;
bool file_ext_search = false;
bool exclude_dir = false;
bool hashed_response = false;
int depth = 0;
std::string file_name = "results.txt";
std::vector<std::string> file_extensions;
std::vector<std::string> excluded_dir;

//hashtable
std::unordered_map<std::string, std::vector<std::string>> found_words;

// print the words from our hashtable
void print_from_hash(std::string word)
{
    if (!sensitive)
        std::transform(word.begin(), word.end(), word.begin(), [](unsigned char c){ return std::tolower(c); });

    for (auto& i : found_words[word])
        std::cout << i << "\n";
}

// this function will either print out from the hashtable or manually look through all the files AFTER the user has specified
// the word that should be looked up, if the user specified to use the hashtable method we have already indexed everything prior to user specifying the word
// and only have to print out the paths straight from the hashtable
void search_for_words_default(std::string sp, std::string word)
{
    // if we're using the hashing / indexing process just print from the hashtable, else continue with the normal processing
    if (hashed_response)
    {
        print_from_hash(word);
    }
    else
    {
        std::vector<std::string> folders;
        fs::path p;
        for (fs::directory_iterator itr(sp); itr != fs::directory_iterator(); ++itr)
        {
            if (itr->path().filename() != "." && itr->path().filename() != "..")
            {
                p = itr->path();
                std::string line_number;
                int i = 0;
                if (fs::is_regular_file(p) && !p.empty())
                {
                    if (sub_search)
                        continue;

                    if (file_ext_search)
                    {
                        if (!std::count(file_extensions.begin(), file_extensions.end(), fs::path(p).extension()))
                            continue;
                    }

                    std::ifstream file(p.string());
                    std::string line;
                    while (std::getline(file, line))
                    {
                        if (display_line)
                        {
                            i++;
                        }

                        std::stringstream data(line);
                        std::string temp;
                        while (getline(data, temp, ' '))
                        {
                            if (display_line)
                                line_number = " - " + std::to_string(i);

                            std::ofstream text_file(file_name, ios::app);
                            std::ostringstream ss;
                            if (!sensitive)
                            {
                                std::transform(word.begin(), word.end(), word.begin(), [](unsigned char c){ return std::tolower(c); });
                                std::transform(temp.begin(), temp.end(), temp.begin(), [](unsigned char c){ return std::tolower(c); });
                            }
                            if (word == temp)
                            {
                                if (output_file)
                                    ss << p.string() << line_number << "\n";
                                else
                                    std::cout << p.string() << line_number << std::endl;
                            }
                            text_file << ss.str();
                            text_file.close();
                        }
                    }
                }
                else if (fs::is_directory(p))
                {
                    if (exclude_dir)
                    {
                        bool match = false;
                        std::stringstream dir_words(p.string());
                        std::string word;

                        // change the delimiter from / to \\ if you're running windows, path will differ from Linux to Windows
                        while (getline(dir_words, word, '/'))
                        {
                            for (auto w : excluded_dir)
                            {
                                std::transform(word.begin(), word.end(), word.begin(), [](unsigned char c){ return std::tolower(c); });
                                std::transform(w.begin(), w.end(), w.begin(), [](unsigned char c){ return std::tolower(c); });
                                if (word == w)
                                    match = true;
                            }
                        }
                        if (match)
                            continue;
                        else
                            folders.push_back(p.string());
                    }
                    folders.push_back(p.string());
                }
                else 
                {
                    std::cout << "ERROR: " << p.string() << std::endl;
                }
            }
        }

        if (sub_search)
            sub_search = false;

        if (recursive)
        {
            if (depth_check)
            {
                while (depth != 0)
                {
                    depth--;
                    for (auto f : folders)
                    {
                        search_for_words_default(f, word);
                    }
                }
            }
            else 
            {
                for (auto f : folders)
                {
                    search_for_words_default(f, word);
                } 
            }
        }
    }
}

// index all of the words in the directories and store to a hashtable (unordered_map)
void word_indexing(std::string sp)
{
    std::vector<std::string> folders;
    fs::path p;
    // Iterate the directory
    for (fs::directory_iterator itr(sp); itr != fs::directory_iterator(); ++itr)
    {
        // check if the directory is not a hidden file or the previous directory (..)
        if (itr->path().filename() != "." && itr->path().filename() != "..")
        {
            p = itr->path();
            // check if it's a file and is not empty
            if (fs::is_regular_file(p) && !p.empty())
            {
                std::ifstream file(p.string());
                std::string line;
                // read each line from the file
                while (std::getline(file, line))
                {
                    std::stringstream data(line);
                    std::string temp;
                    // read each word from the file, we use space (' ') as the delimiter here
                    while(std::getline(data, temp, ' '))
                    {
                        // if the search is insensitive we default to lowercase for easy string comparison
                        if (!sensitive)
                            std::transform(temp.begin(), temp.end(), temp.begin(), [](unsigned char c){ return std::tolower(c); });
                        if (found_words.find(temp) != found_words.end())
                        {
                            // make sure we don't add duplicates from the same file (not sure if this was a requirement or not)
                            // if it's not a duplicate (in the same file that is) we add it to the unordered_map which is a hashtable
                            if (found_words[temp].back() != p.string())
                                found_words[temp].push_back(p.string());
                        }
                        else 
                            found_words[temp].push_back(p.string());
                    }
                }
            }
            // check if it's a directory, if it is add it to a vector of strings containing directories
            else if (fs::is_directory(p))
            {
                folders.push_back(p.string());
            }
        }
    }

    // Recursively search the directories
    for (auto& f : folders)
    {
         word_indexing(f);
    }
}

int main(int argc, char **argv)
{
    std::string search_path = "../TestData";
    std::string word;
    int opt;
    while ((opt = getopt(argc, argv, "icro:ld:st:e:h?")) != -1)
    {
        switch (opt)
        {
        case 'i':
            sensitive = false;
            break;
        case 'c':
            sensitive = true;
            break;
        case 'r':
            recursive = true;
            break;
        case 'o':
            output_file = true;
            file_name = optarg;
            remove(file_name.c_str());
            break;
        case 'l':
            display_line = true;
            break;
        case 'd':
            depth_check = true;
            depth = std::atoi(optarg);
            break;
        case 's':
            sub_search = true;
            break;
        case 't':
        {
            file_ext_search = true;
            std::string tmp;
            std::stringstream ss(optarg);

            while (std::getline(ss, tmp, ','))
                file_extensions.push_back('.' + tmp);
        }
            break;
        case 'e':
        {
            exclude_dir = true;
            std::string tmp;
            std::stringstream ss(optarg);

            while (std::getline(ss, tmp, ','))
               excluded_dir.push_back(tmp); 
        }
            break;
        case 'h':
            hashed_response = true;
            break;
        case '?':
            std::cout << "Usage: " << argv[0] << "\n\t-i {Case insensitive search}\n\t-c {Case sensitive search}\n\t-r {Recursive search}\n\t-o {Output file}\n\t-l {Display Lines}\n\t-d {Depth amount}\n\t-s {Subdirectory search}\n\t-t {File extensions}\n\t-e {Exclude directories}\n\t-h {Hashtable search}" << std::endl;
            exit(-1);
            break;
        default:
            std::cout << "Usage: " << argv[0] << "\n\t-i {Case insensitive search}\n\t-c {Case sensitive search}\n\t-r {Recursive search}\n\t-o {Output file}\n\t-l {Display Lines}\n\t-d {Depth amount}\n\t-s {Subdirectory search}\n\t-t {File extensions}\n\t-e {Exclude directories}\n\t-h {Hashtable search}" << std::endl;
            exit(-1);
            break;
        }
    }

    if (hashed_response)
        word_indexing(search_path);

    std::cout << "------------------------------\nFile Crawler\n------------------------------" << std::endl;
    std::cout << "Please enter word to search for: ";
    std::cin >> word;

    search_for_words_default(search_path, word);
}
