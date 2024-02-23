#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;

inline string deleteWhiteSpace(string str) {
    str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
    return str;
}

inline vector<vector<string>> read_csv(std::string filename) {
    std::vector<std::vector<std::string>> data;
    std::ifstream file(filename);
    if (!file.is_open()) throw std::runtime_error("Could not open file");
    std::string line;

    while (std::getline(file, line)) {
        std::vector<std::string> row;
        std::stringstream ss(line);
        std::string cell;
        while (std::getline(ss, cell, ',')) {
            cell = deleteWhiteSpace(cell);
            row.push_back(cell);
        }
        data.push_back(row);
    }
    file.close();
    return data;
}