#ifndef POP_H
#define POP_H


/* Struct Defs */
struct node{
   char* data;
   struct node *next;
};

struct rule{
   char *target;
   struct node *dep;
   struct node *act;
   struct rule *next;
};

/* Allocs and creates */
struct rule *ruleAlloc();
struct node *nodeAlloc();
struct rule *createRule();
struct node *createNode();

/* Recursive Calls for Populating 
   linked list of linked lists */
char* getTarget(char*, char**);
int colon(char*);
char* refineAction(char*);
struct rule *recRuleList(FILE*);
struct node *recDepList(char*);
struct node *recActList(FILE*);


#endif