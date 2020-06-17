#include <stdlib.h>
#include "algo.h"

/************************************
 * BINARY TREE IMPLEMENTATOIN
 ************************************

/**
 * Add a a sequence identified by an ip address to the binary tree
 *
 * This function will initialize a new binary at address pointed to
 * by trunk.
 *
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


/**************************************
 * QUEUE IMPLEMENTATION
 *************************************/

/**
 * Creates a new qlink
 */
qlink *create_qlink(void *pitem) {
    qlink *pqlink ;

    pqlink = malloc(sizeof(qlink));
    pqlink->pitem = pitem;
    pqlink->pnext = NULL;

    return pqlink;

}

/**
 * Appends a qlink at the end of the queue
 */
void queue_push(qlink **apqlink, void *pitem) {
    qlink *pqlink, *pnewqlink;     

    // In case the queue is empty, add the first link
    if (*apqlink == NULL) {
        *apqlink = create_qlink(pitem);
        return;
    }

    // Load the first link of the queue
    pqlink = *apqlink;

    // Traverse the queue to get to the end
    while ((pqlink = pqlink->pnext) != NULL) {
        pqlink = pqlink->pnext;
    }

    // pqlink now points to the last link
    // Add a new link to the chain
    pqlink->pnext = create_qlink(pitem);
}

/**
 * Pops a qlink off the start of the queue
 */
void *queue_pop(qlink **apqlink) {
    void *pitem;
    qlink *pqlink;

    if (*apqlink == NULL) return NULL;

    // Get the first link of the queue, and put it on the stack
    pqlink = *apqlink;

    // Make the next link the first link
    *apqlink = pqlink->pnext;

    // Get the point to the item
    pitem = pqlink->pitem;

    // Free the link on the stack
    free(pqlink);

    // Return the pointer to the item
    return pitem;
}

void queue_rm(qlink **apqlink) {
}


