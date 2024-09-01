# Hash Table and Sorting Project

## Description
This project implements a hash table with collision resolution and sorting algorithms. It processes log files to count the frequency of requests and sorts them.

## Features
- Hash table with quadratic probing
- QuickSort and InsertionSort for sorting
- Processes log files in chunks
- Contains both Object-Oriented Programming (OOP) and monolithic code
- Parallel processing code (commented out)

## Main Goal
The main goal of this project is to find the top 10 most frequent requests from the log file. To optimize the performance, only the upper part of the QuickSort is sorted, resulting in a time complexity of O(n).

## Files
- `src/main.cpp`: Main entry point
- `src/hashTable.cpp`: Hash table implementation
- `src/sorting.cpp`: Sorting algorithms
- `include/hashTable.hpp`: Hash table header
- `include/sorting.hpp`: Sorting header
- `data/access_log.txt`: Sample log file

## Build and Run
g++ -Wall -Wextra -g3 termProject.cpp sorting.cpp -o output/termProject


## Dependencies
- C++11 or later

## Notes
- The project includes both OOP and monolithic code to demonstrate different coding styles.
- Parallel processing code is included but commented out as it did not result in performance improvements. (maybe if the data if bigger it can work)

