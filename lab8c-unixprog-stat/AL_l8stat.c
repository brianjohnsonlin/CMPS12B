//Andrew Lien
//ahlien 
//l8stat.c
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <errno.h>

char *progname = NULL;
int exit_status = EXIT_SUCCESS;

void l8stat(char *fileInput) {
  struct stat buffer;
  int status = lstat(fileInput, &buffer);
  ///////// If l8stat succeeds, i.e it reads a successfull ///////// 
  ////////          file, exit status success//////////////////////
  if(status == 0) {
    int size = (int)buffer.st_size; //collect size
    int mode = (int)buffer.st_mode; //collect buffer
    //printf("The size is %d\n The mode is %d\n", size,mode);
    printf("%06o\t    %d ", mode, size); //prints the mode&size,4spaces

    //////////from futuretimes.c////////////////////
    struct tm *tm = localtime(&buffer.st_mtime);
    time_t currentTime;
    time(&currentTime);
    double diff = difftime(buffer.st_mtime, currentTime);
    // printf("The difference is %d",diff);
    char *format = NULL;
    
    if(diff<0) diff = -diff;
    if(diff>15552000) format = "%b %d %y";
    else format = "%b %d %X";
      

   //////////from mkstampfiles.c////////
   char timeBuff[1024];
   strftime(timeBuff, sizeof(timeBuff), format, tm);
   printf("%s %s", timeBuff, fileInput);

  ////////from readlink & showlink.c//////
   char *linkN = NULL;
   linkN = malloc(buffer.st_size + 1); //malloc here, free later
   assert(linkN != NULL);
   int lFlag = readlink(fileInput, linkN, buffer.st_size+1);

   if(lFlag != -1) {
     linkN[buffer.st_size] = '\0';
     printf(" --> %s", linkN);
   }
   printf("\n");
   //free linkN
   free(linkN);
////// used else example from showlink.c ////////////////////////////
///// condition if there are no files correctly specified in dir/////
  } else{
      //printf("stat fail\n");
      fflush (NULL);
      fprintf (stderr, "%s: %s: %s\n", progname,
               fileInput, strerror (errno));
      fflush (NULL);
      exit_status = EXIT_FAILURE;

    }
}

int main(int argc, char **argv){
  exit_status = EXIT_SUCCESS;
  progname = basename(argv[0]);
  if(argc == 1){
   l8stat("."); //emulate perl program, send in '.'
  } else{//all other cases 
     for(int i = 1; i < argc; i++){
        char *filename = argv[i];
        l8stat(filename);
    }
  } 
}
