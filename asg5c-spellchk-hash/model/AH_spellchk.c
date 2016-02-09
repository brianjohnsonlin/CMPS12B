// $Id: spellchk.c,v 1.6 2013-05-21 19:58:24-07 - - $

#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include "debugf.h"
#include "hashset.h"
#include "yyextern.h"

#define STDIN_NAME       "-"
#define DEFAULT_DICTNAME \
        "/afs/cats.ucsc.edu/courses/cmps012b-wm/usr/dict/words"
#define DEFAULT_DICT_POS 0
#define EXTRA_DICT_POS   1
#define NUMBER_DICTS     2

void print_error (char *object, char *message) {
   fflush (NULL);
   fprintf (stderr, "%s: %s: %s\n", Exec_Name, object, message);
   fflush (NULL);
   //exit status for spelling error
   Exit_Status = 1;
}

FILE *open_infile (char *filename) {
   FILE *file = fopen (filename, "r");
   if (file == NULL) print_error (filename, strerror (errno));
   DEBUGF ('m', "filename = \"%s\", file = 0x%p\n", filename, file);
   return file;
}

void spellcheck (char *filename, hashset *hashset) {
   yylineno = 1;
   DEBUGF ('m', "filename = \"%s\", hashset = 0x%p\n",
                filename, hashset);
   for (;;) {
      int token = yylex ();
      if (token == 0) break;
      DEBUGF ('m', "line %d, yytext = \"%s\"\n", yylineno, yytext);
     // STUBPRINTF ("%s: %d: %s\n", filename, yylineno, yytext);
     if(has_hashset(hashset,yytext)) continue;
     else{
       printf("%s: %d: %s\n", filename, yylineno, yytext);
     if(Exit_Status == 0) Exit_Status++;
     }
   }
}
       
void load_dictionary (char *dictionary_name, hashset *hashset) {

   if (dictionary_name == NULL) return;
   DEBUGF ('m', "dictionary_name = \"%s\", hashset = %p\n",
           dictionary_name, hashset);
  // STUBPRINTF ("Open dictionary, load it, close it\n");
  //here's where we open the dictionary, inspired by previous lab
  char buffer[1024];
  FILE *dictionary = open_infile(dictionary_name);
  //check to see if the dictionary exist in directory
  if(dictionary == NULL){
    print_error(dictionary_name, "No such file or directory");
    return;
  }
  //at this point, dictionary is not null
  assert(dictionary != NULL);
  int counter = 0;
  // inputting the words 

  for(int lineCount = 1; ; ++lineCount){
    char *linePosition = fgets(buffer,sizeof buffer, dictionary);
  //printf("pass %d\n", lineCount);
  if(linePosition == NULL) break;// nothign was able to read
  //chomp off trailing newline character
  linePosition = strchr(buffer, '\n');
  if(linePosition == NULL) {
    fflush(NULL);
   fprintf(stderr, "%s: %s[%d]: unterminated line\n", 
            Exec_Name,dictionary_name,lineCount);
    fflush(NULL);
    //exit status for loading dictionary
    Exit_Status = 2;    
  } else{
   *linePosition = '\0';
   }
  linePosition = strdup(buffer);
  assert(linePosition != NULL);
  put_hashset(hashset, linePosition);
  //the reason why i stop at pass 16 is because i didn't
  //double the array in put_hashset **should fix
  free(linePosition);
  counter++;
  //printf("%d",counter);
  //now the words have been inputted
  }
 //printf("pass a\n");
 fclose(dictionary);
//remember to close the dictionary afterwards 
//printf("The dictionary has been loaded!!!.\n");
//printf("Here's how many words have been added: %d\n", counter);
}


int main (int argc, char **argv) {
   Exec_Name = basename (argv[0]);
   char *default_dictionary = DEFAULT_DICTNAME;
   char *user_dictionary = NULL;
   hashset *hashset = new_hashset ();
   yy_flex_debug = false;

   int hash_dump = 0; //for implementation 5(vii, viii)

   // Scan the arguments and set flags.
   opterr = false;
   for (;;) {
      int option = getopt (argc, argv, "nxyd:@:");
      //printf("The option is: %d\n", option); 
      if (option == EOF) break;
      switch (option) {
         char optopt_string[16]; // used in default:
         case 'd': user_dictionary = optarg;
                   break;
         case 'n': default_dictionary = NULL;
                   break;
         case 'x': hash_dump++; //STUBPRINTF ("-x\n");
                   break;
         case 'y': yy_flex_debug = true;
                   break;
         case '@': set_debugflags (optarg);
                   if (strpbrk (optarg, "@y")) yy_flex_debug = true;
                   break;
         default : sprintf (optopt_string, "-%c", optopt);
                   print_error (optopt_string, "invalid option");
                   break;
      }
   }

   // Load the dictionaries into the hash table.
   load_dictionary (default_dictionary, hashset);
   load_dictionary (user_dictionary, hashset);

  if(hash_dump >1) { //if -x flag comes up more than once
    hash_check(hashset, hash_dump);
   yylex_destroy();
    //free_hashset(hashset);
    return Exit_Status;
  }

   // Read and do spell checking on each of the files.
   if (optind >= argc) {
      yyin = stdin;
      spellcheck (STDIN_NAME, hashset);
   }else {
      for (int fileix = optind; fileix < argc; ++fileix) {
         DEBUGF ('m', "argv[%d] = \"%s\"\n", fileix, argv[fileix]);
         char *filename = argv[fileix];
         if (strcmp (filename, STDIN_NAME) == 0) {
            yyin = stdin;
            spellcheck (STDIN_NAME, hashset);
         }else {
            yyin = open_infile (filename);
            if (yyin == NULL) continue;
            spellcheck (filename, hashset);
            fclose (yyin);
         }
      }
   }
   free_hashset(hashset);
   yylex_destroy ();
   return Exit_Status;
}

