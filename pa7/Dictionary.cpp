/*********************************************************************************
* Derrick Ko, dejko
* 2023 Spring CSE101 PA# 7
* Dictionary.cpp
* Dictionary ADT
*********************************************************************************/

#include <stdexcept>

#include "Dictionary.h"

using namespace std;

// Node constructor
Dictionary::Node::Node(keyType k, valType v)
    : key(k), val(v), parent(nullptr), left(nullptr), right(nullptr) {}

// Dictionary constructor
Dictionary::Dictionary() : nil(new Node("", 0)), root(nil), current(nil), num_pairs(0) {}

// Copy constructor
Dictionary::Dictionary(const Dictionary& D)
    : nil(new Node("", 0)), root(nil), current(nil), num_pairs(0) {
    preOrderCopy(D.root, D.nil);
}

// Destructor
Dictionary::~Dictionary() {
    postOrderDelete(root);
    delete nil;
}

// Helper Functions-----------------------------------------------------------------------------------------------------------------------
// Helper function: inOrderString
void Dictionary::inOrderString(std::string& s, Node* R) const {
    if (R != nil) {
        inOrderString(s, R->left);
        s += R->key + " : " + std::to_string(R->val) + "\n";
        inOrderString(s, R->right);
    }
}

// Helper function: preOrderString
void Dictionary::preOrderString(string& s, Node* R) const {
    if (R != nil) {
        s.append(R->key + "\n");
        preOrderString(s, R->left);
        preOrderString(s, R->right);
    }
}

// Helper function: preOrderCopy
void Dictionary::preOrderCopy(Node* R, Node* N) {
    if (R != N) {
        setValue(R->key, R->val);
        preOrderCopy(R->left, N);
        preOrderCopy(R->right, N);
    }
}

// Helper function: postOrderDelete
void Dictionary::postOrderDelete(Node* R) {
    if (R != nil) {
        postOrderDelete(R->left);
        postOrderDelete(R->right);
        delete R;
    }
}

// Helper function: search
Dictionary::Node* Dictionary::search(Node* R, keyType k) const {
    while (R != nil && k != R->key) {
        if (k < R->key) {
            R = R->left;
        } else {
            R = R->right;
        }
    }
    return R;
}

// Helper function: findMin
Dictionary::Node* Dictionary::findMin(Node* R) {
    while (R->left != nil) {
        R = R->left;
    }
    return R;
}

// Helper function: findMax
Dictionary::Node* Dictionary::findMax(Node* R) {
    while (R->right != nil) {
        R = R->right;
    }
    return R;
}

// Helper function: findNext
Dictionary::Node* Dictionary::findNext(Node* N) {
    if (N->right != nil) {
        return findMin(N->right);
    }
    Node* P = N->parent;
    while (P != nil && N == P->right) {
        N = P;
        P = P->parent;
    }
    return P;
}

// Helper function: findPrev
Dictionary::Node* Dictionary::findPrev(Node* N) {
    if (N->left != nil) {
        return findMax(N->left);
    }
    Node* P = N->parent;
    while (P != nil && N == P->left) {
        N = P;
        P = P->parent;
    }
    return P;
}

// Access functions ----------------------------------------------------------------------------------------------------------------------
// Access function: size
int Dictionary::size() const {
    return num_pairs;
}

// Access function: contains
bool Dictionary::contains(keyType k) const {
    return search(root, k) != nil;
}

// Access function: getValue
valType& Dictionary::getValue(keyType k) const {
    Node* N = search(root, k);
    if (N == nil) {
        throw logic_error("Dictionary: getValue(): key \"" + k + "\" does not exist");
    }
    return N->val;
}

// Access function: hasCurrent
bool Dictionary::hasCurrent() const {
    return current != nil;
}

// Access function: currentKey
keyType Dictionary::currentKey() const {
    if (!hasCurrent()) {
        throw logic_error("Dictionary: currentKey(): current undefined");
    }
    return current->key;
}

// Access function: currentVal
valType& Dictionary::currentVal() const {
    if (!hasCurrent()) {
        throw logic_error("Dictionary: currentVal(): current undefined");
    }
    return current->val;
}

// Manipulation procedure -------------------------------------------------------------------------------------------
// Manipulation procedure: clear
void Dictionary::clear() {
    postOrderDelete(root);
    root = nil;
    current = nil;
    num_pairs = 0;
}

// Manipulation procedure: setValue
void Dictionary::setValue(keyType k, valType v) {
    Node* X = root;
    Node* Y = nil;
    Node* Z = new Node(k, v);
    Z->left = nil;
    Z->right = nil;

    while (X != nil) {
        Y = X;
        if (k < X->key) {
            X = X->left;
        } else if (k > X->key) {
            X = X->right;
        } else {
            // Key already exists, update the value and return
            X->val = v;
            delete Z;
            return;
        }
    }

    Z->parent = Y;
    if (Y == nil) {
        root = Z;
    } else if (k < Y->key) {
        Y->left = Z;
    } else {
        Y->right = Z;
    }
    num_pairs++;
}

// Manipulation procedure: remove
void Dictionary::remove(keyType k) {
    if (!contains(k)) {
        throw logic_error("Dictionary: remove(): key \"" + k + "\" does not exist");
    }

    Node* N = search(root, k);
    if (N == current) {
        current = nil;
    }

    // Case 1: N is a leaf node
    if (N->left == nil && N->right == nil) {
        if (N == root) {
            root = nil;
        } else {
            if (N->parent->left == N) {
                N->parent->left = nil;
            } else {
                N->parent->right = nil;
            }
        }
        delete N;
    }
    // Case 2: N has only left child
    else if (N->right == nil) {
        if (N == root) {
            root = N->left;
        } else {
            if (N->parent->left == N) {
                N->parent->left = N->left;
            } else {
                N->parent->right = N->left;
            }
        }
        N->left->parent = N->parent;
        delete N;
    }
    // Case 3: N has only right child
    else if (N->left == nil) {
        if (N == root) {
            root = N->right;
        } else {
            if (N->parent->left == N) {
                N->parent->left = N->right;
            } else {
                N->parent->right = N->right;
            }
        }
        N->right->parent = N->parent;
        delete N;
    }
    // Case 4: N has both left and right children
    else {
        Node* Y = findMin(N->right);
        if (Y->parent != N) {
            Y->parent->left = Y->right;
            if (Y->right != nil) {
                Y->right->parent = Y->parent;
            }
            Y->right = N->right;
            Y->right->parent = Y;
        }
        if (N == root) {
            root = Y;
        } else {
            if (N->parent->left == N) {
                N->parent->left = Y;
            } else {
                N->parent->right = Y;
            }
        }
        Y->parent = N->parent;
        Y->left = N->left;
        Y->left->parent = Y;
        delete N;
    }
    num_pairs--;
}

// Manipulation procedure: begin
void Dictionary::begin() {
    if (root != nil) {
        current = findMin(root);
    }
}

// Manipulation procedure: end
void Dictionary::end() {
    if (root != nil) {
        current = findMax(root);
    }
}

// Manipulation procedure: next
void Dictionary::next() {
    if (!hasCurrent()) {
        throw logic_error("Dictionary: next(): current not defined");
    }
    Node* N = findNext(current);
    current = (N != nil) ? N : nil;
}

// Manipulation procedure: prev
void Dictionary::prev() {
    if (!hasCurrent()) {
        throw logic_error("Dictionary: prev(): current not defined");
    }
    Node* N = findPrev(current);
    current = (N != nil) ? N : nil;
}

// Other function --------------------------------------------------------------------------------------------------------
// Other function: to_string
string Dictionary::to_string() const {
    string s;
    inOrderString(s, root);
    return s;
}

// Other function: pre_string
string Dictionary::pre_string() const {
    string s;
    preOrderString(s, root);
    return s;
}

// Other function: equals
bool Dictionary::equals(const Dictionary& D) const {
    if (size() != D.size()) {
        return false;
    }
    string s1 = to_string();
    string s2 = D.to_string();
    return s1 == s2;
}

// Overloaded operator -------------------------------------------------------------------------------------------------------
// Overloaded operator: <<
ostream& operator<<(ostream& stream, Dictionary& D) {
    stream << D.to_string();
    return stream;
}

// Overloaded operator: ==
bool operator==(const Dictionary& A, const Dictionary& B) {
    return A.equals(B);
}

// Overloaded operator: =
Dictionary& Dictionary::operator=(const Dictionary& D) {
    if (this != &D) {
        clear();
        preOrderCopy(D.root, D.nil);
    }
    return *this;
}
