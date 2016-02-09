// Brian Lin bjlin
// $Id: nsort.c,v 1.1 2011-04-29 19:46:42-07 - - $

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

typedef struct node *node_ref;

struct node{
    double value;
    node_ref link;
};

node_ref head = NULL;
int EXIT_STATUS = 0;
bool debug = false;

void insert(node_ref node){
   if(head == NULL) head = node;
   else if(node->value < head->value){
      node->link = head;
      head = node;
   }else{
      node_ref cur = head;
      while(cur->link != NULL){
         if(node->value < cur->link->value){
            node->link = cur->link;
            cur->link = node;
            break;
         }
         cur = cur->link;
      }
      if(cur->link == NULL) cur->link = node;
   }
}

void read_stdin(void){
   while(true){
      double num = 0;
      int input = scanf("%lg", &num);
      if(input == EOF) break;
      if(input == 0){
         fprintf(stderr, "%s\n", "Error detected");
         EXIT_STATUS = EXIT_FAILURE;
         break;
      }
      node_ref node = malloc(sizeof(struct node));
      assert(node != NULL);
      node->value = num;
      node->link = NULL;
      insert(node);
   }
}

void dumpList(void){
   node_ref cur = head;
   while(cur!=NULL){
      if(debug)
         printf("%p-> {%lg, %p]\n",cur,cur->value,cur->link);
      else printf("%24.15g\n",cur->value);
      node_ref tmp = cur;
      cur = cur -> link;
      free(tmp);
   }
}

int main(int argc, char **argv){
   while(true){
      int opt = getopt(argc, argv, "d");
      if(opt == EOF) break;
      if(opt == 'd') debug = true;
   }
   read_stdin();
   dumpList();
   return EXIT_STATUS;
}
