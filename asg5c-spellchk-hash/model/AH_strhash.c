// $Id: strhash.c,v 1.3 2013-05-21 19:58:47-07 - - $

#include <assert.h>
#include <stdio.h>
#include <sys/types.h>

#include "strhash.h"
//this file is used to retrieve a hashcode_t value from each word
hashcode_t strhash (char *string) {
   assert (string != NULL);
   hashcode_t hashcode = 0;
   for (; *string != '\0'; ++string) {
      hashcode = hashcode * 31 + *string;
   }
   return hashcode;
}

