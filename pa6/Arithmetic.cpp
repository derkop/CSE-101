/********************************************************************************* 
* Derrick Ko, dejko 
* 2023 Spring CSE101 PA# 6
* Arithmetic.cpp
* Main file for BigInteger.cpp
*********************************************************************************/ 

#include <fstream>
#include <iostream>

#include "BigInteger.h"

using namespace std;

int main(int argc, char* argv[]) {
    // Check for correct number of arguments
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <input file> <output file>" << endl;
        return EXIT_FAILURE;
    }

    // Open files for read and write
    ifstream in(argv[1]);
    ofstream out(argv[2]);
    if (!in) {
        cerr << "Error: Unable to open file " << argv[1] << " for reading" << endl;
        return EXIT_FAILURE;
    }

    // Read in strings for big integers
    string sA, sB;
    in >> sA >> sB;

    // Create BigInteger objects
    BigInteger A(sA);
    BigInteger B(sB);
    BigInteger two("2");
    BigInteger three("3");
    BigInteger nine("9");
    BigInteger sixteen("16");

    // Print out requested quantities

    // A
    out << A << endl << endl;

    // B
    out << B << endl << endl;

    // A + B
    out << A + B << endl << endl;

    // A - B
    out << A - B << endl << endl;

    // A - A
    out << A - A << endl << endl;

    // 3A - 2B
    out << (three * A) - (two * B) << endl << endl;

    // AB
    out << A * B << endl << endl;

    // A^2
    out << A * A << endl << endl;

    // B^2
    out << B * B << endl << endl;

    // 9A^4 + 16B^5
    out << (nine * (A * A * A * A)) + (sixteen * (B * B * B * B * B)) << endl << endl;

    // Close files
    in.close();
    out.close();

    return 0;
}
