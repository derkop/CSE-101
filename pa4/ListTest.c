//-----------------------------------------------------------------------------
// ListClient.c
// Test client for List ADT
//-----------------------------------------------------------------------------
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#include"List.h"

int main(int argc, char* argv[]){
   
   List A = newList();
   List B = newList();
   List C = newList();
   int X[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
   int i, u=-1, v=-2, w=-3; 
   bool equal = false;

   // initialize Lists A and B
   for(i=1; i<=20; i++){
      append(A, &X[i]);
      prepend(B, &X[i]);
   }

   // print both lists of integers in forward direction
   for(moveFront(A); index(A)>=0; moveNext(A)){
      printf("%d ", *(int*)get(A));
   }
   printf("\n");
   for(moveFront(B); index(B)>=0; moveNext(B)){
      printf("%d ", *(int*)get(B));
   }
   printf("\n");

   // print both lists of integers in backward direction
   for(moveBack(A); index(A)>=0; movePrev(A)){
      printf("%d ", *(int*)get(A));
   }
   printf("\n");
   for(moveBack(B); index(B)>=0; movePrev(B)){
      printf("%d ", *(int*)get(B));
   }
   printf("\n");

   // make a shallow copy of A
   moveFront(A);
   while( index(A)>=0 ){
      append(C, get(A));
      moveNext(A);
   }
   // print the copy in forward direction
   for(moveFront(C); index(C)>=0; moveNext(C)){
      printf("%d ", *(int*)get(C));
   }
   printf("\n");

   // check shallow equality of A and C by comparing pointers
   equal = (length(A)==length(C));
   moveFront(A);
   moveFront(C);
   while( index(A)>=0 && equal){
      equal = ( get(A)==get(C) );
      moveNext(A);
      moveNext(C);
   }
   printf("A equals C is %s\n", (equal?"true":"false") );

   moveFront(A);
   for(i=0; i<5; i++) moveNext(A);     // at index 5
   printf("index(A)=%d\n", index(A));
   insertBefore(A, &u);                // now at index 6
   printf("index(A)=%d\n", index(A));
   for(i=0; i<9; i++) moveNext(A);     // at index 15
   printf("index(A)=%d\n", index(A));
   insertAfter(A, &v);                 // doesn't change index
   printf("index(A)=%d\n", index(A));
   for(i=0; i<5; i++) movePrev(A);     // at index 10
   printf("index(A)=%d\n", index(A));
   delete(A);                          // index is now undefined
   printf("index(A)=%d\n", index(A));
   moveBack(A);                        // now at index 20
   printf("index(A)=%d\n", index(A));
   for(i=0; i<10; i++) movePrev(A);    // at index 10
   printf("index(A)=%d\n", index(A));
   set(A, &w);

    // print A in forward and backward direction
    for(moveFront(A); index(A)>=0; moveNext(A)){
    printf("%d ", *(int*)get(A));
    }
    printf("\n");
    for(moveBack(A); index(A)>=0; movePrev(A)){
    printf("%d ", *(int*)get(A));
    }
    printf("\n");

    // check length of A, before and after clear()
    printf("%d\n", length(A));
    clear(A);
    printf("%d\n", length(A));


    List D = newList();
    int x = 1, y = 2, z = 3, a = 5, b = 6;
    append(D, &x);
    moveFront(D);
    set(D, &y); // should set the first element to y
    moveBack(A);
    set(D, &z); // should set the last element to z
    append(D, &y);
    prepend(D, &z);
    insertBefore(D, &a);
    insertAfter(D, &b);
    printList(stdout, D);
    freeList(&D);

    // Test Index
    List E = newList();
    for (int i = 0; i < 10; i++) {
        append(E, &i);
    }
    moveFront(E);
    for (int i = 0; i < 10; i++) {
        if (index(E) != i) {
            printf("Test Index: FAILED\n");
            break;
        }
        moveNext(E);
    }
    freeList(&E);
    printf("Test Index: PASSED\n");
    
    // Test SetGet
    List F = newList();
    for (int i = 0; i < 10; i++) {
        append(F, &i);
    }
    moveFront(F);
    for (int i = 0; i < 10; i++) {
        set(F, &i);
        if (*((int *) get(F)) != i) {
            printf("Test SetGet: FAILED\n");
            break;
        }
        moveNext(F);
    }
    freeList(&F);
    printf("Test SetGet: PASSED\n");




    freeList(&A);
    freeList(&B);
    freeList(&C);

    return(0);
}