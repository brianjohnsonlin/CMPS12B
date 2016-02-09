// Brian Lin bjlin
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>
#include <libgen.h>

char *execname = NULL;
int EXIT_STATUS = EXIT_SUCCESS;

void l8stat(char* name){
   struct stat stat;
   char timebuff[1024];
   int status = lstat(name, &stat);  //put lsat struct stat into buf
   if(status != 0){
      fprintf(stderr, "%s: %s: %s\n", execname,name,strerror(errno));
      fflush(NULL);
      EXIT_STATUS = EXIT_FAILURE;
      return;
   }
   struct tm *tm = localtime(&stat.st_mtime);   //localtime of file
   if(abs(difftime(stat.st_mtime, time(NULL))) > 15552000)
      strftime(timebuff, 1024, "%b %e  %Y", tm);
   else strftime(timebuff, 1024, "%b %e %R", tm);
   printf("%06o%10ld %s %s",stat.st_mode,stat.st_size,timebuff,name);
   char *linkName = malloc(sizeof(stat));
   int readlink_status =
      readlink(name, linkName, sizeof(stat));
   if(readlink_status != -1){
      linkName[stat.st_size] = '\0';
      printf(" -> %s", linkName);
   }
   free(linkName);
   printf("\n");
}

int main(int argc, char** argv){
   execname = basename (argv[0]);
   if(argc<1) l8stat(".");
   else for(int argi=1; argi<argc; argi++) l8stat(argv[argi]);
   return EXIT_STATUS;
}
