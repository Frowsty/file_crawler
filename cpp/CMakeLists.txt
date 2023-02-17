cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(FileCrawler)

## Set your file here
add_executable(FileCrawler crawler.cpp)

target_compile_features(FileCrawler PRIVATE cxx_std_17)
