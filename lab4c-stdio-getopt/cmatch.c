// Brian Lin bjlin

#define _GNU_SOURCE
#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

char *program_name = NULL;
int exit_status = EXIT_SUCCESS;
char* str;

//copied straight from getoptx.c
typedef struct options {
   bool ignore_case;
   bool filenames_only;
   bool number_lines;
} options;
void scan_options (int argc, char **argv, options *opts){
   opts->ignore_case=false;
   opts->filenames_only=false;
   opts->number_lines=false;
   opterr=false;
   for(;;){
      int opt = getopt (argc, argv, "iln");
      if(opt == EOF) break;
      switch (opt) {
         case 'i':
            opts->ignore_case = true;
            break;
         case 'l':
            opts->filenames_only = true;
            break;
         case 'n':
            opts->number_lines = true;
            break;
         default:
            exit_status = EXIT_FAILURE;
            fflush (NULL);
            fprintf (stderr, "%s: -%c: invalid option\n",
                             program_name, optopt);
            break;
      }
   }
}

void catbyline(FILE *input, char *filename, options opts, int onlyFile){
   char buffer[1024];
   for (int linenr = 1;; linenr++) { //for every line
      //reads in next line
      char *buff = fgets(buffer, sizeof buffer, input);
      if(buff == NULL) break;
      
      //looks for string in buffer
      int found=0;
      if(opts.ignore_case){if(strcasestr(buff, str) != NULL) found = 1;}
      else{if(strstr(buff, str) != NULL) found = 1;}
      
      if(found==1){ //if found
         if(opts.filenames_only){
            printf("%s\n", filename);
            break;
         }else{
            if(onlyFile==0) printf("%s:", filename);
            if(opts.number_lines) printf("%d:", linenr);
            printf("%s", buff);
         }
      }
   }
}

void printLines(int argc, char **argv, options opts){
   for(int argi = 2; argi < argc; argi++){
      char *filename = argv[argi];
      if(argi==2 && strstr(argv[1], "-") != NULL) continue;
      if(strcmp(filename, "-") == 0){
          catbyline (stdin, "-", opts, 0);
          continue;
      }
      
      FILE *input = fopen(filename, "r");
      int onlyFile = 0;
      if(input != NULL){
         if(argc==3 || (argc==4 && strstr(argv[1], "-") != NULL))
            onlyFile=1;
         catbyline(input, filename, opts, onlyFile);
         fclose(input);
      }else{
         exit_status = EXIT_FAILURE;
         fflush(NULL);
         fprintf(stderr, "%s: %s: %s\n", program_name,
                 filename, strerror(errno));
         fflush(NULL);
      }
   }
}

int main (int argc, char **argv) {
   program_name = basename(argv[0]);
   if (argc==1 || argc==2){ //if there are only one or two args
      printf("Usage: cmatch [-iln] string [filename...]\n");
      return EXIT_SUCCESS;
   }
   options opts;
   scan_options(argc, argv, &opts);
   if(strstr(argv[1],"-") == NULL) str = argv[1];
   else str = argv[2];
   printLines(argc,argv,opts);
}
