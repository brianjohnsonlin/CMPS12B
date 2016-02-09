// $Id: hashset.h,v 1.2 2013-03-04 20:49:14-08 - - $

#ifndef __HASHSET_H__
#define __HASHSET_H__

#include <stdbool.h>

typedef struct hashset hashset;

//
// Create a new hashset with a default number of elements.
//
hashset *new_hashset (void);

//
// Frees the hashset, and the words it points at.
//
void free_hashset (hashset *);

//
// Inserts a new string into the hashset.
//
void put_hashset (hashset *, char*);

//
// Looks up the string in the hashset and returns true if found,
// false if not found.
//
bool has_hashset (hashset *, char*);

//
// When -x option is specified more than once, dump
// hash set, printing out each entry with subscript,
// hash code, and string.
void hash_check(hashset *, int);
#endif

