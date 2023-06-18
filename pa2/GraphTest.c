/********************************************************************************* 
 * Derrick Ko, dejko 
 * 2023 Spring CSE101 PA# 2
 * GraphTest.c 
 * Test Client for the Graph ADT
*********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Graph.h"

void printTestResult(int success) {
    if (success) {
        printf("\033[32mTest: PASSED\033[0m\n"); // Green text
    } else {
        printf("\033[31mTest: FAILED\033[0m\n"); // Red text
    }
}

int main(int argc, char* argv[]) {
    Graph G = NULL;
    List L = newList();

    // Create graph with 5 vertices
    printf("Create graph with 10 vertices\n");
    G = newGraph(10);
    printTestResult(getOrder(G) == 10);

    // Add 5 edges to graph, check size
    printf("Add 5 edges to graph, check size\n");
    addEdge(G, 1, 2);
    addEdge(G, 2, 3);
    addEdge(G, 3, 4);
    addEdge(G, 4, 5);
    addEdge(G, 5, 1);
    printTestResult(getSize(G) == 5 && getDist(G, 4) == -1 && getParent(G, 4) == NIL);

    // Set source vertex to 2, check source vertex
    printf("Set source vertex to 2, check source vertex\n");
    BFS(G, 2);
    printTestResult(getSource(G) == 2);

    // Get path from source to vertex 3, check path
    printf("Get path from source to vertex 3, check path\n");
    getPath(L, G, 3);
    printTestResult(front(L) == 2 && back(L) == 3 && length(L) == 2);

    // Get distance to vertex 3, check distance
    printf("Get distance to vertex 3, check distance\n");
    printTestResult(getDist(G, 3) == 1);

    // Free memory
    printf("Free memory\n");
    freeGraph(&G);
    freeList(&L);
    printTestResult(1);

    return 0;
}
