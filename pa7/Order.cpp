/*********************************************************************************
* Derrick Ko, dejko
* 2023 Spring CSE101 PA# 7
* Order.cpp
* Main file for Dictionary.cpp
*********************************************************************************/

#include <fstream>

#include "Dictionary.h"

using namespace std;

int main(int argc, char* argv[]) {
    // Check for the correct number of arguments
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input file> <output file>" << std::endl;
        return EXIT_FAILURE;
    }

    // Open the input and output files
    ifstream infile(argv[1]);
    ofstream outfile(argv[2]);
    
    // Check if the input file is readable
    if (!infile.is_open()) {
        std::cerr << "Could not open the file " << argv[1] << std::endl;
        return EXIT_FAILURE;
    }

    // Check if the output file is writable
    if (!outfile.is_open()) {
        std::cerr << "Could not open the file " << argv[2] << std::endl;
        infile.close();
        return EXIT_FAILURE;
    }

    // Create a Dictionary object
    Dictionary D;

    std::string k = "";
    int v = 1;

    // Read the input file line by line
    while (std::getline(infile, k)) {
        D.setValue(k, v);
        v++;
    }

    // Write the contents of the Dictionary to the output file
    outfile << D.to_string() << std::endl;
    outfile << D.pre_string() << std::endl;

    // Clear the Dictionary
    D.clear();

    // Close the input and output files
    infile.close();
    outfile.close();

    return EXIT_SUCCESS;
}
