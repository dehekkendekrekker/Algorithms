#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "stresstest.h"
#include "algo.h"

typedef struct sequence {
     char label[10];
 } sequence;
 


sequence* init_seq() {
     sequence *pseq = malloc(sizeof(sequence));
     strcpy(pseq->label, "Hello!");
     return pseq;
}

typedef struct ipbuffer {
    int size;
    __be32 *ip[];
} ipbuffer;


btnode *ipbintree = NULL; 
stack *pstack = NULL;



void addtsip(btnode ** bintree, __be32 ts, __be32 ip) {
    void **apleaf;

    apleaf = addbtbranch(bintree, ts);

    
}





 
 /**
  * Reads a line byte by byte until the next newline
  */
 int readln(char *buf) {
     int bytesread;
     int i = 0;
 
     while (1) {
         if (0 == read(0, buf + i, 1)) {
             return 0;
         }
         if (*(buf + i) == '\n') {
             *(buf + i) = 0;
             return i;
         }
         i++;
     }
}




/**
 * Testing programs
 */



void program1(void) {
    char buf[16];
    char *leaf;
    void **apleaf;
    int counter = 0;
    __be32 ip;


    puts("Adds each IPv4 address to the binary tree. Check memory consumption with valgrind");
    while(1) {
        if (readln(buf) == 0) break;
        
        ip = inet_addr(buf);

        apleaf = addbtbranch(&ipbintree, ip);
        if (*apleaf == NULL) {
            leaf = malloc(0);
            *apleaf = leaf;
        }

        counter++;
    }

    printf("Added %i to binary tree\n", counter);
}

void program2(void) {
    char buf[16];
    char *leaf;
    void **apleaf;
    int counter = 0;
    __be32 ip;

    puts("Inserts an IPv4 address into the binary tree, and immediately deletes it.");
    while(1) {
        if (readln(buf) == 0) break;
        
        ip = inet_addr(buf);

        apleaf = addbtbranch(&ipbintree, ip);
        if (*apleaf == NULL) {
            leaf = malloc(0);
            *apleaf = leaf;
        }
        delbtbranch(&ipbintree, ip);

        counter++;
    }

    printf("Added %i to binary tree\n", counter);
}

void program3(void) {
    char buf[16];
    char *leaf;
    int counter = 0;
    __be32 ip;
    void **apleaf;

    puts("Adds each IPv4 address to the binary tree as a whole. When done, deletes the whole tree");
    while(1) {
        if (readln(buf) == 0) break;
        
        ip = inet_addr(buf);

        apleaf = addbtbranch(&ipbintree, ip);
        if (*apleaf == NULL) {
            leaf = malloc(0);
            *apleaf = leaf;
        }

        counter++;
    }

    delbtree(&ipbintree);
}

void program4(void) {
    char buf[16];
    char *leaf;
    void **apleaf;
    __be32 ip;


    // Init the stack
    init_stack(&pstack);


    puts("Adds each IPv4 address to the binary tree. Then deletes them in 1 second intervals");
    while(1) {
        if (readln(buf) == 0) break;
        
        ip = inet_addr(buf);

        apleaf = addbtbranch(&ipbintree, ip);
        if (*apleaf == NULL) {
            leaf = malloc(0); // Just some dummy allocation
            *apleaf = leaf;
        }
        delbtbranch(&ipbintree, ip);
    }

    destroy_stack(&pstack);
 
}


int main(int argc, char *argv[]) {
    int choice;

    if (argc != 2) {
        choice = 1;
    } else {
        choice = atoi(argv[1]);
    }

    switch(choice) {
        case 2:
            program2();
            break;
        case 3:
            program3();
            break;
        case 4:
            program4();
            break;
        case 1:
        default:
            program1();
    }

    exit(1);
}


