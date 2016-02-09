// Brian Lin bjlin
// $Id: strhash.c,v 1.4 2013-08-12 13:28:04-07 - - $

#include <assert.h>
#include <stdio.h>
#include <sys/types.h>

#include "strhash.h"

//
// DESCRIPTION
//    Uses Horner's method to compute the hash code of a string
//    as is done by java.lang.String.hashCode:
//    .  s[0]*31^(n-1) + s[1]*31^(n-2) + ... + s[n-1]
//    Using strength reduction, the multiplication is replaced by
//    a shift.  However, instead of returning a signed number,
//    this function returns an unsigned number, and is 64 bits
//    rather than 32 bits.
//
// REFERENCE
//    http://java.sun.com/j2se/1.4.1/docs/api/java/lang/
//    String.html#hashCode()
// 


size_t strhash (char *string) {
   assert (string != NULL);
   size_t hashcode = 0;
   while (*string) hashcode = (hashcode << 5) - hashcode + *string++;
   return hashcode;
}

