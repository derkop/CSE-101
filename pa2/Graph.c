/********************************************************************************* 
* Derrick Ko, dejko 
* 2023 Spring CSE101 PA# 2
* Graph.c 
* Graph ADT
*********************************************************************************/ 

#include <stdlib.h>
#include <stdio.h>

#include "Graph.h"
#include "List.h"

typedef struct GraphObj {
    List* adj;      // array of Lists
    int* color;     // array of colors
    int* parent;    // array of parents
    int* distance;  // array of distances
    int order;      // number of vertices
    int size;       // number of edges
    int source;     // source vertex for BFS
} GraphObj;

/*** Constructors-Destructors ***/

Graph newGraph(int n) {
    Graph G = malloc(sizeof(GraphObj));
    G->adj = calloc(n + 1, sizeof(List));
    G->color = calloc(n + 1, sizeof(int));
    G->parent = calloc(n + 1, sizeof(int));
    G->distance = calloc(n + 1, sizeof(int));
    G->order = n;
    G->size = 0;
    G->source = NIL;
    for (int i = 1; i <= n; i++) {
        G->adj[i] = newList();
        G->color[i] = 0;
        G->parent[i] = NIL;
        G->distance[i] = INF;
    }
    return G;
}

void freeGraph(Graph* pG) {
    if (pG == NULL || *pG == NULL) {
        return;
    }
    Graph G = *pG;
    for (int i = 1; i <= G->order; i++) {
        freeList(&(G->adj[i]));
    }
    free(G->adj);
    free(G->color);
    free(G->parent);
    free(G->distance);
    free(*pG);
    *pG = NULL;
}

/*** Access functions ***/

// Returns the order of the graph (number of vertices)
int getOrder(Graph G) {
    if (G == NULL) {
        fprintf(stderr, "Error: getOrder() called on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    return G->order;
}

// Returns the size of the graph (number of edges)
int getSize(Graph G) {
    if (G == NULL) {
        fprintf(stderr, "Error: getSize() called on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    return G->size;
}

// Returns the source vertex used in the last call to BFS(), or NIL if BFS() has
// not yet been called.
int getSource(Graph G) {
    if (G == NULL) {
        fprintf(stderr, "Error: getSource() called on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    return G->source;
}

// Returns the parent of vertex u in the BFS tree, or NIL if BFS() has not yet
// been called or if u is the source vertex.
int getParent(Graph G, int u) {
    if (G == NULL) {
        fprintf(stderr, "Error: getParent() called on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    if (u < 1 || u > getOrder(G)) {
        fprintf(stderr, "Error: getParent() called with vertex out of range\n");
        exit(EXIT_FAILURE);
    }
    return G->parent[u];
}

// Returns the distance from the source vertex to vertex u in the BFS tree, or
// INF if BFS() has not yet been called or if u is unreachable from the source.
int getDist(Graph G, int u) {
    if (G == NULL) {
        fprintf(stderr, "Error: getDist() called on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    if (u < 1 || u > getOrder(G)) {
        fprintf(stderr, "Error: getDist() called with vertex out of range\n");
        exit(EXIT_FAILURE);
    }
    return G->distance[u];
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
    if (getSource(G) == NIL) {
        fprintf(stderr, "Error: getPath() called before BFS()\n");
        exit(EXIT_FAILURE);
    }

    // If u is the source vertex, add it to the list and return
    if (u == getSource(G)) {
        append(L, u);
        return;
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

void makeNull(Graph G) {
   for (int i = 1; i <= getOrder(G); i++) {
      G->adj[i] = newList();
      G->color[i] = NIL;
      G->parent[i] = NIL;
      G->distance[i] = INF;
   }
   G->size = 0;
   G->source = NIL;
   G->order = getOrder(G);
}

void addEdge(Graph G, int u, int v) {
   addArc(G, u, v);
   addArc(G, v, u);
   G->size--;
}

void addArc(Graph G, int u, int v) {
   List L = G->adj[u];
   moveFront(L);
   while (index(L) != -1 && v > get(L)) {
      moveNext(L);
   }
   if (index(L) == -1) {
      append(L, v);
   }
   else {
      insertBefore(L, v);
   }
   G->size++;
}

void BFS(Graph G, int s) {
   for (int i = 1; i <= getOrder(G); i++) {
      G->color[i] = NIL;
      G->distance[i] = INF;
      G->parent[i] = NIL;
   }
   G->source = s;
   G->color[s] = 1;
   G->distance[s] = 0;
   G->parent[s] = NIL;
   List queue = newList();
   append(queue, s);
   while (length(queue) != 0) {
      int u = front(queue);
      deleteFront(queue);
      List L = G->adj[u];
      moveFront(L);
      while (index(L) != -1) {
         int v = get(L);
         if (G->color[v] == NIL) {
            G->color[v] = 1;
            G->distance[v] = G->distance[u] + 1;
            G->parent[v] = u;
            append(queue, v);
         }
         moveNext(L);
      }
      G->color[u] = 2;
   }
   freeList(&queue);
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

