#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "pop.h"
#include "func.h"
#include "free.h"
#include "trav.h"

#define FAILURE -1
#define LINELEN 80

void freeRuleList(struct rule* head)
{
   if (head)
   {
      if (head->dep) {
         freeNodeList(head->dep);
      }
      if (head->act) {
         freeNodeList(head->act);
      }
      if (head->next) {
         freeRuleList(head->next);
      }
   }
   free(head->target);
   free(head);
   return;
}
void freeNodeList(struct node* head) {
   if (head) {
      if (head->next) {
         freeNodeList(head->next);
      }
   }
   /* free(head->data); */
   free(head);
   return;
}
