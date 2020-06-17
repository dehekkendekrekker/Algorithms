#ifndef _STACK_H
#define _STACK_H

typedef struct stack {
    int cnt;
    void ** buf;
} stack;

void *init_stack(stack **);

void push(stack **, void*);

void *pop(stack **);

void destroy_stack(stack **);

#endif

