#include <stdio.h>
#include <stdlib.h>
#include "Matrix.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: Sparse <input file> <output file>\n");
        exit(EXIT_FAILURE);
    }
    FILE* in = fopen(argv[1], "r");
    if (in == NULL) {
        fprintf(stderr, "Unable to open file %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    FILE* out = fopen(argv[2], "w");
    if (out == NULL) {
        fprintf(stderr, "Unable to open file %s\n", argv[2]);
        exit(EXIT_FAILURE);
    }

    int n, a, b;
    fscanf(in, "%d %d %d", &n, &a, &b);
    Matrix A = newMatrix(n);
    Matrix B = newMatrix(n);

    for (int i = 0; i < a; i++) {
        int row, col;
        double value;
        fscanf(in, "%d %d %lf", &row, &col, &value);
        changeEntry(A, row, col, value);
    }

    for (int i = 0; i < b; i++) {
        int row, col;
        double value;
        fscanf(in, "%d %d %lf", &row, &col, &value);
        changeEntry(B, row, col, value);
    }

    fprintf(out, "A has %d non-zero entries:\n", NNZ(A));
    printMatrix(out, A);

    fprintf(out, "\nB has %d non-zero entries:\n", NNZ(B));
    printMatrix(out, B);
    fprintf(out, "\n");

    //(1.5)*A
    fprintf(out, "(1.5)*A =\n");
    Matrix Z = scalarMult(1.5, A);
    printMatrix(out, Z);
    fprintf(out, "\n");
    freeMatrix(&Z);

    // A + B
    fprintf(out, "A+B =\n");
    Matrix C = sum(A, B);
    printMatrix(out, C);
    fprintf(out, "\n");
    freeMatrix(&C);

    // A+A 
    fprintf(out, "A+A =\n");
    Matrix D = sum(A, A);
    printMatrix(out, D);
    fprintf(out, "\n");
    freeMatrix(&D);

    // B-A
    fprintf(out, "B-A =\n");
    Matrix E = diff(B, A);
    printMatrix(out, E);
    fprintf(out, "\n");
    freeMatrix(&E);

    // A-A
    fprintf(out, "A-A =\n");
    Matrix F = diff(A, A);
    printMatrix(out, F);
    fprintf(out, "\n");
    freeMatrix(&F);

    // Transpose(A)
    fprintf(out, "Transpose(A) =\n");
    Matrix G = transpose(A);
    printMatrix(out, G);
    fprintf(out, "\n");
    freeMatrix(&G);

    // A*B
    fprintf(out, "A*B =\n");
    Matrix H = product(A, B);
    printMatrix(out, H);
    fprintf(out, "\n");
    freeMatrix(&H);

    // B*B
    fprintf(out, "B*B =\n");
    Matrix I = product(B, B);
    printMatrix(out, I);
    fprintf(out, "\n");
    freeMatrix(&I);

    freeMatrix(&A);
    freeMatrix(&B);
    
    fclose(in);
    fclose(out);
    return 0;
}
