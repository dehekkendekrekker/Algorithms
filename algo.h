#ifndef _ALGO_H
#define _ALGO_H

#include <linux/types.h>


/**
 * Prototype of BINARY TREE implementation
 */

typedef struct btnode {
    void *next[2];
} btnode;

void **addbtbranch(btnode **, __be32);
void *getbtbranch(btnode *, __be32);
void delbtbranch(btnode **, __be32);
void delbtree(btnode **);
static void delbtnode(btnode *, int);


/**
 * Prototype of QUEUE implementation
 */

typedef struct qlink {
    void *pitem;
    struct qlink *pnext;
} qlink;

static qlink *create_qlink(void *);
void queue_pushi(qlink **, void*);
void *queue_popi(qlink **);
void queue_rm(qlink **);

#endif

