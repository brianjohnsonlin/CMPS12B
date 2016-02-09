// Brian Lin bjlin
// $Id: strhash.h,v 1.1 2012-11-16 18:05:22-08 - - $

//
// NAME
//    strhash - return an unsigned 32-bit hash code for a string
//
// SYNOPSIS
//    size_t strhash (char *string);
//

#ifndef __STRHASH_H__
#define __STRHASH_H__

#include <inttypes.h>

size_t strhash (char *string);

#endif

