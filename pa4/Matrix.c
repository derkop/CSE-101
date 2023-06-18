/********************************************************************************* 
* Derrick Ko, dejko 
* 2023 Spring CSE101 PA# 4
* Matrix.c 
* Matrix ADT
*********************************************************************************/ 

#include <stdio.h>
#include <stdlib.h>

#include "Matrix.h"
#include "List.h"

// private Entry type
typedef struct EntryObj {
   int col;
   double val;
} EntryObj;

// private MatrixObj type
typedef struct MatrixObj {
   int size;
   int NNZ;
   List* rows;
} MatrixObj;

// public Matrix type
typedef struct EntryObj *Entry;

// Entry constructor
Entry newEntry(int col, double val) {
   Entry E = malloc(sizeof(EntryObj));
   E->col = col;
   E->val = val;
   return E;
}

// Entry destructor
void freeEntry(Entry* pE) {
   if (pE != NULL && *pE != NULL) {
      free(*pE);
      *pE = NULL;
   }
}

// Matrix constructor
Matrix newMatrix(int n) {
   Matrix M = malloc(sizeof(MatrixObj));
   M->size = n;
   M->NNZ = 0;
   M->rows = calloc(n+1, sizeof(List));
   for (int i = 1; i <= n; i++) {
      M->rows[i] = newList();
   }
   return M;
}

// Matrix destructor
void freeMatrix(Matrix* pM) {
   if (pM != NULL && *pM != NULL) {
      makeZero(*pM);
      for (int i = 1; i <= (*pM)->size; i++) {
         freeList(&((*pM)->rows[i]));
      }
      free((*pM)->rows);
      free(*pM);
      *pM = NULL;
   }
}

// Access functions -----------------------------------------------------------

// Returns the size of square Matrix M.
int size(Matrix M) {
   return M->size;
}

// Returns the number of non-zero elements in M.
int NNZ(Matrix M) {
   return M->NNZ;
}

// Returns true (1) if matrices A and B are equal, false (0) otherwise
int equals(Matrix A, Matrix B) {
    if (A == NULL || B == NULL) {
        fprintf(stderr, "Matrix | equals() | Calling equals() on one or more NULL Matrices.");
        exit(EXIT_FAILURE);
    }
    if (size(A) != size(B)) {
        return 0;
    }
    for (int i = 1; i <= size(A); i++) {
        if (length(A->rows[i]) != length(B->rows[i])) {
            return 0;
        }
        moveFront(A->rows[i]);
        moveFront(B->rows[i]);
        while (index(A->rows[i]) > 0) {
            if (((Entry)get(A->rows[i]))->col != ((Entry)get(B->rows[i]))->col) {
                return 0;
            }
            if (((Entry)get(A->rows[i]))->val != ((Entry)get(B->rows[i]))->val) {
                return 0;
            }
            moveNext(A->rows[i]);
            moveNext(B->rows[i]);
        }
    }
    return 1;
}

// Manipulation procedures -----------------------------------------------------

// Re-sets M to the zero Matrix state
void makeZero(Matrix M) {
   for (int i = 1; i <= size(M); i++) {
      List row = M->rows[i];
      if (row != NULL) {
        clear(row);
      }
   }
   M->NNZ = 0;
}

// Changes ith row, jth column of M to the value x.
// Pre: 1<=i<=size(M), 1<=j<=size(M)
void changeEntry(Matrix M, int i, int j, double x) {
   if (M == NULL) {
      fprintf(stderr, "Matrix | changeEntry() | Calling changeEntry() on a NULL Matrix.");
      exit(EXIT_FAILURE);
   }
   if (i < 1 || i > M->size || j < 1 || j > M->size) {
      fprintf(stderr, "Matrix Error: changeEntry() called with out-of-bounds index.\n");
      exit(EXIT_FAILURE);
   }

   List L = M->rows[i];
   moveFront(L);
   while (index(L) >= 0) {
      Entry E = (Entry) get(L);
      if (E->col == j) {
         if (x == 0.0) {
            delete(L);
            M->NNZ--;
            freeEntry(&E);
            return;
         } else {
            E->val = x;
            return;
         }
      } else if (E->col > j) {
         if (x != 0.0) {
            insertBefore(L, newEntry(j, x));
            M->NNZ++;
            return;
         } else {
            return;
         }
      }
      moveNext(L);
   }

   if (x != 0.0) {
      append(L, newEntry(j, x));
      M->NNZ++;
   }
}

// Matrix Arithmetic operations --------------------------------------------------

// Returns a reference to a new Matrix object having the same entries as A
Matrix copy(Matrix A) {
   Matrix B = newMatrix(A->size);
   for (int i = 1; i <= A->size; i++) {
      List row = A->rows[i];
      moveFront(row);
      while (index(row) != -1) {
         Entry entry = (Entry) get(row);
         changeEntry(B, i, entry->col, entry->val);
         moveNext(row);
      }
   }
   return B;
}

// Returns a reference to a new Matrix object representing the transpose of A
Matrix transpose(Matrix A) {
   Matrix B = newMatrix(A->size);
   for (int i = 1; i <= A->size; i++) {
      List row = A->rows[i];
      moveFront(row);
      while (index(row) != -1) {
         Entry entry = (Entry) get(row);
         changeEntry(B, entry->col, i, entry->val);
         moveNext(row);
      }
   }
   return B;
}

// Returns a reference to a new Matrix object representing xA
Matrix scalarMult(double x, Matrix A) {
   Matrix B = newMatrix(A->size);
   for (int i = 1; i <= A->size; i++) {
      List row = A->rows[i];
      moveFront(row);
      while (index(row) != -1) {
         Entry entry = (Entry) get(row);
         changeEntry(B, i, entry->col, x * entry->val);
         moveNext(row);
      }
   }
   return B;
}

// Computes the sum of two matrices A and B and returns the result matrix C.
// If A or B are NULL, prints an error message and exits the program.
// If A and B have different sizes, prints an error message and exits the program.
// If A and B are the same matrix, returns the result of doubling A.
Matrix sum(Matrix A, Matrix B) {
    // Check if A or B are NULL
    if (A == NULL || B == NULL) {
        fprintf(stderr, "Calling sum() on a NULL Matrix.");
        exit(EXIT_FAILURE);
    }
    // Check if A and B have different sizes
    if (size(A) != size(B)) {
        fprintf(stderr, "Calling sum() on different sized Matricies.");
        exit(EXIT_FAILURE);
    }
    // Check if A and B are the same matrix
    if (A == B) {
        return scalarMult(2, A);
    }
    // Create a new matrix C with the same size as A and B
    Matrix C = newMatrix(A->size);
    List sL;
    // Loop over each row in A, B, and C
    for (int i = 1; i <= C->size; i++) {
        sL = C->rows[i];
        // Move to the front of the current row in A and B
        moveFront(A->rows[i]);
        moveFront(B->rows[i]);
        // While both A and B have non-empty rows
        while (index(A->rows[i]) >= 0 && index(B->rows[i]) >= 0) {
            Entry aE = (Entry)get(A->rows[i]);
            Entry bE = (Entry)get(B->rows[i]);
            // If the column of the current entry in A is less than the column of the current entry in B
            if (aE->col < bE->col) {
                // Append the current entry in A to the current row in C
                append(sL, newEntry(aE->col, aE->val));
                // Increment the number of non-zero entries in C
                C->NNZ++;
                // Move to the next entry in A
                moveNext(A->rows[i]);
            }
            // If the column of the current entry in B is less than the column of the current entry in A
            else if (aE->col > bE->col) {
                // Append the current entry in B to the current row in C
                append(sL, newEntry(bE->col, bE->val));
                // Increment the number of non-zero entries in C
                C->NNZ++;
                // Move to the next entry in B
                moveNext(B->rows[i]);
            }
            // If the columns of the current entries in A and B are equal
            else {
                // If the sum of the values of the current entries is non-zero
                if (aE->val + bE->val != 0) {
                    // Append the sum of the values of the current entries to the current row in C
                    append(sL, newEntry(aE->col, aE->val + bE->val));
                    // Increment the number of non-zero entries in C
                    C->NNZ++;
                }
                // Move to the next entry in A and B
                moveNext(A->rows[i]);
                moveNext(B->rows[i]);
            }
        }
        // If A still has non-empty entries
         while (index(A->rows[i]) >= 0) {
            // get the current entry in A's row i
            Entry aE = (Entry)get(A->rows[i]);
            // add a new entry to sL with the same column and value
            append(sL, newEntry(aE->col, aE->val));
            // increment the NNZ count for Matrix C
            C->NNZ++;
            // move the cursor to the next entry in A's row i
            moveNext(A->rows[i]);
         }
         while (index(B->rows[i]) >= 0) { 
            Entry bE = (Entry)get(B->rows[i]);
            append(sL, newEntry(bE->col, bE->val));
            C->NNZ++;
            moveNext(B->rows[i]);
        }
    }
    return C;
}

// Returns a reference to a new Matrix object representing the difference A-B
// pre: size(A)==size(B)
Matrix diff(Matrix A, Matrix B) {
    if (A->size != B->size) {
        fprintf(stderr, "Error: matrices must have the same size.\n");
        exit(EXIT_FAILURE);
    }
    return sum(A, scalarMult(-1.0, B));
}

double vectorDot(List P, List Q) {
    double dot_prod = 0.0;
    // Move to the front of both lists
    moveFront(P);
    moveFront(Q);
    // Loop until one of the lists is exhausted
    while (index(P) >= 0 && index(Q) >= 0) {
        Entry eP = (Entry)get(P);
        Entry eQ = (Entry)get(Q);
        // If the columns match, add the products of the values to the dot product
        if (eP->col == eQ->col) {
            dot_prod += eP->val * eQ->val;
            // Move to the next element in both lists
            moveNext(P);
            moveNext(Q);
        }
        // If the column of the element in P is less than that of Q, move to the next element in P
        else if (eP->col < eQ->col) {
            moveNext(P);
        }
        // If the column of the element in P is greater than that of Q, move to the next element in Q
        else if (eP->col > eQ->col) {
            moveNext(Q);
        }
    }
    return dot_prod;
}

Matrix product(Matrix A, Matrix B) {
    if (A == NULL || B == NULL) {
        fprintf(stderr, "Matrix | product() | Calling product() on one or more NULL Matrices.");
        exit(EXIT_FAILURE);
    }
    if (A->size != B->size) {
        fprintf(stderr, "Matrix | product() | Calling product() on Matrices with different sizes.");
        exit(EXIT_FAILURE);
    }
    // Create a new matrix with the same size as A and B
    Matrix P = newMatrix(A->size);
    // If either matrix is empty, return an empty matrix
    if (A->NNZ == 0 || B->NNZ == 0) {
        return P;
    }
    // Transpose matrix B for efficient column-wise traversal
    Matrix BT = transpose(B);
    // Loop over every row in matrix A
    for (int i = 1; i <= A->size; i++) {
        List Prow = A->rows[i];
        // If the row is non-empty, loop over every row in matrix BT
        if (length(Prow) > 0) {
            for (int j = 1; j <= BT->size; j++) {
                List Qrow = BT->rows[j];
                // If the row is non-empty, compute the dot product and append the entry to matrix P
                if (length(Qrow) > 0) {
                    double dp = vectorDot(Prow, Qrow);
                    if (dp != 0) {
                        Entry e = newEntry(j, dp);
                        append(P->rows[i], e);
                        P->NNZ++;        
                    }
                }
            }
        }
    }
    // Free the memory used by the transposed matrix
    freeMatrix(&BT);
    return P;
}

void printMatrix(FILE* out, Matrix M) {
   if (out == NULL || M == NULL) {
      printf("Error: printMatrix() called with NULL parameters\n");
      exit(EXIT_FAILURE);;
   }
   for (int i = 1; i <= size(M); i++) {
      if (length(M->rows[i]) > 0) {
         fprintf(out, "%d: ", i);
         for (moveFront(M->rows[i]); index(M->rows[i]) >= 0; moveNext(M->rows[i])) {
            Entry e = get(M->rows[i]);
            fprintf(out, "(%d, %.1f) ", e->col, e->val);
         }
         fprintf(out, "\n");
      }
   }
}
