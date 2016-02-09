// Brian Lin bjlin

#include <stdlib.h>
#include <libgen.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "inssort.h"

#define SIZE 1000

int compareString(const void *this, const void *that){
   return strcmp(*((const char**)this), *((const char**)that));
}

int main (int argc, char **argv){
   argc=argc; argv=argv;
   char* array[SIZE];
   char buffer[1024];
   int count = 0;
   while(fgets(buffer, 1024, stdin) != NULL && count < SIZE){
      if(feof(stdin)) break;
      array[count] = strdup(buffer);
      count++;
   }
   inssort(array, count, sizeof(char*), compareString);
   for(int i = 0; i < count; i++){
      printf("%s", array[i]);
      free(array[i]);
   }
   return EXIT_SUCCESS;
}
