/********************************************************************************* 
* Derrick Ko, dejko 
* 2023 Spring CSE101 PA# 1
* ListTest.c 
* Testing Harness for List ADT 
*********************************************************************************/ 

#include <stdio.h> // for printf function
#include <stdlib.h> // for malloc and free functions

#include "List.h"

int main(){
    List A = newList();
    List B = newList();

    printf("Test append, prepend, length, and printList\n");
    append(A, 1);
    append(A, 2);
    append(A, 3);
    prepend(B, 3);
    prepend(B, 2);
    prepend(B, 1);
    printf("List A: ");
    printList(stdout, A);
    printf("\nList B: ");
    printList(stdout, B);
    printf("\n");

    printf("\n");
    printf("Test front, back, and moveFront\n");
    printf("Front of A: %d\n", front(A));
    printf("Back of A: %d\n", back(A));
    moveFront(A);
    printf("Current element of A: %d\n", get(A));

    printf("\n");
    printf("Test insertBefore, insertAfter, moveNext, movePrev\n");
    insertBefore(A, 0);
    insertAfter(A, 4);
    printf("List A after insertBefore and insertAfter: ");
    printList(stdout, A);
    printf("\n");
    moveNext(A);
    moveNext(A);
    printf("Current element of A after two moveNext calls: %d\n", get(A));
    movePrev(A);
    printf("Current element of A after movePrev call: %d\n", get(A));

    printf("\n");
    printf("Test deleteFront, deleteBack, delete\n");
    deleteFront(A);
    deleteBack(A);
    delete(A);
    printf("List A after deleteFront, deleteBack, delete: ");
    printList(stdout, A);
    printf("\n");

    printf("\n");
    printf("Test copyList and equals\n");
    List C = copyList(B);
    printf("List B: ");
    printList(stdout, B);
    printf("\nList C (copy of B): ");
    printList(stdout, C);
    printf("\nEquals B and C? %s\n", equals(B, C) ? "true" : "false");

    // Free memory
    freeList(&A);
    freeList(&B);
    freeList(&C);

    return 0;
}
