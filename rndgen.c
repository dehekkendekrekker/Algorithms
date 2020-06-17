#include <stdio.h>
#include <stdlib.h>
#include <linux/types.h>
#include <arpa/inet.h>

void main(int argc, char *argv[]){
    int limit = -1;
    int i;
    struct in_addr in;
    __be32 rnd;

    if (argc ==2) {
        limit = atoi(argv[1]);
    }

    while (1) {
        rnd = rand();
        in.s_addr = rnd;
        printf("%s\n", inet_ntoa(in));

        i++;
        if (i == limit) break;
    }

    exit(1);
}
