#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "pop.h"
#include "func.h"

#define FAILURE -1
#define LINELEN 80

/* Allocs */
struct rule *ruleAlloc(){          /* allocates space for a node */
   return (struct rule *) malloc(sizeof(struct rule));
}

struct node *nodeAlloc(){          /* allocates space for a node */
   return (struct node *) malloc(sizeof(struct node));
}

/* create structs */
struct rule *createRule(){
   struct rule *newRule = ruleAlloc();
   newRule->target = NULL;
   newRule->dep = NULL;
   newRule->act = NULL;
   return newRule;
}
 
struct node *createNode(){
   struct node *newNode = nodeAlloc();
   newNode->data = NULL;
   newNode->next = NULL;
   return newNode;
}

/* Returns ptr to rest of target header w/o "target:"
 changes target input to "target*/
char* getTarget(char* line, char** target) {
   *target = strtok(line, " :"); /* Target Delim */
   return strtok(NULL, "\n"); /* Rest of line Delim */
}

int colon(char *string){
   int i;
   for(i = 0; i < strlen(string); i++){
       if (string[i] == ':'){
           return 1;
       }
   }
   return 0;
}

char *refineAction(char *string){
    char* result = malloc(strlen(string)*sizeof(char));
    string++;
    memcpy(result, string, strlen(string)*sizeof(char));
    /* char *result = string + 1; */  /* removes first character */
    
    /* free(string); */
    if (result[strlen(result)-1] == '\n') {
      result[strlen(result)-1] = '\0'; /* removes last character */
    }

    return result;
}


struct rule *recRuleList(FILE* file) {
    size_t size = sizeof(char) * LINELEN;
    int valid;
    char* tmp = NULL;
    tmp = malloc(size);
    skipNewLine(&file);
    valid = getline(&tmp, &size, file);
    if (!valid) {
        free(tmp);
        return NULL;
    }
    if (tmp[0] == '\t') {
        printf("Invalid Smakefile Format.\n");
        exit(FAILURE);
    }

    int cVal;
    if ((cVal = colon(tmp)) && tmp[0] != '\n') {
        struct rule* cur = createRule();
        if (tmp) {
            char* recL = getTarget(tmp, &(cur->target));
            cur->dep = recDepList(recL);
            cur->act = recActList(file);
            cur->next = recRuleList(file);
            /* free(recL); */
            return cur;
        }
    }
    free(tmp);
    return NULL;
}


struct node *recDepList(char *string) {
   /* string ptr pos is saved use NULL in rem of strtok() calls.*/
    char *tempStr;
    char* tmp = malloc((strlen(string)+1)*sizeof(char));
    strcpy(tmp,string);
    if (strchr(tmp, ':') == NULL) {
        printf("test6.smake:1: *** missing separator.  Stop.\n");
        exit(-1);
    }
    tempStr = strtok(tmp, ": \n");
    if (!tempStr) {
        return NULL; /* return null if no deps */ 
    }
 
    struct node *head = createNode();   /* stores original node pointer */
    struct node *cur = head; /* Keep Head ptr */
    cur->data = tempStr;
 
    while(tempStr = strtok(NULL, ": \n")){ /* Create New Node while not at end of line */
        cur->next = createNode();
        cur = cur->next;
        cur->data = tempStr;
    }
    /* free(string); */
    return head; /* Becomes head of rule's dep list */
}

struct node *recActList(FILE* file) {

    size_t size = sizeof(char) * LINELEN;
    
    skipNewLine(&file);

    int c = getc(file);
    if ((c != '\t' && c != '\n' && c != EOF)) {
        ungetc(c,file);
        return NULL;
    }
    ungetc(c,file);

    char* tmp =  malloc(size);
    int valid = getline(&tmp, &size,file);

    if ((valid != -1) && tmp[0] == '\t' \
        && tmp[0] != '\n' \
        && strlen(tmp) != 0) {

        struct node* cur = NULL;

        cur = createNode();
        cur->data = refineAction(tmp);
        cur->next = recActList(file);
        free (tmp);
        return cur;
    }
    free(tmp);
    return NULL;
}