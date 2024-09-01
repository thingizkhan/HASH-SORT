#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <chrono> 
#include <limits> 
#include <sstream> // Include this header for std::stringstream
#include"termProject.hpp"
#include"sorting.hpp"


// int hashsize[]={11, 23, 47, 97, 197, 397, 797, 1597, 3203, 6421, 12853,
//     25717, 51437, 102877, 205759, 411527, 823117, 1646237, 3292489,
//     6584983, 13169977, 26339969, 52679969, 105359939, 210719881,
//     421439783, 842879579, 1685759167};



//CONSTRUCTOR------------------------------------------------------------------------------------------------------------------------------------

hashTable::hashTable(std::size_t size) : hashVector(size) {
    // Constructor body, if any initialization logic is required
}



//HASH FUNCTION------------------------------------------------------------------------------------------------------------------------------------
// std::size_t pairedHash(const std::string& input) {
//     std::size_t hashValue = 0;
//     // Process two characters at a time
//     for (std::size_t i = 0; i < input.length(); i += 2) {
//         // Combine the current two characters
//         char c1 = input[i];
//         char c2 = i + 1 < input.length() ? input[i + 1] : 0; // Use 0 for padding if necessary
//         std::size_t pair = (static_cast<std::size_t>(c1) << 8) | c2;
        
//         // Combine the pair with the current hash value
//         hashValue = (hashValue << 16) ^ (hashValue >> 16) ^ pair;
//     }
//     return hashValue;
// }

// int pairedHash(const std::string& input) {//öyle bir hash fonksiyonu yazın ki, aynı stringler aynı keye sahip olsun
//     int hashValue = 0;
//     for (char c : input) {
//         // Shift the current hash value to the left and add the character's ASCII value.
//         hashValue += (hashValue << 5) ^(hashValue >> 27) ^ c;
//     }
//     return hashValue;
// }
unsigned int hashTable::pairedHash(const std::string& input) {
    unsigned int hashValue = 0;
    for (char c : input) {
        hashValue += (hashValue << 3) - hashValue + c; // Equivalent to 7 * hashValue + c
    }
    return hashValue;
}



//PROBE FUNCTION------------------------------------------------------------------------------------------------------------------------------------
// int probe(int key) {
//     const int maxAttempts = hashVector.size(); // Maximum attempts equal to the size of the hash table
//     int i = 0;

//     while (i < maxAttempts) {
//         int index = (key + i * i) % hashVector.size();
//         if (!hashVector[index].occupied || hashVector[index].key == key) {
//             return index;
//         }
//         i++;
//     }


//     // Return a special value indicating failure to find a slot
//     return -1;
// }

std::size_t hashTable::probe(int key) {
    const std::size_t maxAttempts = hashVector.size();
    std::size_t i = 0;
    while (i < maxAttempts) {
        std::size_t index = (key + i * i) % hashVector.size();
        if (!hashVector[index].occupied || hashVector[index].key == key) {
            return index;
        }
        i++;
    }
    return std::numeric_limits<std::size_t>::max(); // Special value indicating failure
}




//RESIZE FUNCTION------------------------------------------------------------------------------------------------------------------------------------
// // Function to resize and rehash the hashVector
// void resizeAndRehash() {
//     // Check if we have more sizes to grow into
//     if (currentHashSizeIndex >= sizeof(hashsize)/sizeof(hashsize[0]) - 1) {
//         std::cout << "Maximum hash size reached, cannot resize further." << std::endl;
//         return;
//     }

//     // Move to the next size
//     int newSize = hashsize[++currentHashSizeIndex];
//     std::vector<hashNode> newHashVector(newSize);

//     // Rehash all elements
//     for (const auto& node : hashVector) {
//         if (node.occupied) {
//             // Compute the new index for each occupied node
//             int newIndex = probe(node.key) % newSize;
//             newHashVector[newIndex] = node; // Place it in the new hash table
//         }
//     }

//     // Replace the old hash table with the new one
//     hashVector = std::move(newHashVector);
// }


//PARALEL PROGRAMMING------------------------------------------------------------------------------------------------------------------------------------
// void processChunk(const std::string& chunk) {
//     std::stringstream stream(chunk);
//     std::string line;

//     while (std::getline(stream, line)) {
//         // Process each line in the chunk
//         std::size_t firstQuote = line.find('\"');
//         std::size_t secondQuote = line.find('H', firstQuote + 1);
//         if (firstQuote != std::string::npos && secondQuote != std::string::npos) {
//             std::string request = line.substr(firstQuote + 5, secondQuote - firstQuote - 6);

//             unsigned int hashValue = pairedHash(request);
//             std::size_t index = probe(hashValue);

//             if (index == std::numeric_limits<std::size_t>::max()) {
//                 std::cerr << "Failed to find a free slot for the key: " << hashValue << std::endl;
//                 continue;
//             }

//             // Critical section to ensure thread safety
//             #pragma omp critical
//             {
//                 if (!hashVector[index].occupied) {
//                     hashVector[index].name = request;
//                     hashVector[index].key = hashValue;
//                     hashVector[index].occupied = true;
//                     hashVector[index].value = 1;
//                 } else {
//                     hashVector[index].value++;
//                 }
//             }
//         }
//     }
// }
//I used paralel programming to process the data in chunks but it is not working faster so I commented it out



void hashTable::getHash() {
    std::ifstream file("access_log.txt", std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Unable to open file" << std::endl;
        return;
    }

    const std::size_t bufferSize = 16384;
    std::vector<char> buffer(bufferSize);
    std::string line, leftover;

    while (file.read(buffer.data(), bufferSize) || file.gcount() > 0) {
        std::size_t bytesRead = file.gcount();
        std::stringstream stream(leftover + std::string(buffer.data(), bytesRead));
        leftover.clear();

        while (std::getline(stream, line)) {
            std::size_t firstQuote = line.find('\"');
            std::size_t secondQuote = line.find('H', firstQuote + 1);
            if (firstQuote != std::string::npos && secondQuote != std::string::npos) {
                std::string request = line.substr(firstQuote + 5, secondQuote - firstQuote - 6);
                unsigned int hashValue = pairedHash(request);
                std::size_t index = probe(hashValue);

                if (index == std::numeric_limits<std::size_t>::max()) {
                    std::cerr << "Failed to find a free slot for the key: " << hashValue << std::endl;
                    continue;
                }

                if (!hashVector[index].occupied) {
                    hashVector[index].name = request;
                    hashVector[index].key = hashValue;
                    hashVector[index].occupied = true;
                    hashVector[index].value = 1;
                } else {
                    hashVector[index].value++;
                }
            }
        }

    }

    file.close();
}



int main() {
    auto start = std::chrono::high_resolution_clock::now();
    hashTable hash;
    hash.getHash();
    Sorting::quickSort(hash.getHashVector(), 0, hash.getSize() - 1);

 
    for (size_t i = hash.getSize()-1 ; i > hash.getSize()-11; --i) {
        const hashNode& node = hash.getElement(i);
        std::cout << node.name << ", Count: " << node.value << std::endl;
    }

    // Note: Implement quickSort and size methods or modify the approach
    // Sorting::quickSort(hashVector, 0, hashVector.size() - 1);

    // Display sorted values logic here... 

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    
  // Display only the last 10 sorted values
    

    std::cout << "Time taken by QuickSort: " << duration.count() << " microseconds" << std::endl;

    return 0;
}