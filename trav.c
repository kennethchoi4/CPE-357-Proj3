#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>

#include "pop.h"
#include "trav.h"
#include "func.h"

#define FAILURE -1
#define LINELEN 80

/* Head rule kept for traversal and update flag set when comparing */
int traverseListORIG(struct rule* head, struct rule* curr, int update) {
   struct node* tDep = curr->dep;
   if (!tDep) {
      runAction(curr);
   }
   while (tDep) {
      struct rule* tRule = head;
      while (tRule) {
         if (!strcmp(tRule->target,tDep->data)) {
            update = traverseList(head, tRule, update);
         }
         /* Iter thru Rule list to find dep target*/
         if (!tRule->next) {
            break;
         }
         tRule = tRule->next;
      }

      /* echo or rule like that */
      if((compTime(curr->target, tDep->data)) == -1) {
         runAction(curr);
      }

      if (!tDep->next) {
         break;
      }
      tDep = tDep->next;
   }
   return update;
}

int traverseList(struct rule* head, struct rule* curr, int update) {
   /* target is curr */
   struct node* tDep = curr->dep;

   /* if no dep, run actions */
   if (!tDep) {
      runAction(curr);
      return update;
   }
   
   /* for each dep, check if its also a rule */
   while (tDep) {
      struct rule* tRule = head;
      while (tRule) {
         if (!strcmp(tRule->target,tDep->data)) {
            /* if so recurse */
            update = traverseList(head, tRule, 0);
            break;
         }
         /* Iter thru Rule list to find dep target*/
         if (!tRule->next) {
            break;
         }
         tRule = tRule->next;
      }

      /* echo or rule like that */
      if(!(compTime(curr->target, tDep->data))) {
         update = 1;
      }


      if (!tDep->next) {
         break;
      }
      tDep = tDep->next;
   }
   /* before return, if update is high, run actions */
   if (update) {
      runAction(curr);
   }
   return update;

}


void runAction(struct rule* rule) {
   /* Only Update if flag is high! */
   struct node* act = rule->act;
   while (act) {
      printf("%s\n",act->data);
      char* str = act->data;
      if (system(str) == -1) {
         perror("ERROR");
      }
      if (act->next) {
         act = act->next;
      }
      else {
         return;
      }
   }
}


int compTime(char* targ, char* dep){
   struct tm *t1, *t2;
   struct stat buf1, buf2;

   stat(targ, &buf1);
   t1 = gmtime(&(buf1.st_mtime));  
   /* t1 timing */
   int year1 = t1->tm_year;
   int mon1 = t1->tm_mon;
   int day1 = t1->tm_mday;
   int hour1 = t1->tm_hour;
   int min1 = t1->tm_min;
   int sec1 = t1->tm_sec;

   stat(dep, &buf2);
   t2 = gmtime(&(buf2.st_mtime)); 
   /* t2 timing */
   int year2 = t2->tm_year;
   int mon2 = t2->tm_mon;
   int day2 = t2->tm_mday;
   int hour2 = t2->tm_hour;
   int min2 = t2->tm_min;
   int sec2 = t2->tm_sec;

   if(year1!=year2){
      return t1->tm_year>t2->tm_year;
   }
   if(mon1!=mon2){
      return t1->tm_mon>t2->tm_mon;
   }
   if(day1!=day2){
      return t1->tm_mday>t2->tm_mday;
   }
   if(hour1!=hour2){
      return t1->tm_hour>t2->tm_hour;
   }
   if(min1!=min2){
      return min1>min2;
   }
   if(sec1!=sec2){
      return sec1>sec2;
   }
   else {
      return 0;
   }
}