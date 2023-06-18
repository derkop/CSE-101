/*********************************************************************************
* Derrick Ko, dejko
* 2023 Spring CSE101 PA# 8
* WordFrequency.cpp
* Main file for Dictionary
*********************************************************************************/

#include <fstream>

#include "Dictionary.h"

using namespace std;

int main(int argc, char* argv[]) {
    size_t begin, end, len;
    ifstream in;
    ofstream out;
    string line, key;
    string delim = " \t\\\"\',<.>/?;:[{]}|`~!@#$%^&*()-_=+0123456789";
    Dictionary D;

    // Check for correct number of arguments
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <input file> <output file>" << endl;
        return EXIT_FAILURE;
    }

    // Open and check files
    in.open(argv[1]);
    out.open(argv[2]);

    if (!in.is_open()) {
        cerr << "Unable to open file " << argv[1] << " for reading" << endl;
        return EXIT_FAILURE;
    }

    if (!out.is_open()) {
        cerr << "Unable to open file " << argv[2] << " for reading" << endl;
        return EXIT_FAILURE;
    }

    // Read lines
    while (getline(in, line)) {
        len = line.length();

        // Get first key
        begin = min(line.find_first_not_of(delim, 0), len);
        end = min(line.find_first_of(delim, begin), len);
        key = line.substr(begin, (end - begin));

        // Key found
        while (key != "") {
            // Convert key to lower case
            for (size_t i = 0; i < key.length(); i++) {
                key[i] = tolower(key[i]);
            }

            // Add to dictionary
            if (D.contains(key)) {
                D.getValue(key)++;
            }
            else {
                D.setValue(key, 1);
            }

            // Get next token
            begin = min(line.find_first_not_of(delim, (end + 1)), len);
            end = min(line.find_first_of(delim, begin), len);
            key = line.substr(begin, (end - begin));
        }
    }

    // Output dictionary
    out << D << endl;
    
    // Clean up, close files
    D.clear();
    in.close();
    out.close();

    return EXIT_SUCCESS;
}
