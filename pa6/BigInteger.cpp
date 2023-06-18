/********************************************************************************* 
* Derrick Ko, dejko 
* 2023 Spring CSE101 PA# 6
* BigInteger.cpp
* BigInteger ADT
*********************************************************************************/ 

#include "BigInteger.h"
#include "List.h"

#include <cctype>
#include <cmath>
#include <stdexcept>

using namespace std;

const ListElement BASE  = 1000000000;  // 10^(power)
const int         POWER = 9;           // log_10(base)

// Helper Functions --------------------------------------------------------------------------------------------------
// Remove leading zeros
void delZero( List* L) {
    L->moveFront();
    while (L->length() > 0 && !L->peekNext()) {
        L->eraseAfter();
    }
}

List tempMult(long s, List *b, int* ctr) {
    List L;
    long carry = 0;
    long temp = 0;
    for (b->moveBack(); b->position() > 0; b->movePrev()) {
        temp = (b->peekPrev() * s) + carry;
        carry = temp / BASE;
        temp %= BASE;
        L.insertAfter(temp);
    }
    if (carry > 0) {
        L.insertAfter(carry);
    }
    L.moveBack();
    for (int i = 0; i < *ctr; i++) {
        L.insertAfter(0);
    }
    return L;
}

// BigInteger Constructors---------------------------------------------------------------------------------------------------------
BigInteger::BigInteger() {
    signum = 0;
    digits = List();
}

// Constructor that creates a new BigInteger from the long value x.
BigInteger::BigInteger(long x) {
    // Handle zero separately
    if (x == 0) {
        signum = 0;
        digits.insertAfter(0);
        return;
    }
    // Handle negative numbers
    if (x < 0) {
        signum = -1;
        x = -x;
    } else {
        signum = 1;
    }
    // Extract digits from the long value
    while (x > 0) {
        digits.insertBefore(x % BASE);
        x /= BASE;
    }
}

BigInteger::BigInteger(std::string s) {
    if (!s.length()) {
        throw std::invalid_argument("BigInteger: Constructor: empty string");
    }
    int offset = 0;
    if ((s[0] == '+') | (s[0] == '-')) {
        signum = 1;
        if (s[0] == '-') {
            signum = -1;
        }
        offset = 1;
        s = s.substr(offset, s.length() - 1);
    } else {
        signum = 1;
    }
    for (long unsigned int i = 0; i < s.length();) {
        if (!std::isdigit(s[i])) {
            throw std::invalid_argument("BigInt Constructor is non-numeric");
        }
        i = i + 1;
    }
    std::string num = "";
    List L;
    size_t max = 0;
    size_t curr_digits = s.length();
    // Divide the input string into chunks of POWER digits and convert each chunk to a number
    while (max < s.length() / POWER) {
        num = s.substr(curr_digits - POWER, POWER);
        digits.insertAfter(std::stol(num, nullptr, 10));
        curr_digits = curr_digits - POWER;
        max++;
    }
    // If there are remaining digits, convert them to a number and insert them
    if (curr_digits > 0) {
        num = s.substr(0, curr_digits);
        digits.insertAfter(std::stol(num, nullptr, 10));
    }

    delZero(&digits);  // Remove leading zeros from the digits list
}

BigInteger::BigInteger(const BigInteger& N) {
    this->signum = N.signum;
    this->digits = N.digits;
}

// Access Functions -----
int BigInteger::sign() const{
    return this->signum;
}

int BigInteger::compare(const BigInteger& N) const {
    // Simple case, compare signs
    if (signum > N.signum) {
        return 1;
    }
    else if (signum < N.signum) {
        return -1;
    }
    else if (signum == 0 && N.signum == 0) {
        return 0;
    }
    // Same sign, do checks
    const List& A = digits;
    const List& B = N.digits;
    int length = A.length();
    if (length != B.length()) {
        // If lengths differ, the longer list is larger
        return (length > B.length()) ? signum : -N.signum;
    }
    // Create copies of A and B to avoid modifying the originals
    List A_copy = A;
    List B_copy = B;
    // Move cursors to the front of the lists
    A_copy.moveFront();
    B_copy.moveFront();
    // Compare digits element by element
    for (int i = 0; i < length; ++i) {
        if (A_copy.peekNext() > B_copy.peekNext()) {
            // Current digit in A is larger, so A is larger overall
            return signum;
        }
        else if (A_copy.peekNext() < B_copy.peekNext()) {
            // Current digit in B is larger, so B is larger overall
            return -N.signum;
        }
        // Move cursors to the next element
        A_copy.moveNext();
        B_copy.moveNext();
    }
    // All digits are equal
    return 0;
}

// Manipulation Procedures -----
void BigInteger::makeZero() {
    digits.clear();
    signum = 0;
}

void BigInteger::negate() {
    signum *= -1;
}

// BigInteger Arithmetic operations---------------------------------------------------------------------------------------------------------
// add()
// Returns a BigInteger representing the sum of this and N.
BigInteger BigInteger::add(const BigInteger& N) const {
    BigInteger X = *this;
    BigInteger Y = N;
    BigInteger Z;

    // Handle cases where signs differ
    if (X.sign() > 0 && Y.sign() < 0) {
        Y.negate();
        return X.sub(Y);
    } else if (X.sign() < 0 && Y.sign() > 0) {
        X.negate();
        return Y.sub(X);
    } else if (X.sign() < 0 && Y.sign() < 0) {
        X.negate();
        Y.negate();
        Z = X.add(Y);
        Z.negate();
        return Z;
    }

    // Ensure X is the smaller BigInteger
    if (X > Y) {
        return Y.add(X);
    }

    List x = X.digits;
    List y = Y.digits;
    List z = Z.digits;

    long carry = 0;
    long temp = 0;

    // Perform addition digit by digit
    x.moveBack();
    y.moveBack();
    while (x.position() > 0 && y.position() > 0) {
        temp = carry + x.peekPrev() + y.peekPrev();
        carry = temp / BASE;
        temp %= BASE;
        z.insertAfter(temp);
        x.movePrev();
        y.movePrev();
    }

    // Handle remaining digits of Y
    while (y.position() > 0) {
        temp = carry + y.peekPrev();
        carry = temp / BASE;
        temp %= BASE;
        z.insertAfter(temp);
        y.movePrev();
    }

    // If there is a final carry, insert it
    if (carry > 0) {
        z.insertAfter(carry);
    }

    Z.signum = 1;
    Z.digits = z;
    return Z;
}

// sub()
// Returns a BigInteger representing the difference of this and N.
BigInteger BigInteger::sub(const BigInteger& N) const {
    BigInteger B = *this;
    BigInteger A = N;
    BigInteger C;
    List a = A.digits;
    List b = B.digits;
    List c = C.digits;

    // Check for special cases
    if (A == B) {
        return C;  // Return zero if A and B are equal
    }
    if (A.sign() && !B.sign()) {
        A.negate();
        return A;  // Return negation of A if A is positive and B is negative
    }
    if (!A.sign() && B.sign()) {
        return B;  // Return B if A is negative and B is positive
    }
    if (A.sign() < 0 && B.sign() > 0) {
        B.negate();
        C = A.add(B);
        C.negate();
        return C;  // Return (A + (-B)) negated if A is negative and B is positive
    }
    if (A.sign() > 0 && B.sign() < 0) {
        A.negate();
        C = A.add(B);
        return C;  // Return (A + (-B)) if A is positive and B is negative
    }
    if (A.sign() < 0 && B.sign() < 0) {
        A.negate();
        B.negate();
        C = B.sub(A);
        C.negate();
        return C;  // Return (B - A) negated if both A and B are negative
    }
    if (A < B) {
        C = A.sub(B);
        C.negate();
        return C;  // Return negation of (A - B) if A is less than B
    }
    // Perform digit-by-digit subtraction
    a.moveBack();
    b.moveBack();
    long dist = 0;
    long temp = 0;
    int i = b.position();
    while (!(i <= 0)) {
        if (a.peekPrev() - dist < b.peekPrev()) {
            temp = a.peekPrev() + BASE - b.peekPrev() - dist;
            dist = 1;
        } else {
            temp = a.peekPrev() - dist - b.peekPrev();
            if (!(a.peekPrev() <= 0)) {
                dist = 0;
            }
        }
        c.insertAfter(temp);
        a.movePrev();
        b.movePrev();
        i--;
    }
    while (a.position() > 0) {
        if (a.peekPrev() - dist < 0) {
            temp = a.peekPrev() + BASE - 0 - dist;
            dist = 1;
        } else {
            temp = a.peekPrev() - dist - 0;
            if (!(a.peekPrev() <= 0)) {
                dist = 0;
            }
        }
        c.insertAfter(temp);
        a.movePrev();
    }
    C.digits = c;
    delZero(&(C.digits));  // Remove leading zeros from the result
    C.signum = -1;  // Set the sign of the result to negative
    return C;
}

// mult()
// Returns a BigInteger representing the product of this and N. 
BigInteger BigInteger::mult(const BigInteger& N) const {
    BigInteger B = *this;
    BigInteger A = N;
    BigInteger C;
    List a = A.digits;
    List b = B.digits;
    int ctr = 0;
    a.moveBack();
    b.moveBack();
    int p = a.position();
    // Multiply each digit of A with B and accumulate the result in C
    for (int i = p; i > 0; i--) {
        List temp = tempMult(a.peekPrev(), &b, &ctr);  // Multiply current digit of A with B
        BigInteger T;
        T.signum = 1;
        T.digits = temp;
        C = C.add(T);  // Accumulate the result in C
        a.movePrev();  // Move to the next digit of A
        ctr++;
    }
    C.signum = A.signum * B.signum;  // Determine the sign of the result
    return C;
}

// Other Functions ----------------------------------------------------------------------------------------------------
std::string BigInteger::to_string() {
    std::string out = "";
    if (this->signum == 0) {
        return "0";  // Return "0" if the BigInteger is zero
    } else if (signum == -1) {
        out += "-";  // Add a negative sign if the BigInteger is negative
    }
    digits.moveFront();
    // Remove leading zeros from the digits list
    while (digits.front() == 0 && digits.length() > 1) {
        digits.eraseAfter();
    }
    for (digits.moveFront(); digits.position() < digits.length(); digits.moveNext()) {
        std::string A = std::to_string(digits.peekNext());
        std::string B = "";
        // Pad the number with zeros to ensure each chunk has POWER digits
        while ((int)(B.length() + A.length()) < POWER && digits.position() != 0) {
            B += '0';
        }
        out += (B + A);  // Concatenate the padded chunk with the output string
    }
    return out;
}

// Overriden Operators --------------------------------------------------------------------------------------------------------------
// operator<<()
// Inserts string representation of N into stream.
std::ostream& operator<<( std::ostream& stream, BigInteger N ) {
    return stream<<N.BigInteger::to_string();
}

// operator==()
// Returns true if and only if A equals B. 
bool operator==( const BigInteger& A, const BigInteger& B ) {
    int i = A.compare(B);
    return (i == 0) ? true : false;
}

// operator<()
// Returns true if and only if A is less than B. 
bool operator<( const BigInteger& A, const BigInteger& B ) {
    int i = A.compare(B);
    return (i == -1) ? true : false;
}

// operator<=()
// Returns true if and only if A is less than or equal to B. 

bool operator<=( const BigInteger& A, const BigInteger& B ) {
    int i = A.compare(B);
    return ((i == 0) || (i == -1)) ? true : false;
}

// operator>()
// Returns true if and only if A is greater than B. 
bool operator>( const BigInteger& A, const BigInteger& B ) {
    int i = A.compare(B);
    return (i == 1) ? true : false;
}

// operator>=()
// Returns true if and only if A is greater than or equal to B. 
bool operator>=( const BigInteger& A, const BigInteger& B ) {
    int i = A.compare(B);
    return ((i == 0) || (i == 1)) ? true : false;
}

// operator+()
// Returns the sum A+B. 
BigInteger operator+( const BigInteger& A, const BigInteger& B ) {
    return A.add(B);
}

// operator+=()
// Overwrites A with the sum A+B. 
BigInteger operator+=( BigInteger& A, const BigInteger& B ) {
    BigInteger I = A.add(B);
    A.digits = I.digits;
    A.signum = I.signum;
    return A; 
}

// operator-()
// Returns the difference A-B. 
BigInteger operator-( const BigInteger& A, const BigInteger& B ) {
    return A.sub(B);
}

// operator-=()
// Overwrites A with the difference A-B. 
BigInteger operator-=( BigInteger& A, const BigInteger& B ) {
    BigInteger I = A.sub(B);
    A.digits = I.digits;
    A.signum = I.signum;
    return A; 
}

// operator*()
// Returns the product A*B. 
BigInteger operator*( const BigInteger& A, const BigInteger& B ) {
    return A.mult(B);
}

// operator*=()
// Overwrites A with the product A*B. 
BigInteger operator*=( BigInteger& A, const BigInteger& B ) {
    BigInteger I = A.mult(B);
    A.digits = I.digits;
    A.signum = I.signum;
    return A; 
}