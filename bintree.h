#ifndef _BINTREE_H
#define _BINTREE_H

#include <linux/types.h>

typedef struct btnode {
    void *next[2];
} btnode;

void **addbtbranch(btnode **, __be32);

void *getbtbranch(btnode *, __be32);

void delbtbranch(btnode **, __be32);

void delbtree(btnode **);

static void delbtnode(btnode *, int);

#endif

