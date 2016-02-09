// $Id: list1.c,v 1.3 2013-02-08 18:00:41-08 - - $

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node *node_ref;
struct node {
   char *word;
   node_ref link;
};

int main (int argc, char **argv) {
   node_ref head = NULL;
   for (int argi = 0; argi < 5; ++argi) {
      node_ref node = malloc (2*sizeof (node_ref));
      assert (node != NULL);
      node->word = argv[argi];
      node->link = head;
      head = node;
   }
   for (node_ref curr = head; curr->link != NULL; curr = curr->link) {
      printf ("%p->node {word=%p->[%s], link=%p}\n",
              curr, curr->word, curr->word, curr->link);
   }
   node_ref curr = head;
   while(curr != NULL){
      node_ref tmp = curr->link;
      free(curr);
      curr = tmp;
   }
   return EXIT_SUCCESS;
}
