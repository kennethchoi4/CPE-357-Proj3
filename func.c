#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "func.h"
#include "trav.h"
#include "pop.h"
#include "free.h"

#define FAILURE -1
#define LINELEN 80

/* Safe version of malloc() - Uses Calloc for buffer size of 0 */
void* safeAlloc(size_t size) {
    void *ptr = NULL;
    ptr = malloc(size);
    if (!ptr) {
        perror("Out of Memory");
        exit(-1);
    }
    return ptr;
}


/* Safe version of realloc() */
void* safeRealloc(void *ptr, size_t size) {
    ptr = realloc(ptr, size);
    if (!ptr) {
        perror("Out of Memory");
        exit(-1);
    }
    return ptr;
}

/* Error Protector */
FILE *safeOpen(char* fileName, char* mode) {
    void* fp = fopen(fileName, mode);
    return fp;
}

/* Error Protector */
void safeClose(FILE* fp) {
    int closeFlag = fclose(fp);
    if (closeFlag == EOF) {
        perror("Error Closing File");
        exit(-1);
    } 
}

void skipNewLine(FILE** file) {
    while (1) {
        int c = getc(*file);
        if (c != '\n') {
            ungetc(c,*file);
            break;
        }
    }
}


struct rule *checkArgs(int argc, char* argv[], struct rule* head, FILE* file) {
    /* Error Handling Finding File or too many args */
    if (argc == 1) {
        return head;
    }
    if (argc == 2) {
        char* hStr = argv[1];
        struct rule* tmp = head;
        while (tmp) {
            int i;
            if (!(i = strcmp(tmp->target, hStr))) {
                return tmp; 
            }
            if (tmp->next) {
                tmp = tmp->next;
            }
            else {
                perror("Target Not Found");
                exit(FAILURE);
            }
        }
    }
    else {
        perror("Too Many Arguments - Only Need ONE Target Name");
        exit(FAILURE);
    }

}

void exeMake(struct rule* head) {

    if (!head) {
        printf("test6.smake:1: *** missing separator.  Stop.\n");
        exit(FAILURE);
    }

    int update = 0;

    update = traverseList(head, head, update);
}
