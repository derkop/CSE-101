/********************************************************************************* 
* Derrick Ko, dejko 
* 2023 Spring CSE101 PA# 5
* Shuffle.cpp
* Main function for List.cpp
*********************************************************************************/ 

#include <stdexcept>
#include <iomanip>

#include "List.h"

using namespace std;

void shuffle(List& D) {
    List A, B;
    int x, y;
    int n = D.length();
    // Split D into halves
    D.moveFront();
    while (D.position() < n / 2) {
        x = D.moveNext();
        A.insertBefore(x);
    }   
    while (D.position() < n) {
        y = D.moveNext();
        B.insertBefore(y);
    }
    // Reset D and shuffle A and B back into D
    D.clear();
    A.moveFront();
    B.moveFront();
    // Shuffle elements from A and B back into D
    while (A.position() < A.length() && B.position() < B.length()) {
        x = B.moveNext();
        D.insertBefore(x);

        y = A.moveNext();
        D.insertBefore(y);
    }
    // Add remaining elements from A or B if any
    while (A.position() < A.length()) {
        y = A.moveNext();
        D.insertBefore(y);
    }
    while (B.position() < B.length()) {
        x = B.moveNext();
        D.insertBefore(x);
    }
}

int main(int argc, char* argv[]) {
    int n, count, num;
    List Deck, Temp;
    // Check for correct number of arguments
    if (argc != 2) {
        throw invalid_argument("Incorrect number of arguments. Usage: Shuffle <int>");
    }
    // Input for max number of cards
    num = stoi(argv[1]);
    if (num <= 0) {
        throw invalid_argument("Argument must be a positive integer.");
    }
    // Header
    cout << "deck size\tshuffle count" << endl;
    string div(30, '-');
    cout << div << endl;    
    // Iterate through possible deck sizes
    for (n = 1; n <= num; n++) {
        count = 0;
        Deck.moveBack();
        Deck.insertBefore(n - 1);    
        // Shuffle once to start
        Temp = Deck;
        shuffle(Deck);
        count++;
        // Shuffle until deck matches original
        while (!(Temp == Deck)) {
            shuffle(Deck);
            count++;
        }
        // Output
        cout << " " << left << setw(16) << n << count << endl;
    }    
}
