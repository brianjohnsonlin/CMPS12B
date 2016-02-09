//andrew lien
//ahlien
//linesort.c

#include <stdlib.h>
#include <libgen.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "inssort.h"

#define MAX_BUFFER 1000
#define MAX_LINE 4096

char *progname = NULL;

//inspired by strstrcmp.c
int strstrcmp(const void *this, const void *that){
  char **thisstr = (char**) this;
  char **thatstr = (char**) that;
  return strcmp (*thisstr, *thatstr);
} 
int main (int argc, char **argv){
  progname = basename(argv[0]);
  char line[MAX_LINE];
  char **buffer = NULL;
 int iVal = 0;
  buffer = (char**)malloc(sizeof(char*) * MAX_BUFFER);
  while(fgets(line,MAX_LINE,stdin)){
    if(feof(stdin)) break;
    
    buffer[iVal] = strdup(line);
    iVal++;
    if (iVal == MAX_BUFFER) break;
  }
  int i = argc;
  i++;
  inssort(buffer,iVal,sizeof(buffer[0]),strstrcmp);
  //at this point, buffer should be sorted
  for(int i = 0; i < iVal; i++){
   // if(buffer[i] !=NULL){
      printf("%s", buffer[i]);
    //}
  }
  //at this point, buffer isn't need, need to free ALL char
  for(int freeCount = 0; freeCount < iVal; freeCount++){
    free(buffer[freeCount]);
  }
  free(buffer);
}

