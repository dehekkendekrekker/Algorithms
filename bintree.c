#include <stdlib.h>
#include "bintree.h"

/**
 * Add a a sequence identified by an ip address to the binary tree
 *
 * This function will initialize a new binary at address pointed to
 * by trunk.
 *
 * NOTE. Make sure the sequence does not already exist within the tree
 * If you don't, a new leaf will be added to an already existing branch,
 * leaving the previously allocated block without a pointer
 */
void ** addbtbranch(btnode **trunk, __be32 ip) {
    char i;
    char bit;
    __be32 bitmask;
    btnode *pbtnode;

    // Bintree initialisation
    if (*trunk == NULL) {
        *trunk = calloc(1, sizeof(btnode));
    }


    pbtnode = *trunk;

    // Loop through all bits of the IP address
    for (i = 31; i >= 0 ; i--) {
        bitmask = 1 << i;
        bit = (ip & bitmask) == bitmask;

        // Check if we have a node under index bit
        if (pbtnode->next[bit] != NULL) {
            pbtnode = pbtnode->next[bit];
            continue;
        }

        // If it is null, determine wether to add a branch node or leaf node
        if (i == 0) { // Are we at the last branch node?
            return &pbtnode->next[bit];
        } else {
            pbtnode->next[bit] = calloc(1, sizeof(btnode));
        }

        pbtnode = pbtnode->next[bit];
    }
}

/**
 * Get a sequence based on its IP
 */
void *getbtbranch(btnode *trunk, __be32 ip) {
    char i;
    char bit;
    __be32 bitmask;
    btnode *pbtnode = trunk;

    // Return NULL if the bintree is uninitialized
    if (pbtnode == NULL) return NULL;

    // Loop through all bits of the IP address
    for (i = 31; i >= 0; i--) {
        bitmask = 1 << i;
        bit = (ip & bitmask) == bitmask;
        pbtnode = pbtnode->next[bit];

        // If during the loop a NULL link is returned,
        // it means the ip address is not present in the bin tree
        if (pbtnode == NULL) break;
    }
   
    if (pbtnode == NULL) return NULL;

    // pbtnode now points to the sequence
    return pbtnode;
}


/**
 * Deletes a sequence
 * NOTE. Make sure the sequence exists in the tree before deleting.
 */
void delbtbranch(btnode **aptrunk, __be32 ip) {
    char    i = 0;
    char    bit = 0;
    __be32  bitmask = 0;

    // Store of deletables nodes
    void **apbtnodes[32] = {0};

    void  **apbtnode = (void**)aptrunk;

    // Loop through all bits and through all nodes. Root to leaf
    for (i = 31; i >= 0; i--) {   // 32 cycles
    //    printf("%i\n", i);
        apbtnodes[i] = apbtnode;   // Address of the pointer to the next  block

        bitmask = 1 << i;
        bit = (ip & bitmask) == bitmask;

        apbtnode = &(((btnode*)*apbtnode)->next[bit]);

    }

    // abpnode now contains the address of the pointer to the sequence
    // Free this up
    free(*apbtnode);
    *apbtnode = NULL;

    // Now loop through the list of addresses and pointers in reverse order
    // And determine wether or not they should be deleted.
    for (i = 0; i < 32; i++) {
        apbtnode = apbtnodes[i];
        if (((btnode*)*apbtnode)->next[0] == NULL && ((btnode*)*apbtnode)->next[1] == NULL) {
            free(*apbtnode);
            *apbtnode = NULL;
        }
    }
}

/**
 * Deletes a binary tree pointed to by *aptrunk
 */
void delbtree(btnode **aptrunk) {
    int depth = 32;

    delbtnode(*aptrunk, depth);
    *aptrunk = NULL;
}


/**
 * Deletes a node from a binary tree
 */
static void delbtnode(btnode *btnode, int depth) {
    if (depth == 0) goto free;

    if (btnode->next[0] != NULL)
        delbtnode(btnode->next[0], depth - 1);

    if (btnode->next[1] != NULL)
        delbtnode(btnode->next[1], depth - 1);

free:
    free(btnode);
}

