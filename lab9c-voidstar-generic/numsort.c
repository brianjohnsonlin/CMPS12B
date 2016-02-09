// Brian Lin bjlin

#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <math.h>
#include "inssort.h"

#define SIZE 1000

int compareDouble(const void *this, const void *that){
   return *(double*)this - *(double*)that;
}

int main(int argc, char **argv) {
   argc=argc; argv=argv;
   double array[SIZE];
   int count = 0;
   while(count < SIZE){
      double num;
      int scan = scanf("%lg", &num);
      if(scan == 0 || scan == EOF) break;
      array[count] = num;
      count++;
   }
   inssort(array, count, sizeof(double), compareDouble);
   for(int i = 0; i < count; i++) {
      printf("%20.15g\n", array[i]);
   }
   return EXIT_SUCCESS;
}
