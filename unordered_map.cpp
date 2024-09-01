#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <string> // Add this line
#include <chrono>
#include <ctime>

using namespace std::chrono;

struct LogEntry {
    std::string filename;
    int visits;
};

bool compareLogEntries(const LogEntry& entry1, const LogEntry& entry2) {
    return entry1.visits > entry2.visits;
}

int main() {
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    std::ifstream inputFile("access_log.txt"); // Change this line to "access_log.txt

    if (!inputFile.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    std::unordered_map<std::string, int> filenameVisits;

    std::string line;
    while (std::getline(inputFile, line)) {
        size_t startPos = line.find("GET ");
        if (startPos != std::string::npos) {
            size_t endPos = line.find(" HTTP");
            if (endPos != std::string::npos) {
                std::string filename = line.substr(startPos + 5, endPos - (startPos + 5));
                filenameVisits[filename]++;
            }
        }
    }

    inputFile.close();

    std::vector<LogEntry> logEntries;
    for (const auto& entry : filenameVisits) {
        logEntries.push_back({entry.first, entry.second});
    }

    std::sort(logEntries.begin(), logEntries.end(), compareLogEntries);

    std::cout << "Top 10 most visited web pages:" << std::endl;
    for (size_t i = 0; i < std::min(logEntries.size(), static_cast<size_t>(10)); ++i) {
        std::cout << logEntries[i].filename << "\t\t# of total visits: " << logEntries[i].visits << std::endl;
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    std::cout << "Total Elapsed Time: "<< time_span.count() <<"seconds" << std::endl;

    return 0;
}