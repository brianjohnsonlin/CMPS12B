// Brian Lin bjlin
// $Id: jfmt.java,v 1.2 2013-09-24 14:38:16-07 - - $

import java.io.*;
import java.util.LinkedList;
import java.util.List;
import java.util.Scanner;
import static java.lang.System.*;

class jfmt {
    // Static variables keeping the general status of the program.
    public static final String JAR_NAME = get_jarname();
    public static final int EXIT_SUCCESS = 0;
    public static final int EXIT_FAILURE = 1;
    public static int exit_status = EXIT_SUCCESS;
    public static int lineMax = 65;

    // A basename is the final component of a pathname.
    // If a java program is run from a jar, the classpath is the
    // pathname of the jar.
    static String get_jarname() {
        String jarpath = getProperty ("java.class.path");
        int lastslash = jarpath.lastIndexOf ('/');
        if (lastslash < 0) return jarpath;
        return jarpath.substring (lastslash + 1);
    }


    // Formats a single file.
    static void format (Scanner infile) {
        
        // Create a LinkedList of Strings.
        List<String> words = new LinkedList<String>();
        
        // Read each line from the opened file, one after the other.
        // Stop the loop at end of file.
        for (int linenr = 1; infile.hasNextLine(); ++linenr) {
            String line = infile.nextLine();
            // Split the line into words around white space and iterate
            // over the words.
            for (String word: line.split ("\\s+")) {
                // Prints paragraph if an empty word is found.
                if (word.length() == 0) print_paragraph(words);
                //out.printf ("...[%s]%n", word);
                // Append the word to the end of the linked list.
                words.add (word);
            }
            //End of file: print paragraph
            print_paragraph(words);
        }
    }
    
    static void print_paragraph(List<String> words){
        int charInLine=0; //number of characters in current line
        if (words.isEmpty()) return; //skip if empty
        for (String word: words){
            //if adding the new word (and a space) exceeds lineMax, 
            //go to next line and reset the count
            if(charInLine+word.length()+1>lineMax){
                out.println();
                charInLine=0;
            }
            
            //if count is NOT 0, put a space and add one to
            //the word count
            if(charInLine!=0){
                out.print(" ");
                charInLine++;
            }
            
            //prints the word and adds the length to the word count
            out.print(word);
            charInLine+=word.length();
        }
        out.println();
        words.clear();
    }


    // Main function scans arguments and opens/closes files.
    public static void main (String[] args) {
        if (args.length == 0) {
            // There are no filenames given on the command line.
            out.printf ("FILE: -%n");
            format (new Scanner (in));
        }else {
            //checks if first arg is a number
            //that can be used as lineMax
            int argix = 0;
            if (args[0].matches("-\\d+")){
                lineMax = Integer.parseInt(args[0]) *-1;
                argix++;
            }       
            // Iterate over each filename given on the command line.
            for (; argix < args.length; ++argix) {
                String filename = args[argix];
                if (filename.equals ("-")) {
                    // Treat a filename of "-" to mean System.in.
                    out.printf ("FILE: -%n");
                    format (new Scanner (in));
                }else {
                    // Open the file and read it, or error out.
                    try {
                        Scanner infile =new Scanner(new File(filename));
                        out.printf ("FILE: %s%n", filename);
                        format (infile);
                        infile.close();
                    }catch (IOException error) {
                        exit_status = EXIT_FAILURE;
                        err.printf ("%s: %s%n", JAR_NAME,
                                        error.getMessage());
                    }
                }
            }
        }
        exit (exit_status);
    }

}
