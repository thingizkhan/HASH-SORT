
#ifndef SORTING_H
#define SORTING_H

#include "termProject.hpp"
#include <vector>

class Sorting {
public:
    static void quickSort(std::vector<hashNode>& arr, int low, int high);
    static void insertionSort(std::vector<hashNode>& arr);

private:
    static void swap(hashNode* a, hashNode* b);
    static int partition(std::vector<hashNode>& arr, int low, int high);
    static void insertionSort(std::vector<hashNode>& arr, int low, int high);
};

#endif 