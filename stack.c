#include <stdlib.h>
#include "stack.h"

#define STACK_BLOCKSIZE 1000

/**
 * Initialised a new stack at the pointed to location.
 */
void *init_stack(stack **apstack) {
    *apstack = (stack*)malloc(sizeof(stack));
    ((stack*)*apstack)->cnt = 0;
    ((stack*)*apstack)->buf = malloc(STACK_BLOCKSIZE * sizeof(void*));
}

/**
 * Destroy the stack
 */
void destroy_stack(stack **apstack) {
    free(((stack*)*apstack)->buf);
    free(*apstack);
    *apstack = NULL;
}

void push(stack **apstack, void *pitem) {


}

void *pop(stack **apstack) {
}



