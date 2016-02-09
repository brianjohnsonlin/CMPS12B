//Andrew Lien
//ahlien
//numsort.c


#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <math.h>
#include "inssort.h"

#define MAX_SIZE 1000
char *progname = NULL;
//example of qsort(3c), inspired by cqsort-int.c
//goal: have it be similar to compareTo where
// smaller numbers = -1, bigger = 1, same = 0
int doublecmp(const void *this, const void *that){
  double *thisdoub = (double*) this;
  double *thatdoub = (double*) that;
  return *thisdoub < *thatdoub ? -1 : *thisdoub > *thatdoub ? +1:0;
  //if(thisdoub<thatdoub) return -1;
  //else if(thisdoub>thatdub)return 1;
  // else return 0;
}
int main(int argc, char **argv) {
 progname = basename (argv[0]);
   double array[MAX_SIZE];
   int count = 0;
   for (;;) {
   //first check if array is full, go to inssort
      if (count == MAX_SIZE) break;
   //retrieve doubles from stdin   
      double num;
      int scan = scanf("%lg", &num);
   //breaks if scan reaches end of file, or unrecognizibles   
       if (scan != 0) array[count] = num; 
       //printf("Your number is: %lg\n", array[count]);
         else break;
      if (scan == EOF) break;
       //if(scan == 0) break;
        count++;
      //  if (scan = 0) return EXIT_FAILURE;  
   }
   //take out the warnings against not using argc
   int i = argc;
   i++;
   //perform inssort 
   inssort(array, count, sizeof (double), doublecmp);
   //at this point, array should be sorted, print
   for (int i = 0; i < count; i++) {
      printf("The %dth number is: ",i);
      printf("%20.15g\n",array[i]);
   }
}
