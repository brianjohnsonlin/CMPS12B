// Brian Lin bjlin
// $Id: bitreecalc.java,v 1.1 2013-10-16 12:58:29-07 - - $

import java.io.*;
import java.util.Scanner;
import static java.lang.System.*;

class bitreecalc {
   private static final String STDIN_FILENAME = "-";
   public static symbol_table table = new symbol_table();
   private static boolean echo = false;
   private static FileWriter writer = null;
   
   // Main function scans arguments and options to read files.
   public static void main (String[] args) {
      if (args.length == 0)bitcalc_filename(STDIN_FILENAME);
      else{
         for(int i=0;i<args.length;i++)
            if(args[i].charAt(0)=='-'&&args[i].length()>1){
               if(args[i].equals("-e")) echo=true;
               else if(args[i].equals("-o")){
                  if(i+1!=args.length){
                     try{writer = new FileWriter(args[i+1]);}
                     catch(IOException e){
                        auxlib.die(e.getMessage());
                     }
                     args[i+1]="-a";
                     i++;
                  }else auxlib.die("no output filename found");
               }else auxlib.die("improper option usage");
            }
         boolean check = false; //checks if next "if" ran
         for (String arg: args){
            if(!(arg.charAt(0)=='-'&&arg.length()>1)){
               bitcalc_filename(arg);
               check = true;
            }
         }
         if(!check) bitcalc_filename(STDIN_FILENAME);
      }
      print("**** FINAL SYMBOL TABLE ****");
      for(char i='a';i<='z';i++)
         if(!Double.isNaN(table.get_value(i))) prtSymbol(i);
      if(writer!=null)
         try{writer.close();}
         catch(IOException e){auxlib.die(e.getMessage());}
      exit(auxlib.exitvalue);
   }
   
   // For each filename, open the file, read it, and print.
   private static void bitcalc_filename (String filename) {
      if (filename.equals (STDIN_FILENAME)) {
         bitcalc_file (new Scanner (in));
      }else {
         try {
            Scanner file = new Scanner (new File (filename));
            bitcalc_file (file);
            file.close();
         }catch (IOException error) {
            auxlib.warn(error.getMessage());
         }
      }
   }
   
   // For each line in the file, scan for a variable
   // and an operator and handle whichever case.
   private static void bitcalc_file(Scanner file){
      while(file.hasNextLine()){
         String line = file.nextLine();
         if(echo)print("**** "+line);
         line=line.trim();
         if(line.isEmpty())continue;
         if(line.charAt(0)=='#')continue;
         String[] lineArray = line.split("");
         char variable = nextChar(lineArray);
         if(!Character.isLetter(variable)){
            auxlib.warn("syntax error");
            continue;
         }
         char operator = nextChar(lineArray);
         switch(operator){
            case '=': compute(variable,lineArray); break;
            case ':': set(variable,lineArray); break;
            case '?': prtSymbol(variable); break;
            default : auxlib.warn("syntax error"); break;
         }
      }
   }
   
   // Function to find next non-whitespace character in a line.
   private static char nextChar(String[] lineArray){
      for(int i = 0; i < lineArray.length; i++){
         if(lineArray[i].isEmpty())continue;
         if(Character.isWhitespace(lineArray[i].charAt(0))){
            lineArray[i]="";
            continue;
         }
         char cha = lineArray[i].charAt(0);
         lineArray[i]="";
         return cha;
      }
      return '\u0000'; //end of line value
   }
   
   // Sets a tree to a variable and computes the value.
   private static void compute(char variable,String[] lineArray){
      linked_stack<bitree> stack = new linked_stack<bitree>();
      int status=0; //0:running 1:cleanexit 2:errorexit
      while(status==0){
         char value = nextChar(lineArray);
         if(Character.isLetter(value))
            stack.push(new bitree(value));
         else switch(value){
            case '+': case '-': case '*': case '/':
               try{
                  bitree right = stack.pop();
                  bitree left  = stack.pop();
                  stack.push(new bitree(value,left,right));
               }catch(java.util.NoSuchElementException e){
                  auxlib.warn("stack underflow");
                  status=2;
               } break;
            case '\u0000':
               status = 1;
               break;
            default :
               auxlib.warn("syntax error");
               status = 2;
               break;
         }
      }
      if(status==1){
         bitree tree = stack.pop();
        if(!stack.empty())
           auxlib.warn("leftover stack");
         else{
            table.put(variable, tree.eval(), tree);
            prtSymbol(variable);
         }
      }
   }
   
   // Sets a double value to a variable.
   private static void set(char variable, String[] lineArray){
      String number="";
      for(String cha : lineArray)number+=cha;
      number = number.trim();
      try{
         table.put(variable,Double.parseDouble(number),null);
         prtSymbol(variable);
      }catch(NumberFormatException error){
         auxlib.warn("number format error: "+error.getMessage());
      }
   }
   
   // Print the properties of a variable.
   private static void prtSymbol(char variable){
      bitree tree = table.get_tree(variable);
      if(tree!=null) table.put(variable, tree.eval(), tree);
      print(variable + ": " + table.get_value(variable));
      print("   " + table.get_tree(variable));
   }
   
   // Print function to direct printing to appropriate place
   private static void print(String line){
      if(writer == null)out.println(line);
      else try{writer.write(line+"\n");}
           catch(IOException e){auxlib.die(e.getMessage());}
   }
}
