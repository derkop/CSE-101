//-----------------------------------------------------------------------------
//  MatrixClient.c 
//  A test client for the Matrix ADT
//-----------------------------------------------------------------------------
#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>

#include"Matrix.h"
     
int main(){
   int n = 4;
   Matrix A = newMatrix(n);
   Matrix B = newMatrix(n);
   Matrix C, D, E, F, G, H;

   // set values for A
   changeEntry(A, 1, 1, 1);
   changeEntry(A, 1, 2, 2);
   changeEntry(A, 2, 1, 3);
   changeEntry(A, 2, 2, 4);
   changeEntry(A, 3, 1, 5);
   changeEntry(A, 3, 2, 6);
   changeEntry(A, 4, 1, 7);
   changeEntry(A, 4, 2, 8);
   
   // set values for B
   changeEntry(B, 1, 1, 1);
   changeEntry(B, 2, 2, 2);
   changeEntry(B, 3, 3, 3);
   changeEntry(B, 4, 4, 4);
   
   // test NNZ, printMatrix, and makeZero
   printf("A has %d non-zero entries:\n", NNZ(A));
   printMatrix(stdout, A);
   printf("\n");
   makeZero(A);
   printf("After making A zero, it has %d non-zero entries:\n", NNZ(A));
   printMatrix(stdout, A);
   printf("\n");

   // test scalarMult
   C = scalarMult(2.0, B);
   printf("C = 2.0 * B has %d non-zero entries:\n", NNZ(C));
   printMatrix(stdout, C);
   printf("\n");

   // test sum
   D = sum(A, B);
   printf("D = A + B has %d non-zero entries:\n", NNZ(D));
   printMatrix(stdout, D);
   printf("\n");

   // test diff
   E = diff(A, B);
   printf("E = A - B has %d non-zero entries:\n", NNZ(E));
   printMatrix(stdout, E);
   printf("\n");

   // test transpose
   F = transpose(A);
   printf("F is the transpose of A and has %d non-zero entries:\n", NNZ(F));
   printMatrix(stdout, F);
   printf("\n");

   // test product
   G = product(A, transpose(B));
   printf("G = A * B^T has %d non-zero entries:\n", NNZ(G));
   printMatrix(stdout, G);
   printf("\n");

   // test copy and equals
   H = copy(A);
   printf("H is a copy of A and has %d non-zero entries:\n", NNZ(H));
   printMatrix(stdout, H);
   printf("\n");
   printf("A == H? %s\n", equals(A, H) ? "true" : "false");
   printf("A == B? %s\n", equals(A, B) ? "true" : "false");
   printf("A == A? %s\n", equals(A, A) ? "true" : "false");
   printf("\n");

   Matrix Z = newMatrix(5);
    int nnz = NNZ(Z);
    int Msize = size(Z);
    if (nnz == 0 && Msize == 5) {
      printf("Test create empty matrix: PASSED\n");
    } else {
      printf("Test create empty matrix: FAILED\n");
    }
    freeMatrix(&Z);


   // free matrices
   freeMatrix(&A);
   freeMatrix(&B);
   freeMatrix(&C);
   freeMatrix(&D);
   freeMatrix(&E);
  
  return 0;
}
