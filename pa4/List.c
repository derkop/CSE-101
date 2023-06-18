/********************************************************************************* 
* Derrick Ko, dejko 
* 2023 Spring CSE101 PA# 4
* List.c 
* List ADT
*********************************************************************************/ 

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "List.h"

typedef struct NodeObj* Node;

typedef struct NodeObj {    
    void* data;
    struct NodeObj* prev;
    struct NodeObj* next;
} NodeObj;

typedef struct ListObj {
    Node front;
    Node back;
    Node cursor;
    int length;
    int index;
} ListObj;

// Returns reference to new Node object. Initializes next and data fields.
Node newNode(void* data){
   Node N = malloc(sizeof(NodeObj));
   assert(N != NULL);
   N->data = data;
   N->next = NULL;
   return(N);
}

// Frees heap memory pointed to by *pN, sets *pN to NULL.
void freeNode(Node* pN) {
    if (pN != NULL && *pN != NULL) {
        free(*pN);
        *pN = NULL;
    }
}

// Constructors-Destructors ---------------------------------------------------
// Returns reference to new empty List object.
List newList(void) {
    List L;
    L = malloc(sizeof(ListObj));
    L->front = L->back = L->cursor = NULL;
    L->length = 0;
    L->index = -1;
    return L;
}

// Frees all heap memory associated with List *pL, and sets *pL to NULL.
void freeList(List* pL) {
    if (pL != NULL && *pL != NULL) {
        clear(*pL);
        free(*pL);
        *pL = NULL;
    }
}

// Access functions -----------------------------------------------------------
// Returns the number of elements in L.
int length(List L) {
    if(L==NULL){
        fprintf(stderr, "L is NULL");
    exit(EXIT_FAILURE);
    }
    return L->length;
}

int index(List L) {
    return L->index;
}

void* front(List L) {
    if (length(L) == 0) {
        fprintf(stderr, "front() called on empty List\n");
        exit(EXIT_FAILURE);
    }
    return L->front->data;
}

void* back(List L) {
    if (length(L) == 0) {
        fprintf(stderr, "back() called on empty List\n");
        exit(EXIT_FAILURE);
    }
    return L->back->data;
}

void* get(List L) {
    if (length(L) == 0) {
        fprintf(stderr, "get() called on empty List\n");
        exit(EXIT_FAILURE);
    }
    if (index(L) < 0) {
        fprintf(stderr, "get() called with undefined index\n");
        exit(EXIT_FAILURE);
    }
    return L->cursor->data;
}

// Manipulation procedures -------------------------------------------------------------
// Resets L to its original empty state.
void clear(List L) {
    if (L == NULL) {
        printf("calling clear() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    Node curr = L->front;
    while (curr != NULL) {
        Node temp = curr->next;
        freeNode(&curr);
        curr = temp;
    }
    L->front = NULL;
    L->back = NULL;
    L->length = 0;
    L->index = -1;
}

// Overwrites the cursor element’s data with x.
// Pre: length()>0, index()>=0
void set(List L, void* x) {
    if (L == NULL) {
        printf("calling set() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (length(L) == 0) {
        printf("calling set() on an empty List\n");
        exit(EXIT_FAILURE);
    }
    if (index(L) < 0) {
        printf("calling set() with undefined cursor\n");
        exit(EXIT_FAILURE);
    }
    L->cursor->data = x;
}

// If L is non-empty, sets cursor under the front element,
// otherwise does nothing.
void moveFront(List L) {
    if (L == NULL) {
        printf("calling moveFront() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (length(L) > 0) {
        L->cursor = L->front;
        L->index = 0;
    }
}

// If L is non-empty, sets cursor under the back element,
// otherwise does nothing.
void moveBack(List L) {
    if (L == NULL) {
        printf("calling moveBack() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (length(L) > 0) {
        L->cursor = L->back;
        L->index = L->length - 1;
    }
}

void movePrev(List L) {
    if (L == NULL) {
        printf("calling movePrev() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (L->cursor != NULL) {
        if (L->cursor == L->front) {
            L->cursor = NULL;
            L->index = -1;
        } else {
            L->cursor = L->cursor->prev;
            L->index--;
        }
    }
}

void moveNext(List L) {
    if (L == NULL) {
        printf("calling moveNext() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (L->cursor != NULL) {
        if (L->cursor == L->back) {
            L->cursor = NULL;
            L->index = -1;
        } else {
            L->cursor = L->cursor->next;
            L->index++;
        }
    }
}

// Insert new element into L. If L is non-empty,
// insertion takes place before front element.
void prepend(List L, void* x) {
    Node N = newNode(x);
    if (L->length == 0) {
        L->front = N;
        L->back = N;
        L->cursor = N;
        L->index = 0;
    } else {
        N->next = L->front;
        L->front->prev = N;
        L->front = N;
        L->index++;
    }
    L->length++;
}

// Insert new element into L. If L is non-empty,
// insertion takes place after back element.
void append(List L, void* x) {
    Node N = newNode(x);
    if (L->length == 0) {
        L->front = L->back = N;
    } else {
        N->prev = L->back;
        L->back->next = N;
        L->back = N;
    }
    L->length++;
}

// Insert new element before cursor.
// Pre: length()>0, index()>=0
void insertBefore(List L, void* x) {
  if (L == NULL) {
    fprintf(stderr, "calling insertBefore() on NULL List reference\n");
    exit(EXIT_FAILURE);
  }
  if (length(L) <= 0 || index(L) < 0) {
    fprintf(stderr, "calling insertBefore() on empty or NULL cursor\n");
    exit(EXIT_FAILURE);
  }
  Node N = newNode(x);
  if (index(L) == 0) {
    L->front->prev = N;
    N->next = L->front;
    L->front = N;
  } else {
    N->prev = L->cursor->prev;
    L->cursor->prev->next = N;
    N->next = L->cursor;
    L->cursor->prev = N;
  }
  L->index++;
  L->length++;
}

// Insert new element after cursor.
// Pre: length()>0, index()>=0   
void insertAfter(List L, void* x) {
    if (L->cursor == NULL) {
        fprintf(stderr, "insertAfter() called on undefined cursor\n");
        exit(EXIT_FAILURE);
    }
    Node N = newNode(x);
    if (L->cursor == L->back) {
        L->back->next = N;
        N->prev = L->back;
        L->back = N;
    } else {
        N->next = L->cursor->next;
        L->cursor->next->prev = N;
        N->prev = L->cursor;
        L->cursor->next = N;
    }
    L->length++;
}

void deleteFront(List L) {
    if (L == NULL) {
        fprintf(stderr, "calling deleteFront() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (L->length > 0) {
        Node n = L->front;
        if (L->cursor == L->front) {
            L->cursor = NULL;
            L->index = -1;
        }
        if (L->length > 1) {
            L->front = L->front->next;
            L->front->prev = NULL;
            if(L->index!=-1){
                L->index--;
            }
            
        } else {
            L->front = L->back = NULL;
            if(L->index!=-1){
                L->index--;
            }
        }
        L->length--;
        freeNode(&n);
    }
}

void deleteBack(List L) {
  if (L == NULL) {
    printf("calling deleteBack() on NULL List reference\n");
    exit(EXIT_FAILURE);
  }
  if (length(L) <= 0) {
    printf("calling deleteBack() on an empty List\n");
    exit(EXIT_FAILURE);
  }
  Node temp = L->back;
  if (L->cursor == L->back) {
    L->cursor = NULL;
    L->index = -1;
  }
  if (length(L) == 1) {
    L->front = NULL;
    L->back = NULL;
    L->length = 0;
    freeNode(&temp);
  } else {
    L->back = temp->prev;
    L->back->next = NULL;
    L->length--;
    freeNode(&temp);
  }
}

void delete(List L) {
   if (L == NULL) {
      fprintf(stderr, "calling delete() on NULL List reference\n");
      exit(EXIT_FAILURE);
   }
   if (length(L) == 0 || index(L) < 0) {
      fprintf(stderr, "calling delete() on empty List or undefined cursor\n");
      exit(EXIT_FAILURE);
   }
   Node temp = L->cursor;
   if (temp == L->front) {
      L->front = temp->next;
      if (L->front != NULL) {
         L->front->prev = NULL;
      }
   } else if (temp == L->back) {
      L->back = temp->prev;
      if (L->back != NULL) {
         L->back->next = NULL;
      }
   } else {
      temp->prev->next = temp->next;
      temp->next->prev = temp->prev;
   }
   L->cursor = NULL;
   L->index = -1;
   L->length--;
   freeNode(&temp);
}

// Other operations -----------------------------------------------------

// Prints to the file pointed to by out, a
// string representation of L consisting
// of a space separated sequence of integers,
// with front on left.
void printList(FILE* out, List L) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling printList() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    for (moveFront(L); index(L) >= 0; moveNext(L)) {
        fprintf(out, "%d ", *((int*) get(L)));
    }

    fprintf(out, "\n");
}
