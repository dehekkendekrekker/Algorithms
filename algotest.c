#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include "algo.h"

typedef struct sequence {
     char label[10];
 } sequence;
 


sequence* init_seq() {
     sequence *pseq = malloc(sizeof(sequence));
     strcpy(pseq->label, "Hello!");
     return pseq;
}

typedef struct ts2ipbuffer {
    long ts;
    __be32 ip;
} ts2ipbuf;


btnode *ipbintree = NULL; 
queue ipqueue;





 
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


void handle_alarm(int sig) {
    ts2ipbuf *pts2ip;

    printf("Time's up!\n");
    
    printf("Cleaning up %i items\n", ipqueue.cnt);
    while ((pts2ip = queue_popi(&ipqueue)) != NULL) {
        delbtbranch(&ipbintree, pts2ip->ip);
        free(pts2ip);
    }
    puts("All done");
    exit(1);
}

void program5(void) {
    struct timespec ts;
    char buf[16];
    char *leaf;
    int counter = 0;
    __be32 ip;
    void **apleaf;
    ts2ipbuf *pts2ip;

    puts("Inserts as many IP addresses and timestamps as it can in one second. Will the proceed to delete them all");

    ipqueue = init_queue();
    signal(SIGALRM, handle_alarm);
    alarm(2);

    while(1) {
        if (readln(buf) == 0) break;

        clock_gettime(CLOCK_REALTIME, &ts);
       
        // Get binary version of ip address
        ip = inet_addr(buf);

        // Create a timestamp 2 ip entry
        pts2ip = malloc(sizeof(ts2ipbuf));
        pts2ip->ip = ip;
        pts2ip->ts = ts.tv_nsec;
        
        // Push the entry onto the queue;
        queue_pushi(&ipqueue, pts2ip);

        // Create a bintree branch for the ip address;
        apleaf = addbtbranch(&ipbintree, ip);
            if (*apleaf == NULL) {
                leaf = malloc(0);
                *apleaf = leaf;
             }
       

        counter++;
    }
}


void program6() {
    queue testqueue;
    qlink *qlink;
    int i;
    char *buf;

    testqueue = init_queue();

    puts("Inserting 10 items");
    for (i =0; i < 3; i++) {
        buf = calloc(10, sizeof(char));
        sprintf(buf, "Hello %i", i);
        queue_pushi(&testqueue, buf);
    }

    puts("Looping through list, displaying item");
    queue_rewind(&testqueue);

    while ((qlink = queue_nexti(&testqueue)) != NULL) {
        printf("%s\n", qlink->pitem);
    }

    puts("Pick out Hello 1, and unlink it (middle)");
    queue_rewind(&testqueue);
    while ((qlink = queue_nexti(&testqueue)) != NULL) {
        if (strcmp(qlink->pitem, "Hello 1") == 0) {
            free(qlink->pitem);
            queue_unlink(&testqueue, qlink);
        }
    }

    puts("Pick out Hello 0, and unlink it (start)");
    queue_rewind(&testqueue);
    while ((qlink = queue_nexti(&testqueue)) != NULL) {
        if (strcmp(qlink->pitem, "Hello 0") == 0) {
            free(qlink->pitem);
            queue_unlink(&testqueue, qlink);
        }
    }

    puts("Pick out Hello 2, and unlink it (end)");
    queue_rewind(&testqueue);
    while ((qlink = queue_nexti(&testqueue)) != NULL) {
        if (strcmp(qlink->pitem, "Hello 2") == 0) {
            free(qlink->pitem);
            queue_unlink(&testqueue, qlink);
        }
    }

    puts("Verify ...");
    queue_rewind(&testqueue);
    while ((qlink = queue_nexti(&testqueue)) != NULL) {
        printf("%s\n", qlink->pitem);
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
        case 5:
            program5();
            break;
        case 6:
            program6();
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


