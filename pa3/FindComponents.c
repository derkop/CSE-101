/********************************************************************************* 
* Derrick Ko, dejko 
* 2023 Spring CSE101 PA# 3
* FindComponents.c 
* Main Function for Graph.c
*********************************************************************************/ 

#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "List.h"

int main(int argc, char* argv[]) {
    FILE* in, * out;
    int n, u, v, x = 0, compCount = 0;
    Graph G, GT;
    List S = newList(), *scc;

    // Check command line for correct number of arguments
    if (argc != 3) {
        printf("Usage: %s <input file> <output file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Open input file
    if ((in = fopen(argv[1], "r")) == NULL) {
        printf("Unable to open file %s for reading\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    // Open output file
    if ((out = fopen(argv[2], "w")) == NULL) {
        printf("Unable to open file %s for writing\n", argv[2]);
        exit(EXIT_FAILURE);
    }

    // Read in the number of vertices
    fscanf(in, " %d", &n);

    // Create a new graph with n vertices
    G = newGraph(n);

    // Read in edges and add them to the graph
    while (fscanf(in, " %d %d", &u, &v) == 2 && u != 0 && v != 0) {
        addArc(G, u, v);
    }

    // Print adjacency list representation of G
    fprintf(out, "Adjacency list representation of G:\n");
    printGraph(out, G);
    fprintf(out, "\n");

    // Get transpose of G
    GT = transpose(G);

    // Add vertices to the stack
    for (int i = 1; i <= n; i++) {
        append(S, i);
    }

    // Call DFS on G and GT
    DFS(G, S);
    DFS(GT, S);

    // Count the number of strongly connected components
    for (int i = 1; i <= n; i++) {
        if (getParent(GT, i) == NIL) {
            x++;
        }
    }

    // Create an array of lists of strongly connected components
    scc = calloc(x, sizeof(List));

    for (int i = 0; i < x; i++) {
        scc[i] = newList();
    }

    // Move vertices to their respective component lists
    moveBack(S);

    while (index(S) != -1) {
        int y = get(S);
        prepend(scc[compCount], y);
        if (getParent(GT, y) == NIL) {
            compCount++;
        }

        movePrev(S);
    }

    // Print strongly connected components
    fprintf(out, "G contains %d strongly connected components:\n", x);

    for (int i = 0; i < x; i++) {
        fprintf(out, "Component %d: ", i + 1);
        printList(out, scc[i]);
        fprintf(out, "\n");
        freeList(&scc[i]);
    }


    // Cleaning
    free(scc);
    freeGraph(&G);
    freeGraph(&GT);
    freeList(&S);
    fclose(in);
    fclose(out);
    return 0;
}
