#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <chrono> 






// int hashsize[]={11, 23, 47, 97, 197, 397, 797, 1597, 3203, 6421, 12853,
//     25717, 51437, 102877, 205759, 411527, 823117, 1646237, 3292489,
//     6584983, 13169977, 26339969, 52679969, 105359939, 210719881,
//     421439783, 842879579, 1685759167};

struct hashNode{
    std::string name;
    int key;
    int value;
    bool occupied;

};

std::vector<hashNode> hashVector(25057);//İf you get collusupons, you can increase the size by 1 in hashsize array


int currentHashSizeIndex = 0; // Index to keep track of the current hash size


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

unsigned int pairedHash(const std::string& input) {
    unsigned int hashValue = 0;
    for (char c : input) {
        hashValue += (hashValue << 3) - hashValue + c; // Equivalent to 7 * hashValue + c
    }
    return hashValue;
}










class hashTable{

private:
//table pointer
    hashNode* table;
public:
    hashTable(){
        table = new hashNode[25057];

    }
    ~hashTable(){
        delete[] table;
    }
};

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

int probe(int key) {
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








void getHash(){
   
    //GETLINE
    std::ifstream file("access_log.txt");
    std::string line;

    if(file.is_open()){
        while(getline(file, line)){

            
             std::size_t firstQuote = line.find('\"');
            std::size_t secondQuote = line.find('H');
           // if (firstQuote != std::string::npos && secondQuote != std::string::npos) {
                std::string request = line.substr(firstQuote + 5, secondQuote - firstQuote - 6);
              //std::cout << request << std::endl;
            //GEtLINE

            //   // Resize and rehash if load factor exceeds 0.5
            // if (hashVector.size() > 0 && (static_cast<double>(hashVector.size()) / hashsize[currentHashSizeIndex]) > 0.5) {
            //     resizeAndRehash();
            // }
                unsigned int hashValue = pairedHash(request);
                std::size_t index = probe(hashValue);

                if (index == -1) {
        // Handle the situation when no slot is found
        std::cerr << "Failed to find a free slot for the key: " << hashValue << std::endl;
        return; // or other error handling
    }

            if (!hashVector[index].occupied) {
                // New insertion
                hashVector[index].name = request;
                hashVector[index].key = hashValue;
                hashVector[index].occupied = true;
                hashVector[index].value = 1; // Initialize value for new entry
            } else {
                // Node already occupied, increment value
                hashVector[index].value++;
            }

               
        //}
        }
        file.close();
    }
    else{
        std::cout<<"Unable to open file"<<std::endl;
    }
}

// void insert(){}
// void get(){}
// void increaseSize(){}
//void proccesInChunks(){}//If you are going to process the data in chunks, you can use a hash table to store the data in the first chunk, and then use the hash table to process the data in the second chunk. This is a very common technique in MapReduce.



//SORTING------------------------------------------------------------------------------------------------------------------------------
void swap(hashNode* a, hashNode* b) {
    hashNode t = *a;
    *a = *b;
    *b = t;
}

void insertionSort(std::vector<hashNode>& arr, int low, int high) {
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


int partition(std::vector<hashNode> &arr, int low, int high) {
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

void quickSort(std::vector<hashNode> &arr, int low, int high) {

    // If the segment size is less than 20, use insertion sort
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












int main(){
    // // Initialize hashVector to the initial size
    // hashVector.resize(hashsize[currentHashSizeIndex]);
   // hashVector.reserve(25057);//25057 is the size of the hashVector

    std::cout<<"Hello World"<<std::endl;
     // Start the timer
    auto start = std::chrono::high_resolution_clock::now();

 getHash();
// //Open the file for writing
//   std::ofstream outFile("output.txt");

//     if (outFile.is_open()) {
//         for (size_t i = 0; i < hashVector.size(); i++) {
//             // Write to the file instead of console
//             outFile << hashVector[i].name << " " << hashVector[i].key <<" "<<hashVector[i].value<< std::endl;
//         }
//         outFile.close();  // Close the file when done
//     } else {
//         std::cout << "Unable to open file for writing." << std::endl;

//     }

    quickSort(hashVector, 0, hashVector.size()-1);

     // Display only the last 10 sorted values
    for (size_t i = hashVector.size() - 10; i < hashVector.size(); i++) {
        std::cout << hashVector[i].name << " " << hashVector[i].key << " " << hashVector[i].value << std::endl;
    }

    

    // Stop the timer
    auto stop = std::chrono::high_resolution_clock::now();

    // Calculate duration
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);



// Print the duration
    std::cout << "Time taken by QuickSort: " << duration.count() << " microseconds" << std::endl;
    
return 0;
}