/********************************************************************************* 
* Derrick Ko, dejko 
* 2023 Spring CSE101 PA# 3
* Graph.c 
* Graph ADT
*********************************************************************************/ 

#include <stdlib.h>
#include <stdio.h>

#include "Graph.h"
#include "List.h"

typedef struct GraphObj {
    List* adj;    // array of Lists containing adjacent vertices
    int* color;   // array of vertex colors
    int* parent;  // array of parent vertices
    int* discover;  // array of discover times
    int* finish;  // array of finish times
    int order;    // number of vertices in the graph
    int size;     // number of edges in the graph
} GraphObj;

/*** Constructors-Destructors ***/

Graph newGraph(int n) {
    Graph G;
    G = malloc(sizeof(struct GraphObj));
    G->adj = malloc((n+1) * sizeof(List));
    G->color = malloc((n+1) * sizeof(int));
    G->parent = malloc((n+1) * sizeof(int));
    G->discover = malloc((n+1) * sizeof(int));
    G->finish = malloc((n+1) * sizeof(int));
    G->order = n;
    G->size = 0;
    for (int i = 1; i <= n; i++) {
        G->adj[i] = newList();
        G->color[i] = WHITE; 
        G->parent[i] = NIL;
        G->discover[i] = UNDEF;
        G->finish[i] = UNDEF;
    }
    return G;
}

void freeGraph(Graph* pG) {
    if (pG != NULL && *pG != NULL) {
        // free all adjacency lists
        for (int i = 1; i <= (*pG)->order; i++) {
            freeList(&((*pG)->adj[i]));
        }
        // free Graph object
        free((*pG)->adj);
        free((*pG)->color);
        free((*pG)->parent);
        free((*pG)->discover);
        free((*pG)->finish);
        free(*pG);
        *pG = NULL;
    }
}

/*** Access functions ***/

// Returns the order of the graph (number of vertices)
int getOrder(Graph G) {
    if (G == NULL) {
        fprintf(stderr, "getOrder() called on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    return G->order;
}

// Returns the size of the graph (number of edges)
int getSize(Graph G) {
    if (G == NULL) {
        fprintf(stderr, "getSize() called on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    return G->size;
}

// Returns the parent of vertex u in the BFS tree, or NIL if BFS() has not yet
// been called or if u is the source vertex.
int getParent(Graph G, int u) {
    if (G == NULL) {
        fprintf(stderr, "Error: getParent() called on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    if (u < 1 || u > getOrder(G)) {
        fprintf(stderr, "getParent() called with vertex out of range\n");
        exit(EXIT_FAILURE);
    }
    return G->parent[u];
}

/* Pre: 1<=u<=n=getOrder(G) */
int getDiscover(Graph G, int u) {
    if (u < 1 || u > getOrder(G)) {
        printf("calling getDiscover() with invalid vertex index\n");
        exit(EXIT_FAILURE);
    }
    return G->discover[u];
}

/* Pre: 1<=u<=n=getOrder(G) */
int getFinish(Graph G, int u) {
    if (u < 1 || u > getOrder(G)) {
        fprintf(stderr, "getFinish(): vertex %d is out of range\n", u);
        exit(EXIT_FAILURE);
    }
    return G->finish[u];
}

// Fills the list L with the vertices of the shortest path from the source vertex
// to vertex u in the BFS tree, or appends to L the value NIL if there is no such path.
// Assumes that BFS() has been called and that u is a vertex in the graph.
void getPath(List L, Graph G, int u) {
    if (G == NULL) {
        fprintf(stderr, "Error: getPath() called on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    if (u < 1 || u > getOrder(G)) {
        fprintf(stderr, "Error: getPath() called with vertex out of range\n");
        exit(EXIT_FAILURE);
    }
    // If u is not reachable from the source vertex, append NIL to the list and return
    if (getParent(G, u) == NIL) {
        append(L, NIL);
        return;
    }

    // Recursively add the path from the source to the parent of u to the list,
    // then add u to the list
    getPath(L, G, getParent(G, u));
    append(L, u);
}

// Manipulation procedures
void addEdge(Graph G, int u, int v) {
   addArc(G, u, v);
   addArc(G, v, u);
   G->size--;
}

void addArc(Graph G, int u, int v) {
    if (v > getOrder(G) || u > getOrder(G) || v < 1 || u < 1) {
        printf("addArc(): invalid vertex index\n");
        exit(1);
    }
    List L = G->adj[u];
    if (length(L) == 0) {
        append(L, v);
    } else {
        for (moveFront(L); index(L) >= 0; moveNext(L)) {
            if (v < get(L)) {
                insertBefore(L, v);
                break;
            } else if (v == get(L)) {
                // Arc already exists, do nothing
                return;
            }
        }
        if (index(L) < 0) {
            append(L, v);
        }
    }

    G->size++;
}

/* Pre: length(S)==getOrder(G) */
/* DFS function using Visit */
void DFS(Graph G, List S){
    // Check if the length of S is equal to the number of vertices in G
    // if(length(S) != getOrder(G)){ 
    //     printf("length S != size G");
    //     exit(EXIT_FAILURE);
    // }
    // Set all vertices to initial state
    for(int i = 0; i <= getOrder(G); i++){
        G->color[i] = WHITE;
        G->parent[i] = NIL;
        G->discover[i] = UNDEF;
        G->finish[i] = UNDEF;
    }
    // Initialize time variable and create temporary list to store vertices in S
    int time = 0;
    List temp = copyList(S);
    clear(S);
    // Traverse the vertices in the temporary list
    moveFront(temp);
    while(index(temp) >= 0){
        int vertex = get(temp);
        if(G->color[vertex] == WHITE){
            // Visit vertex if it has not been visited before
            visit(G, vertex, &time, S);
        }
        // Move to next vertex in the temporary list
        moveNext(temp);
    }
    // Free temporary list
    freeList(&temp);
} 

void visit(Graph G, int x, int *time, List S){
    G->discover[x] = ++(*time);
    G->color[x] = GRAY;
    List L = G->adj[x];
    moveFront(L);
    while (index(L) >= 0) {
        int y = get(L);
        if(G->color[y] == WHITE){
            G->parent[y] = x;
            visit(G, y, time, S);
        }
        moveNext(L);
    }
    G->color[x] = BLACK;
    G->finish[x] = ++(*time);
    prepend(S, x); // Add the vertex to the front of S
}

// Other Functions
Graph transpose(Graph G) {
    int n = getOrder(G);
    Graph T = newGraph(n);
    for (int u = 1; u <= n; u++) {
        List adj = G->adj[u];
        moveFront(adj);
        while (index(adj) != -1) {
            int v = get(adj);
            addArc(T, v, u); // Add reverse edge to transpose graph
            moveNext(adj);
        }
    }
    return T;
}

Graph copyGraph(Graph G) {
    int n = getOrder(G);
    Graph C = newGraph(n);
    for (int u = 1; u <= n; u++) {
        List adj = G->adj[u];
        moveFront(adj);
        while (index(adj) != -1) {
            int v = get(adj);
            addArc(C, u, v); // Add edge to copy graph
            moveNext(adj);
        }
    }
    return C;
}

void printGraph(FILE* out, Graph G) {
    if (out == NULL || G == NULL) {
        fprintf(stderr, "Graph Error: calling printGraph() on NULL reference(s)\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 1; i <= getOrder(G); i++) {
        fprintf(out, "%d: ", i);

        if (length(G->adj[i]) == 0) {
            fprintf(out, "\n");
            continue;
        }
        for (moveFront(G->adj[i]); index(G->adj[i]) != -1; moveNext(G->adj[i])) {
            fprintf(out, "%d", get(G->adj[i]));

            if (index(G->adj[i]) != length(G->adj[i]) - 1) {
                fprintf(out, " ");
            }
        }
        fprintf(out, "\n");
    }
}

