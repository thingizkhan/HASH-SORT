#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <chrono> 
#include <limits> 
#include <sstream> // Include this header for std::stringstream
#include"termProject.hpp"
#include"sorting.hpp"

void Sorting::swap(hashNode* a, hashNode* b) {
    hashNode t = *a;
    *a = *b;
    *b = t;
}

void Sorting::insertionSort(std::vector<hashNode>& arr, int low, int high) {
    
    for (int i = low + 1; i <= high; i++) {
        hashNode key = arr[i];
        int j = i - 1;

        // Move elements of arr[low..i-1], that are greater than key, to one position ahead of their current position
        while (j >= low && arr[j].value > key.value) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

int Sorting::partition(std::vector<hashNode>& arr, int low, int high) {
    int pivot = arr[high].value; // pivot
    int i = (low - 1); // Index of smaller element

    for (int j = low; j <= high - 1; j++) {
        // If current element is smaller than the pivot
        if (arr[j].value < pivot) {
            i++; // increment index of smaller element
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}
void Sorting::quickSort(std::vector<hashNode>& arr, int low, int high) {
    // If the segment size is less than 21, use insertion sort
    if (high - low < 21) {
        insertionSort(arr, low, high);
    } 
   else if (low < high) {
        // pi is partitioning index, arr[p] is now at right place
        int pi = partition(arr, low, high);

        // Sort elements after partition
        quickSort(arr, pi + 1, high);

        // Conditionally sort elements before partition if segment size is less than 20
        
    }
}