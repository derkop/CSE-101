/********************************************************************************* 
* Derrick Ko, dejko 
* 2023 Spring CSE101 PA# 3
* GraphTest.c 
* Testing Client for Graph.c
*********************************************************************************/ 
#include <stdio.h>
#include <stdlib.h>
#include "List.h"
#include "Graph.h"

int main(int argc, char* argv[]) {
    int i, n = 10;
    List S = newList();
    Graph G = newGraph(n);
    Graph T = NULL, C = NULL;

    for (i = 1; i <= n; i++) {
        append(S, i);
    }

    addEdge(G, 1, 2);
    addEdge(G, 1, 5);
    addEdge(G, 2, 5);
    addEdge(G, 2, 6);
    addEdge(G, 3, 2);
    addEdge(G, 3, 4);
    addEdge(G, 3, 6);
    addEdge(G, 3, 7);
    addEdge(G, 3, 8);
    addEdge(G, 6, 5);
    addEdge(G, 6, 7);
    addEdge(G, 8, 4);
    addEdge(G, 8, 7);

    printf("Original graph:\n");
    printGraph(stdout, G);

    DFS(G, S);
    printf("\n");
    fprintf(stdout, "Vertex:  Discovery  Finish  Parent\n");
    for(i=1; i<=n; i++){
        fprintf(stdout, "%6d: %9d %7d %7d\n", i, getDiscover(G, i), getFinish(G, i), getParent(G, i));
    }

    printf("\nList of vertices in order of decreasing finish times:\n");
    printList(stdout, S);

    T = transpose(G);
    C = copyGraph(G);

    printf("\n");
    printf("\nTranspose of graph:\n");
    printGraph(stdout, T);

    printf("\nCopy of graph:\n");
    printGraph(stdout, C);

    DFS(T, S);
    printf("\n");
    fprintf(stdout, "Vertex:  Discovery  Finish  Parent\n");
   for(i=1; i<=n; i++){
      fprintf(stdout, "%6d: %9d %7d %7d\n", i, getDiscover(G, i), getFinish(G, i), getParent(G, i));
   }

    freeList(&S);
    freeGraph(&G);
    freeGraph(&T);
    freeGraph(&C);

    return 0;
}
