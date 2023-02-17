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

void search_for_words_default(std::string sp, std::string word)
{
    std::vector<std::string> folders;
    fs::path p;
    for (fs::directory_iterator itr(sp); itr != fs::directory_iterator(); ++itr)
    {
        if (itr->path().filename() != "." && itr->path().filename() != "..")
        {
            p = itr->path();
            if (fs::is_regular_file(p) && !p.empty())
            {
                std::ifstream file(p.string());
                std::string line;
                while (std::getline(file, line))
                {
                    std::stringstream data(line);
                    std::string temp;
                    while (getline(data, temp, ' '))
                    {
                        if (sensitive)
                        {
                            if (word == temp)
                            {
                                std::cout << p.string() << std::endl;
                            }
                        }
                        else 
                        {
                            std::transform(word.begin(), word.end(), word.begin(), [](unsigned char c){ return std::tolower(c); });
                            std::transform(temp.begin(), temp.end(), temp.begin(), [](unsigned char c){ return std::tolower(c); });
                            if (word == temp)
                            {
                                std::cout << p.string() << std::endl;
                            }
                        }
                    }
                }
            }
            else if (fs::is_directory(p))
            {
                folders.push_back(p.string());
            }
            else 
            {
                std::cout << "ERROR: " << p.string() << std::endl;
            }
        }
    }

    if (recursive)
    {
        for (auto f : folders)
        {
            search_for_words_default(f, word);
        }
    }
}

int main(int argc, char **argv)
{
    std::string search_path = "../TestData";
    std::string word;
    int opt;
    while ((opt = getopt(argc, argv, "icr?")) != -1)
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
        case '?':
            std::cout << "Usage: " << argv[0] << "\n\t-i {Case insensitive search}\n\t-c {Case sensitive search}" << std::endl;
            exit(-1);
            break;
        default:
            std::cout << "Usage: " << argv[0] << "\n\t-i {Case insensitive search}\n\t-c {Case sensitive search}" << std::endl;
            exit(-1);
            break;
        }
    }

    std::cout << "------------------------------\nFile Crawler\n------------------------------" << std::endl;
    std::cout << "Please enter word to search for: ";
    std::cin >> word;

    search_for_words_default(search_path, word);
}
