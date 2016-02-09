// Brian Lin bjlin
// $Id: bigint.c,v 1.12 2013-05-16 15:14:31-07 - - $

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bigint.h"
#include "debug.h"

#define MIN_CAPACITY 16

struct bigint {
   size_t capacity;
   size_t size;
   bool negative;
   char *digits;
};

static void trim_zeros (bigint *this) {
   while (this->size > 0) {
      size_t digitpos = this->size - 1;
      if (this->digits[digitpos] != 0) break;
      --this->size;
   }
}

bigint *new_bigint (size_t capacity) {
   bigint *this = malloc (sizeof (bigint));
   assert (this != NULL);
   this->capacity = capacity;
   this->size = 0;
   this->negative = false;
   this->digits = calloc (this->capacity, sizeof (char));
   assert (this->digits != NULL);
   DEBUGS ('b', show_bigint (this));
   return this;
}

bigint *new_string_bigint (char *string) {
   assert (string != NULL);
   size_t length = strlen (string);
   bigint *this = new_bigint (length > MIN_CAPACITY
                            ? length : MIN_CAPACITY);
   char *strdigit = &string[length - 1];
   if (*string == '_') {
      this->negative = true;
      ++string;
   }
   char *thisdigit = this->digits;
   while (strdigit >= string) {
      assert (isdigit (*strdigit));
      *thisdigit++ = *strdigit-- - '0';
   }
   this->size = thisdigit - this->digits;
   trim_zeros (this);
   DEBUGS ('b', show_bigint (this));
   return this;
}


static bigint *do_add (bigint *this, bigint *that) {
   DEBUGS ('b', show_bigint (this));
   DEBUGS ('b', show_bigint (that));
   size_t length = (this->size > that->size
                  ? this->size : that->size) + 1;
   bigint *result = new_bigint (length > MIN_CAPACITY
                              ? length : MIN_CAPACITY);
   result->size = length;
   bool carry = false;
   for(size_t i = 0; i < result->size; i++){
      int thisDigit = 0;
      int thatDigit = 0;
      if(i < this->size) thisDigit = this->digits[i];
      if(i < that->size) thatDigit = that->digits[i];
      int resultDigit = thisDigit + thatDigit;
      if(carry) resultDigit++;
      carry=false;
      if(resultDigit>9){
         resultDigit-=10;
         carry=true;
      }
      result->digits[i] = resultDigit;
   }
   trim_zeros(result);
   return result;
}

static bigint *do_sub (bigint *this, bigint *that) {
   //assumes that "this" is the bigger number
   DEBUGS ('b', show_bigint (this));
   DEBUGS ('b', show_bigint (that));
   assert(this->size >= that->size);
   bigint *result = new_bigint (this->size > MIN_CAPACITY
                              ? this->size : MIN_CAPACITY);
   result->size = this->size;
   bool borrow = false;
   for(size_t i = 0; i < result->size; i++){
      int thisDigit = this->digits[i];
      int thatDigit = 0;
      if(i < that->size) thatDigit = that->digits[i];
      if(borrow) thisDigit--;
      borrow=false;
      int resultDigit = thisDigit - thatDigit;
      if(resultDigit<0){
          resultDigit+=10;
          borrow=true;
      }
      result->digits[i] = resultDigit;
   }
   trim_zeros(result);
   return result;
}

void free_bigint (bigint *this) {
   free (this->digits);
   free (this);
}

void print_bigint (bigint *this, FILE *file) {
   DEBUGS ('b', show_bigint (this));
   int numChars = 0;
   if(this->negative){
       fprintf (file, "-");
       numChars++;
   }
   for (char *byte = &this->digits[this->size - 1];
        byte >= this->digits; --byte) {
      if(numChars > 68){
          fprintf (file, "\\\n");
          numChars = 0;
      }
      fprintf (file, "%d", *byte);
      numChars++;
   }
   fprintf (file, "\n");
}

bool thisIsBigger(bigint *this, bigint *that){
   //returns true if equal or "this" is bigger
   //or false if "that" is bigger
   if(this->size > that->size) return true;
   if(this->size < that->size) return false;
   for(int i=this->size-1; i>-1; i--){
       if(this->digits[i] > that->digits[i]) return true;
       if(this->digits[i] < that->digits[i]) return false;
   }
   return true;
}

bigint *add_bigint (bigint *this, bigint *that) {
   DEBUGS ('b', show_bigint (this));
   DEBUGS ('b', show_bigint (that));
   bigint *result;
   bigint *bigger;
   bigint *smaller;
   if(thisIsBigger(this,that))
      {bigger = this; smaller = that;}
   else{bigger = that; smaller = this;}
   if(bigger->negative == smaller->negative)
      result = do_add(bigger,smaller);
   else result = do_sub(bigger,smaller);
   result->negative = bigger->negative;
   return result;
}

bigint *sub_bigint (bigint *this, bigint *that) {
   DEBUGS ('b', show_bigint (this));
   DEBUGS ('b', show_bigint (that));
   bigint *result;
   bigint *bigger;
   bigint *smaller;
   bool reversed = false;
   if(thisIsBigger(this,that))
      {bigger = this; smaller = that;}
   else{bigger = that; smaller = this; reversed = true;}
   if(bigger->negative == smaller->negative)
      result = do_sub(bigger,smaller);
   else result = do_add(bigger,smaller);
   result->negative = bigger->negative;
   if(reversed) result->negative = !result->negative;
   return result;
}

bigint *mul_bigint (bigint *this, bigint *that) {
   DEBUGS ('b', show_bigint (this));
   DEBUGS ('b', show_bigint (that));
   size_t length = this->size + that->size;
   bigint *result = new_bigint (length > MIN_CAPACITY
                              ? length : MIN_CAPACITY);
   result->size = length;
   for(size_t i=0; i<this->size; i++){
      int c = 0;
      for(size_t j=0; j<that->size; j++){
         int d = result->digits[i+j] +
                 this->digits[i] * that->digits[j] + c;
         result->digits[i+j] = d%10;
         c = d/10;
      }
      result->digits[i+that->size] = c;
   }
   result->negative = this->negative != that->negative;
   trim_zeros(result);
   return result;
}

void show_bigint (bigint *this) {
   fprintf (stderr, "bigint@%p->{%lu,%lu,%c,%p->", this,
            this->capacity, this->size, this->negative ? '-' : '+',
            this->digits);
   for (char *byte = &this->digits[this->size - 1];
        byte >= this->digits; --byte) {
      fprintf (stderr, "%d", *byte);
   }
   fprintf (stderr, "}\n");
}

