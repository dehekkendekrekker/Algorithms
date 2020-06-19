#if __has_include <linux/slab.h>
    #include <linux/slab.h>
    #define __CALLOC(a,b) kcalloc(a, b, GFP_KERNEL)
    #define __FREE(a) kfree(a)
#else
    #include <stdlib.h>
    #define __CALLOC(a,b) calloc(a, b)
    #define __FREE(a) free(a)
#endif

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
        *trunk = __CALLOC(1, sizeof(btnode));
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
            pbtnode->next[bit] = __CALLOC(1, sizeof(btnode));
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
 * Deletes a branch from the binary tree. It does matter if the branch exists within the system.
 */
void delbtbranch(btnode **aptrunk, __be32 ip) {
    char    i = 0;
    char    bit = 0;
    __be32  bitmask = 0;

    // Store of deletables nodes
    void **apbtnodes[32] = {0};

    void  **apbtnode = (void**)aptrunk;

    // Return null if the tree is empty
    if (*apbtnode == NULL) return;

    // Loop through all bits and through all nodes. Root to leaf
    for (i = 31; i >= 0; i--) {   // 32 cycles
        apbtnodes[i] = apbtnode;   // Address of the pointer to the next  block

        bitmask = 1 << i;
        bit = (ip & bitmask) == bitmask;

        // If the next node is net set, assume it has been deleted
        if ((((btnode*)*apbtnode)->next[bit]) == NULL) return;

        apbtnode = &(((btnode*)*apbtnode)->next[bit]);

    }

    // abpnode now contains the address of the pointer to the sequence
    // __FREE this up
    __FREE(*apbtnode);
    *apbtnode = NULL;

    // Now loop through the list of addresses and pointers in reverse order
    // And determine wether or not they should be deleted.
    for (i = 0; i < 32; i++) {
        apbtnode = apbtnodes[i];
        if (((btnode*)*apbtnode)->next[0] == NULL && ((btnode*)*apbtnode)->next[1] == NULL) {
            __FREE(*apbtnode);
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
void delbtnode(btnode *btnode, int depth) {
    if (depth == 0) goto free;

    if (btnode->next[0] != NULL)
        delbtnode(btnode->next[0], depth - 1);

    if (btnode->next[1] != NULL)
        delbtnode(btnode->next[1], depth - 1);

free:
    __FREE(btnode);
}


/**************************************
 * QUEUE IMPLEMENTATION (DOUBLY LINKED LIST)
 *************************************/

queue init_queue() {
    queue q;
    q.pcur = NULL;
    q.pstart = NULL;
    q.pend = NULL;
    q.cnt= 0;

    return q;
}

/**
 * Creates a new qlink
 */
qlink *create_qlink(void *pitem) {
    qlink *pqlink ;

    pqlink = __CALLOC(1, sizeof(qlink));
    pqlink->pitem = pitem;
    pqlink->pnext = NULL;
    pqlink->pprev = NULL;

    return pqlink;
}

/**
 * Appends a qlink at the end of the queue
 */
void queue_pushi(queue *pqueue, void *pitem) {
    qlink *pqlink, *pnewqlink;     

    // Create a new qlink
    pnewqlink = create_qlink(pitem);

    // If the list is empty, make the new link the start of the queue as well
    if (pqueue->cnt == 0) {
        pqueue->pstart = pnewqlink;
        pqueue->pend = pnewqlink;

        goto inc_q_cnt;
    }

    // Get the last link
    pqlink = pqueue->pend;

    // Stick the new link after the last link
    pqlink->pnext = pnewqlink;
    pnewqlink->pprev = pqlink;

    // Make the new link, the last link
    pqueue->pend = pqlink->pnext;

    // If the list is empty, make the new link the start of the queue as well
    if (pqueue->cnt == 0) {
        pqueue->pstart = pnewqlink;
        pqueue->pcur = pqueue->pstart;
    }

inc_q_cnt:
    pqueue->cnt++;
}

/**
 * Pops the pointer to the item on the list off the first link.
 * The algorithm will take care of freeing used blocks for its own system
 * The caller needs to take care of freeing the item itself
 */
void *queue_popi(queue *pqueue) {
    void *pitem;
    qlink *pqlink;

    // In case there are no links in the queue
    if (pqueue->cnt == 0) return NULL;

    // Get the first link of the queue, and put it on the stack
    pqlink = pqueue->pstart;

    // Make the next link the first link
    pqueue->pstart = pqlink->pnext;

    // Get the pointer to the item
    pitem = pqlink->pitem;

    // Free the link on the stack
    __FREE(pqlink);

    // Decrease cnt;
    pqueue->cnt--;
    // Return the pointer to the item
    return pitem;
}

/**
 * Returns the next item in the queue
 */
qlink *queue_nexti(queue *pqueue){
    qlink *pcur;

    pcur = pqueue->pcur;

    // This occurs when we've reached the end of the list
    if (pcur == NULL) return NULL;

    pqueue->pcur = pcur->pnext; // Set the internal pointer to the next link

    return pcur;
}

/**
 * Sets the internal pointer to the first link in the queue
 */
void queue_rewind(queue *pqueue) {
    pqueue->pcur = pqueue->pstart;
}

/**
 * Removes a link from the queue
 */
void queue_unlink(queue *pqueue, qlink *pqlink) {
    qlink *pprev, *pnext;

    // Failsafe
    if (pqlink == NULL) return;


    // There is only one item in the list that will be unlinked
    //
    if (pqlink == pqueue->pstart && pqlink == pqueue->pend) {
        pqueue->pstart = NULL;
        pqueue->pend   = NULL;
        pqueue->pcur   = NULL;

        goto queue_unlink_free; 
    }

    // If the to be deleted qlink is at the end
    if (pqlink == pqueue->pend) {
        // Make previous link the end of the chain.
        pqueue->pend = pqlink->pprev;

        // Erase the reference to the to be deleted qlink
        pqlink->pprev->pnext = NULL;

        // If the to be deleted qlink is the current, make the previous the current
        if (pqueue->pcur == pqlink)
            pqueue->pcur == pqlink->pprev;

        goto queue_unlink_free;
    }

    // If the to be deleted link is at the start
    if (pqlink == pqueue->pstart) {
        // Make the next link the start of the chain
        pqueue->pstart = pqlink->pnext;

        // Erase the reference to the deleted link
        pqlink->pnext->pprev = NULL;

        // If the to be deleted qlink is the current, make the next the current
        if (pqueue->pcur == pqlink)
            pqueue->pcur == pqlink->pnext;

        goto queue_unlink_free;
    }

    // The link is somewhere in the middle of the chain at this point
    pqlink->pnext->pprev = pqlink->pprev;
    pqlink->pprev->pnext = pqlink->pnext;
    if (pqueue->pcur == pqlink);
        pqueue->pcur = pqlink->pnext;


    
queue_unlink_free:
    // Free the memory of the qlink
    __FREE(pqlink);
    pqueue->cnt--;

    return;
}



