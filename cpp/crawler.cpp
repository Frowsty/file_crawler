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
int depth = 0;
std::string file_name = "results.txt";
std::vector<std::string> file_extensions;
std::vector<std::string> excluded_dir;

void search_for_words_default(std::string sp, std::string word)
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
                        if (sensitive)
                        {
                            if (word == temp)
                            {
                                if (output_file)
                                    ss << p.string() << line_number << "\n";
                                else
                                    std::cout << p.string() << line_number << std::endl;
                            }
                        }
                        else 
                        {
                            std::transform(word.begin(), word.end(), word.begin(), [](unsigned char c){ return std::tolower(c); });
                            std::transform(temp.begin(), temp.end(), temp.begin(), [](unsigned char c){ return std::tolower(c); });
                            if (word == temp)
                            {
                                if (output_file)
                                    ss << p.string() << line_number << "\n";
                                else
                                    std::cout << p.string() << line_number << std::endl;
                            }
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

int main(int argc, char **argv)
{
    std::string search_path = "../TestData";
    std::string word;
    int opt;
    while ((opt = getopt(argc, argv, "icro:ld:st:e:?")) != -1)
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

            while (getline(ss, tmp, ','))
                file_extensions.push_back('.' + tmp);
        }
            break;
        case 'e':
        {
            exclude_dir = true;
            std::string tmp;
            std::stringstream ss(optarg);

            while (getline(ss, tmp, ','))
               excluded_dir.push_back(tmp); 
        }
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
