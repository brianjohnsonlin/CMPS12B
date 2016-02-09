// Brian Lin bjlin
// $Id: lsort.c,v 1.1 2011-04-29 19:46:42-07 - - $

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

typedef struct node *node_ref;
typedef char *cstring;

struct node{
    cstring value;
    node_ref link;
};

int EXIT_STATUS = 0;
node_ref head = NULL;
bool debug = false;

void insert(node_ref node){
   if(head == NULL){
       head = node;
   }
   else if(strcmp(node->value,head->value)<0){
      node->link = head;
      head = node;
   }else{
      node_ref cur = head;
      while(cur->link != NULL){
         if(strcmp(node->value,head->value)<0){
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
   char buffer[72];
   while(true){
      char *input = fgets(buffer, sizeof buffer, stdin);
      if(input == NULL) break;
      char *nl = strchr(buffer,'\n');
      if(nl != NULL) *nl='\0';
      else{
         fprintf(stderr, "%s\n", "Error detected");
         EXIT_STATUS = EXIT_FAILURE;
      }
      node_ref node = malloc(sizeof(struct node));
      assert(node != NULL);
      node -> value = strdup(buffer);
      assert(node->value != NULL);
      node -> link = NULL;
      insert(node);
   }
}

void dumpList(void){
   node_ref cur = head;
   while(cur!=NULL){
      if(debug)
         printf("%p-> {%s, %p]\n",cur,cur->value,cur->link);
      else printf("%s\n",cur->value);
      node_ref tmp = cur;
      cur = cur -> link;
      free(tmp->value);
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
