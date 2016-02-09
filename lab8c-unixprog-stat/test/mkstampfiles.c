// $Id: mkstampfiles.c,v 1.12 2013-05-17 17:53:22-07 - - $

//
// Open several files and write into them.  Then use utime(2)
// to force a time stamp onto them.

#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <utime.h>

char *execname = NULL;
int exit_status = EXIT_SUCCESS;

void maketimefile (time_t when) {
   char filename[64];
   sprintf (filename, "timestamp.%016lX", when);
   FILE *file = fopen (filename, "w");
   if (file == NULL) {
      fprintf (stderr, "%s: %s: %s\n",
               execname, filename, strerror (errno));
      exit_status = EXIT_FAILURE;
      return;
   }
   printf ("fopen (%s): OK\n", filename);
   char buffer[64];
   strftime (buffer, sizeof buffer, "%c %Z", localtime (&when));
   fprintf (file, "%s\n", buffer);
   strftime (buffer, sizeof buffer, "%c %Z", gmtime (&when));
   fprintf (file, "%s\n", buffer);
   fclose (file);
   struct utimbuf utimbuf;
   utimbuf.actime = when;
   utimbuf.modtime = when;
   utime (filename, &utimbuf);
}

int main (int argc, char **argv) {
   (void) argc; // warning: unused parameter 'argc'
   execname = basename (argv[0]);
   time_t now = time (NULL);
   const time_t DAYS = 24 * 60 * 60;
   maketimefile (-0x80000000L);
   maketimefile (0);
   maketimefile (now - 200 * DAYS);
   maketimefile (now);
   maketimefile (now + 200 * DAYS);
   maketimefile (0x7FFFFFFF);
   maketimefile (0xFFFFFFFFL);
   maketimefile (0xFFFFFFFFFFL);
   return exit_status;
}

//TEST// rm ./timestamp.* >mkstampfiles.lis 2>&1
//TEST// ./mkstampfiles >>mkstampfiles.lis 2>&1
//TEST// ls -goatr timestamp.* >>mkstampfiles.lis 2>&1
//TEST// grep . timestamp.* >>mkstampfiles.lis 2>&1
//TEST// l8stat.perl *.perl *.c timestamp.* >>mkstampfiles.lis 2>&1
//TEST// mkpspdf mkstampfiles.ps mkstampfiles.c* *.perl mkstampfiles.lis

