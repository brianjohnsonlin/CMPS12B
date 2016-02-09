// $Id: hashset.c,v 1.7 2013-05-23 15:41:07-07 - - $
//andrew lien
//ahlien
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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
//chains IS an array

/*
So like you have an array, pretty much, you can think of it
as an up and down array, and each element of that array
has a corresponding hash code value, and thats how it knows
which word goes with which position. There's a function he gives
us, it's caleld strhash.c, don't mess with it, but we just send
the word that we send in to the strhash.c and that gives you a
value. So then, that value is goign to act as your index point
It's in hash hashcode_t, that's your index now, and that's going
to basically find the point in the has you're going to start at 
and then within that chain, there's a linked list,

*/

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
   for(int i = 0; i < (int)this->size; i++) free(this->chains[i]);
   memset(this->chains,0,this->size*sizeof(char*));
   free(this->chains);
   memset(this,0,sizeof(this));
   free(this);
} 

void doubletheArray(hashset *this){
   //printf("Here is where i am going to double the array\n");
   int previousSize = this->size;
   this->size = (previousSize * 2) + 1;
   //allocate a new array & set its pointers to null
   char **newhashnode = malloc (this->size * sizeof (char*));
   for (int i = 0; i < (int)this-> size; i++) newhashnode[i] = NULL;
   //for each entry in the OLD array 
   for (int i = 0; i < previousSize; i++) { 
     //used if statement instead of while loop, continue
      if (this->chains[i] == NULL) continue; 
       //popped hash node from this->chains &
       //recomputed the hash number & hash index for new array
      int newIndex = strhash(this->chains[i]) % this->size; 
      //push the hash node onto the index of the new array
      while (newhashnode[newIndex] != NULL) { 
        if (strcmp(newhashnode[newIndex], this->chains[i]) == 0) break; 
        //when newhash matches size of thischains, break;
        //increment new index based on % this size
         newIndex = (newIndex + 1) % this->size; 
      }
       //give newhashnode properties of this->chains
      newhashnode[newIndex] = this->chains[i];
      //at this point newhasnode has properties of 
      //this->chains & has bigger array
   }
   char **tmp = this->chains;
   this->chains = newhashnode; 
   free(tmp); //free old this->chains
}

//if word is put into hash, ignore it, append to end of linked list
//loading the dictionary
void put_hashset (hashset *this, char *item) {
  // STUBPRINTF ("hashset=%p, item=%s\n", this, item);

  //from implementation 5(e)
  if((this->load*2) > this->size) doubletheArray(this);
  hashcode_t ind = (strhash(item)%this->size);
  while(this->chains[ind] != NULL){
    ind = (ind + 1) % this->size;
  }
  this->chains[ind] = strdup(item);
  this->load++;
}
//the spell checking portion, you're going to input
//another filname, and that file is goign to  have word
// that is spelled correctly or incorectly, and you're
//going to output the word that's spelled correctly.
bool has_hashset (hashset *this, char *item) {
   //STUBPRINTF ("hashset=%p, item=%s\n", this, item);
  //In order to avoid negative numbers, use defined data type
  //hashcode_t, defined to be an unsigned 32-bit integer.
  hashcode_t ind = (strhash(item) % this->size);
  while(this->chains[ind] != NULL){
    if(strcmp(this->chains[ind],item) == 0) return true;
    ind = (ind + 1) % this->size;//perform linear search down hash chain
  }

  //convert to lower case, implementation 6(ii);
  int a = 0;
  char *low = item;
  while(item[a] != NULL){
  low[a] = (tolower(item[a])); //tolower makes lowercases
  a++;
 }
 ind = (strhash(low) % this->size);
  while(this->chains[ind] != NULL) {
    if(strcmp(this->chains[ind],low) == 0) return true;
    ind = (ind + 1) % this->size;
  }
   return false;
}
//for implementation 5
void hash_check(hashset *this, int hashDump){
  //printf("It's calling the hash check function\n");
   int j = 0;
  //need cluster array a quarter of main hash
  int cluster[this->size/4]; //no need to malloc
  //initialize this new array to null
  for(size_t i = 0; i< (int)this->size/4; i++) cluster[i] = NULL;
   int clusterCount = 0; 
   for (int i = 0; i < (int)this->size; i++){
     if ((int)this->size == i-1){ //the last element of the array
       if ((this->chains[i] =! NULL) && (this->chains[0] == NULL)) {
         ++cluster[clusterCount];
         clusterCount = 0;
       }
       if ((this->chains[i] == NULL) && (this->chains[0] == NULL)) {
          //merge two clusers
          int firstclusterCount=0;
          for (int k = 0; k < (int)this->size; k++){
          //find size of first cluster
            if (this->chains[k] != NULL) {
              ++firstclusterCount;
            } else {
                --cluster[firstclusterCount];
                ++cluster[firstclusterCount + clusterCount];
              }
          }
       }
    }else if (this->chains[i] != NULL) {
       j++;  //for counting number of words in hashset
       ++clusterCount;
     } else {
        ++cluster[clusterCount];
        clusterCount = 0;
       }
     }
   printf ("%10d words in the hash set\n", j);  
   printf ("%10zu length of the hash array\n", this->size); 
   int sum = 0;
   for (int i = 1; i < this->size/4; i++) {
     if (cluster[i] != 0) {
       printf("%10d clusters of size %i\n", cluster[i] , i);
       sum+=(cluster[i]*i);
     }
   }
   //printf("%10d Sum of clusters 
   //which should equal word count\n", sum);
   if (hashDump > 1) {
     for (int i = 0; i < this->size; i++){
       if (this->chains[i] != NULL) {
         char *word = this->chains[i];
         hashcode_t hash = strhash(word);
         printf("array[%10d] = %12u \"%s\"\n", i, hash, word); 
       }
     }
  }
}





