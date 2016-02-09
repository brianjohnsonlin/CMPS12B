// Brian Lin bjlin
// $Id: hashset.c,v 1.7 2013-05-23 15:41:07-07 - - $

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debugf.h"
#include "hashset.h"
#include "strhash.h"

#define HASH_NEW_SIZE 15

typedef struct hashnode hashnode;
struct hashnode {
   char *word;
   hashnode *link;
};

struct hashset {
   size_t size;
   size_t load;
   hashnode **chains;
};

hashset *new_hashset (void) {
   hashset *this = malloc (sizeof (struct hashset));
   assert (this != NULL);
   this->size = HASH_NEW_SIZE;
   this->load = 0;
   size_t sizeof_chains = this->size * sizeof (hashnode *);
   this->chains = malloc (sizeof_chains);
   assert (this->chains != NULL);
   memset (this->chains, 0, sizeof_chains);
   DEBUGF ('h', "%p -> struct hashset {size = %d, chains=%p}\n",
                this, this->size, this->chains);
   return this;
}

void free_hashset (hashset *this) {
   DEBUGF ('h', "free (%p)\n", this);
   for(size_t i = 0; i < this->size; i++){
      hashnode *curr = this->chains[i];
      while(curr != NULL){
         hashnode *tmp = curr;
         curr = curr->link;
         free(tmp->word);
         free(tmp);
      }
   }
   free(this->chains);
   free(this);
}

void double_hashset(hashset *this){
   DEBUGF ('h', "double (%p)\n", this);
   size_t old_size = this->size;
   this->size = this->size * 2 + 1;
   size_t sizeof_chains = this->size * sizeof (hashnode *);
   hashnode **tmp_chains = malloc (sizeof_chains);
   assert (tmp_chains != NULL);
   memset (tmp_chains, 0, sizeof_chains);
   for(size_t i = 0; i < old_size; i++){
      hashnode *tmpnode = this->chains[i];
      while(tmpnode != NULL){
         int hashnum = strhash(tmpnode->word) % this->size;
         hashnode *curr = tmp_chains[hashnum];
         if(curr == NULL) tmp_chains[hashnum] = tmpnode;
         else{
            while(curr->link != NULL) curr = curr->link;
            curr->link = tmpnode;
         }
         hashnode *tmp = tmpnode->link;
         tmpnode->link = NULL;
         tmpnode = tmp;
      }
   }
   //free(this->chains); //causes abort core dump
   this->chains = tmp_chains;
}

void put_hashset (hashset *this, char *item) {
   hashnode *newNode = malloc(sizeof(struct hashnode));
   newNode->word = item;
   hashnode *curr = this->chains[strhash(item) % this->size];
   if(curr == NULL){
      this->chains[strhash(item) % this->size] = newNode;
   }else{
      while(curr->link != NULL){
         if(strcmp(curr->word, item) == 0){
            free(newNode);
            return;
         }
         curr = curr->link;
      }
      curr->link = newNode;
   }
   this->load++;
   if(this->load * 2 > this->size) double_hashset(this);
}

bool has_hashset (hashset *this, char *item) {
   hashnode *curr = this->chains[strhash(item) % this->size];
   while(curr != NULL){
      if(strcmp(curr->word, item) == 0) return true;
      curr = curr->link;
   }
   return false;
}

void debug_clusters (hashset *this){
   DEBUGF ('h', "debug clusters of (%p)\n", this);
   printf("%6zd words in hash set\n", this->load);
   printf("%6zd size of hash array\n", this->size);
   int clusters[1024] = {0};
   for(size_t i = 0; i < this->size; i++){
      int count = 0;
      hashnode *curr = this->chains[i];
      while(curr != NULL){
         curr = curr->link;
         count++;
      }
      clusters[count]++;
   }
   for(int i = 1; i < 1024; i++) if(clusters[i]!=0)
      printf("%6d clusters of size %3d\n", clusters[i], i);
}

void debug_cluster_info (hashset *this){
   DEBUGF ('h', "debug cluster information of (%p)\n", this);
   for(size_t i = 0; i < this->size; i++)
      if(this->chains[i] != NULL){
         char *str = this->chains[i]->word;
         printf("array[%10zd] = %19zu \"%s\"\n",i,strhash(str),str);
         hashnode *curr = this->chains[i]->link;
         while(curr != NULL){
            str = curr->word;
            printf("%19s %19zu \"%s\"\n","=",strhash(str),str);
            curr = curr->link;
         }
      }
}
