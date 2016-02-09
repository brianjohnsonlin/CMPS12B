//andrew lien
//ahlien

#include <libgen.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "inssort.h"

//inspired by java Insertion Sort in lab9 PDF
void inssort (void *base, size_t nelem, size_t size,
              int (*compar) (const void *, const void *)){
  for(size_t sorted = 1; sorted < nelem; ++sorted){
    size_t slot = sorted;
    void *copy = malloc(size); //use alloca instead
    //char copy[size], we can declare an array local to data
    //memcpy(copy to, copied from, the size of it); 
    memcpy(copy, (char *)(base + slot *size), size);
    for(; slot>0; --slot){
      void *previous = base + ((slot -1) * size);
      int cmp = (*compar)(copy,previous);
      if (cmp > 0) break;
      memcpy((base + slot * size), previous, size);
    }
      memcpy((base + slot*size), copy,size);
      
      free(copy);
  }
}
 
