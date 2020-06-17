#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include "algotest.h"
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
    char *pbuf;
    queue ips;

    ips = init_queue();

    puts("Adds each IPv4 address to a queue. When all have been added, they are popped off the queue");
    while(1) {
        if (readln(buf) == 0) break;

        pbuf = malloc(16);
        memcpy(pbuf, buf, 16);
  //      printf("PUSH: %s\n", pbuf);
        queue_pushi(&ips, pbuf);
    }

    printf("Total items in queue: %i\n", ips.cnt);

    while ((pbuf = queue_popi(&ips)) != NULL) {
//        printf("POP: %s\n", pbuf);
        free(pbuf);
    }  
}


int main(int argc, char *argv[]) {
    int choice;
    struct timespec ts;
    time_t starttime, endtime;

    clock_gettime(CLOCK_REALTIME, &ts);
    starttime = ts.tv_sec;
    

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

    clock_gettime(CLOCK_REALTIME, &ts);
    endtime = ts.tv_sec;

    printf("Runtime: %i seconds\n", endtime-starttime);

    exit(1);
}


