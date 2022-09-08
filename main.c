#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "pop.h"
#include "trav.h"
#include "func.h"
#include "free.h"

#define FAILURE -1
#define LINELEN 80


int main(int argc, char* argv[]) {
    FILE* file;
    file = fopen("Smakefile", "r");
    if (file == NULL) {
        printf("make: *** No targets specified and no makefile found.  Stop.\n");
        return -1;
    }
    struct rule* head = recRuleList(file);
    struct rule* recHead = checkArgs(argc, argv, head, file);
    exeMake(recHead);
    freeRuleList(head);
    safeClose(file);
    return 0;
}