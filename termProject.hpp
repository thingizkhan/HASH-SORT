#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <vector>
#include <string>
#include <limits>

// Definition of the hashNode struct
struct hashNode {
    std::string name;
    int key;
    int value;
    bool occupied;

    hashNode() : key(0), value(0), occupied(false) {}
};

class hashTable {
public:
    // Constructor
    hashTable(std::size_t size = 25057);

    // Destructor
    ~hashTable() {
        // Clean up resources if needed
    }
    
    std::vector<hashNode>& getHashVector() { return hashVector; }
    std::size_t getSize() const { return hashVector.size(); }
    hashNode& getElement(std::size_t index) { return hashVector[index]; }
    const hashNode& getElement(std::size_t index) const { return hashVector[index]; }

 

    void getHash();
private:
    // Vector to store hash nodes
    std::vector<hashNode> hashVector;

       // Hash function declaration
    unsigned int pairedHash(const std::string& input);

    // Probing function declaration for collision resolution
    std::size_t probe(int key);

};

#endif // HASHTABLE_HPP
