#ifndef FILEIO_H
#define FILEIO_H

#include <fstream>
#include <string>
#include <functional>
#include <stdexcept>
#include <vector>
using namespace std;

template <typename T, typename Formatter>
void saveToFile(const string& filename, const vector<T>& items, Formatter format) {
    ofstream file(filename);
    if (!file) {
        throw runtime_error("Failed to open '" + filename + "' for writing.");
    }
    for (const auto& item : items) {
        file << format(item) << endl;
    }
}

template <typename T, typename Parser>
vector<T> loadFromFile(const string& filename, Parser parse, bool required = true) {
    ifstream file(filename);
    vector<T> items;
    if (!file) {
        if (required) {
            throw runtime_error("Failed to open '" + filename + "'.");
        } else {
            return items;
        }
    }
    string line;
    int lineNo = 0;
    while(getline(file, line)){
        ++lineNo;
        if (!line.empty() && line.back() == '\r'){
            line.pop_back();
        }
        if (line.empty()) continue;
        try{
            items.push_back(parse(line));
        }catch(const exception& e) {
            throw runtime_error(filename + ", line " + to_string(lineNo) + ": " + e.what());
        }
    }
    return items;
}

#endif