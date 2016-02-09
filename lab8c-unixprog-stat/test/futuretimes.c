// $Id: futuretimes.c,v 1.41 2013-05-17 17:42:18-07 - - $

#include <limits.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SEC_PER_DAY (24 * 60 * 60)
#define SEC_PER_YEAR (SEC_PER_DAY * 365 + SEC_PER_DAY / 4)
const time_t localtime_segfault = (time_t) INT_MAX * SEC_PER_YEAR;

int main (void) {
   setlocale (LC_NUMERIC, "en_US");
   printf ("sizeof (time_t) = %ld\n", sizeof (time_t));
   for (time_t when = 1; when > 0; when = when * 2 + 1) {
      if (when > localtime_segfault) break;
      char *timeformat = "%b %d %a %H:%M:%S %Z";
      char buffer[256];
      struct tm *tm = localtime (&when);
      strftime (buffer, sizeof buffer, timeformat, tm);
      printf ("%'25ld = %'15d %s\n", when, tm->tm_year + 1900, buffer);
   }
   printf ("%'25ld = %'15d = INT_MAX (localtime_segfault)\n",
           localtime_segfault, INT_MAX);
   printf ("%'25ld = %'15ld = LONG_MAX\n", LONG_MAX,
           LONG_MAX / SEC_PER_YEAR);
   return EXIT_SUCCESS;
}

//TEST// ./futuretimes >futuretimes.lis
//TEST// mkpspdf futuretimes.ps futuretimes.c* futuretimes.lis

