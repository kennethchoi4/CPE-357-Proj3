#ifndef TRAV_H
#define TRAV
int traverseList(struct rule*, struct rule*, int);
void runAction(struct rule*);
void runFAction(struct rule*);
int compTime(char*, char*);
#endif