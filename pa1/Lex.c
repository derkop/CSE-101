/********************************************************************************* 
* Derrick Ko, dejko 
* 2023 Spring CSE101 PA# 1
* Lex.c 
* Main Function for List.c
*********************************************************************************/ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "List.h"

#define MAX_LINE_LENGTH 1024

// Helper Functions
// Returns the number of lines in the given file
int count_lines(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "could not open file '%s'\n", filename);
        exit(1);
    }
    int count = 0;
    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
        count++;
    }
    fclose(fp);
    return count;
}

// Reads the lines of the given file into an array
void read_lines(char *filename, char **lines, int num_lines) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "could not open file '%s'\n", filename);
        exit(1);
    }
    char line[MAX_LINE_LENGTH];
    int i = 0;
    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL && i < num_lines) {
        lines[i] = malloc((strlen(line) + 1) * sizeof(char));
        strcpy(lines[i], line);
        i++;
    }
    fclose(fp);
}

int main(int argc, char **argv) {
    // Check command line arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
        exit(1);
    }

    // Count the number of lines in the input file
    int num_lines = count_lines(argv[1]);

    // Allocate memory for the array of lines
    char **lines = malloc(num_lines * sizeof(char *));
    if (lines == NULL) {
        fprintf(stderr, "Error: memory allocation failed\n");
        exit(1);
    }

    // Read the lines of the input file into the array
    read_lines(argv[1], lines, num_lines);

    // Create a list of indices into the array, sorted by lexicographic order
    List L = newList();
    append(L, 0);
    for (int i = 1; i < num_lines; i++) {
        moveFront(L);
        while (index(L) >= 0 && strcmp(lines[i], lines[get(L)]) > 0) {
            moveNext(L);
        }
        if (index(L) >= 0) {
            insertBefore(L, i);
        } else {
            append(L, i);
        }
    }

    // Write the sorted lines to the output file
    FILE *out_fp = fopen(argv[2], "w");
    if (out_fp == NULL) {
        fprintf(stderr, "Error: could not open file '%s' for writing\n", argv[2]);
        exit(1);
    }
    moveFront(L);
    while (index(L) >= 0) {
        fprintf(out_fp, "%s", lines[get(L)]);
        moveNext(L);
    }
    fclose(out_fp);

    // Clean up memory
    freeList(&L);
    for (int i = 0; i < num_lines; i++) {
        free(lines[i]);
    }
    free(lines);

    return 0;
}
