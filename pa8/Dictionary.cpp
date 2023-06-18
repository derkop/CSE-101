/*********************************************************************************
* Derrick Ko, dejko
* 2023 Spring CSE101 PA# 8
* Dictionary.cpp
* Dictionary ADT with a Red-Black Tree
*********************************************************************************/

#include <stdexcept>

#include "Dictionary.h"

#define RED -1
#define BLACK -2

using namespace std;

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
void Dictionary::preOrderString(std::string& s, Node* R) const {
    if (R != nil) {
        s += R->key + (R->color == RED ? " (RED)" : "") + "\n";
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

// RBT Helper Functions (Optional) -----------------------------------------
void Dictionary::LeftRotate(Node* N) {
    Node* Y = N->right;
    N->right = Y->left;

    if (Y->left != nil) {
        Y->left->parent = N;
    }

    Y->parent = N->parent;

    if (N->parent == nil) {
        root = Y;
    } else {
        if (N == N->parent->left) {
            N->parent->left = Y;
        } else {
            N->parent->right = Y;
        }
    }

    Y->left = N;
    N->parent = Y;
}

void Dictionary::RightRotate(Node* N) {
    Node* Y = N->left;
    N->left = Y->right;

    if (Y->right != nil) {
        Y->right->parent = N;
    }

    Y->parent = N->parent;

    if (N->parent == nil) {
        root = Y;
    } else {
        if (N == N->parent->right) {
            N->parent->right = Y;
        } else {
            N->parent->left = Y;
        }
    }

    Y->right = N;
    N->parent = Y;
}

void Dictionary::RB_InsertFixUp(Node* N) {
    while (N->parent->color == RED) {
        if (N->parent == N->parent->parent->left) {
            Node* Uncle = N->parent->parent->right;
            if (Uncle->color == RED) {
                // Case 1
                N->parent->color = BLACK;
                Uncle->color = BLACK;
                N->parent->parent->color = RED;
                N = N->parent->parent;
            } else {
                if (N == N->parent->right) {
                    // Case 2
                    N = N->parent;
                    LeftRotate(N);
                }
                // Case 3
                N->parent->color = BLACK;
                N->parent->parent->color = RED;
                RightRotate(N->parent->parent);
            }
        } else {
            Node* Uncle = N->parent->parent->left;
            if (Uncle->color == RED) {
                // Case 4
                N->parent->color = BLACK;
                Uncle->color = BLACK;
                N->parent->parent->color = RED;
                N = N->parent->parent;
            } else {
                if (N == N->parent->left) {
                    // Case 5
                    N = N->parent;
                    RightRotate(N);
                }
                // Case 6
                N->parent->color = BLACK;
                N->parent->parent->color = RED;
                LeftRotate(N->parent->parent);
            }
        }
    }
    root->color = BLACK;
}

void Dictionary::RB_Transplant(Node* u, Node* v) {
    if (u->parent == nil) {
        root = v;
    }
    else if (u == u->parent->left) {
        u->parent->left = v;
    }
    else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

void Dictionary::RB_DeleteFixUp(Node* N) {
    while (N != root && N->color == BLACK) {
        if (N == N->parent->left) {
            Node* Sibling = N->parent->right;
            if (Sibling->color == RED) {
                // Case 1
                Sibling->color = BLACK;
                N->parent->color = RED;
                LeftRotate(N->parent);
                Sibling = N->parent->right;
            }
            if (Sibling->left->color == BLACK && Sibling->right->color == BLACK) {
                // Case 2
                Sibling->color = RED;
                N = N->parent;
            } else {
                if (Sibling->right->color == BLACK) {
                    // Case 3
                    Sibling->left->color = BLACK;
                    Sibling->color = RED;
                    RightRotate(Sibling);
                    Sibling = N->parent->right;
                }
                // Case 4
                Sibling->color = N->parent->color;
                N->parent->color = BLACK;
                Sibling->right->color = BLACK;
                LeftRotate(N->parent);
                N = root;
            }
        } else {
            Node* Sibling = N->parent->left;
            if (Sibling->color == RED) {
                // Case 5
                Sibling->color = BLACK;
                N->parent->color = RED;
                RightRotate(N->parent);
                Sibling = N->parent->left;
            }
            if (Sibling->right->color == BLACK && Sibling->left->color == BLACK) {
                // Case 6
                Sibling->color = RED;
                N = N->parent;
            } else {
                if (Sibling->left->color == BLACK) {
                    // Case 7
                    Sibling->right->color = BLACK;
                    Sibling->color = RED;
                    LeftRotate(Sibling);
                    Sibling = N->parent->left;
                }
                // Case 8
                Sibling->color = N->parent->color;
                N->parent->color = BLACK;
                Sibling->left->color = BLACK;
                RightRotate(N->parent);
                N = root;
            }
        }
    }
    N->color = BLACK;
}

void Dictionary::RB_Delete(Node* N) {
    Node* X;
    Node* Y = N;
    int save = Y->color;
    if (N->left == nil) {
        X = N->right;
        RB_Transplant(N, N->right);
    }
    else if (N->right == nil) {
        X = N->left;
        RB_Transplant(N, N->left);
    }
    else {
        Y = findMin(N->right);
        save = Y->color;
        X = Y->right;
        if (Y->parent == N) {
            X->parent = Y;
        }
        else {
            RB_Transplant(Y, Y->right);
            Y->right = N->right;
            Y->right->parent = Y;
        }
        RB_Transplant(N, Y);
        Y->left = N->left;
        Y->left->parent = Y;
        Y->color = N->color;
    }
    if (save == BLACK) {
        RB_DeleteFixUp(X);
    }
    delete N;
}

//Constructors & Destructors ----------------------------------------

Dictionary::Node::Node(keyType k, valType v) {
    key = k;
    val = v;
    parent = nullptr;
    left = nullptr;
    right = nullptr;
    color = RED;
}

Dictionary::Dictionary() {
    nil = new Node("\0", -1);
    nil->color = BLACK;
    root = nil;
    current = nil;
    num_pairs = 0;
}

Dictionary::Dictionary(const Dictionary& D) {
    nil = new Node("\0", -1);
    nil->color = BLACK;
    root = nil;
    current = nil;
    num_pairs = 0;

    preOrderCopy(D.root, D.nil);
}

Dictionary::~Dictionary() {
    postOrderDelete(root);
    delete nil;
}

// Access Functions ----------------------------------------------------------------------------------------------------
int Dictionary::size() const {
    return num_pairs;
}

bool Dictionary::contains(keyType k) const {
    if (search(root, k) == nil) {
        return false;
    }
    return true;
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

valType& Dictionary::currentVal() const {
    if (hasCurrent() == false) {
        throw logic_error("Dictionary: currentVal(): current undefined");
    }
    return current->val;
}

// Manipulation procedures -------------------------------------------------
void Dictionary::clear() {
    postOrderDelete(root);
    root = nil;
    current = nil;
    num_pairs = 0;
}

void Dictionary::setValue(keyType k, valType v) {
    Node* X = root;
    Node* Y = nil;
    Node* Z = new Node(k, v);
    Z->left = nil;
    Z->right = nil;
    Z->color = RED;

    while (X != nil) {
        Y = X;
        if (k < X->key) {
            X = X->left;
        } else if (k > X->key) {
            X = X->right;
        } else {
            // Key already exists, update the value and clean up
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
    RB_InsertFixUp(Z);
}

void Dictionary::remove(keyType k) {
    if (contains(k) == false) {
        throw logic_error("Dictionary: remove(): key \"" + k + "\" does not exist");
    }
    Node* N = search(root, k);
    if (N == current) {
        current = nil;
    }
    RB_Delete(N);
    num_pairs--;
}

void Dictionary::begin() {
    if (num_pairs > 0) {
        current = findMin(root);
    }
}

void Dictionary::end() {
    if (num_pairs > 0) {
        current = findMax(root);
    }
}

void Dictionary::next() {
    if (hasCurrent() == false) {
        throw logic_error("Dictionary: next(): current undefined");
    }
    if (findNext(current) != nil) {
        Node* N = findNext(current);
        current = N;
    }
    else {
        current = nil;
    }
}

void Dictionary::prev() {
    if (hasCurrent() == false) {
        throw logic_error("Dictionary: prev(): current undefined");
    }
    if (findPrev(current) != nil) {
        Node* N = findPrev(current);
        current = N;
    }
    else {
        current = nil;
    }
}

// Other Functions ---------------------------------------------------------------------------------------------------------
std::string Dictionary::to_string() const {
    string S = "";
    this->inOrderString(S, this->root);
    return S;
}

std::string Dictionary::pre_string() const {
    string S = "";
    this->preOrderString(S, this->root);
    return S;
}

bool Dictionary::equals(const Dictionary& D) const {
    if (this->num_pairs != D.num_pairs) {
        return false;
    }
    string A, B;
    A = this->to_string();
    B = D.to_string();
    if (A == B) {
        return true;
    }
    return false;
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

