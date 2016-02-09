// Brian Lin bjlin
// $Id: bitree.java,v 1.3 2013-10-17 18:34:13-07 - - $
//
// NAME
//    class bitree - starter class for bitree implementation.
//

class bitree {
   char symbol;
   bitree left;
   bitree right;

   bitree (char symbol_, bitree left_, bitree right_) {
      symbol = symbol_;
      left = left_;
      right = right_;
   }

   bitree (char symbol_) {
      this (symbol_, null, null);
   }
   
   // Returns String form of tree in infix notation
   public String toString () {
      if(left==null||right==null)return ""+symbol;
      return "("+left+symbol+right+")";
   }
   
   // Calculates the tree
   public double eval(){
      if(Character.isLetter(symbol)){
         bitree tree = bitreecalc.table.get_tree(symbol);
         if(tree!=null)
            bitreecalc.table.put(symbol, tree.eval(), tree);
         return bitreecalc.table.get_value(symbol);
      }
      else switch(symbol){
         case '+': return left.eval()+right.eval();
         case '-': return left.eval()-right.eval();
         case '*': return left.eval()*right.eval();
         case '/': return left.eval()/right.eval();
         default : auxlib.die("syntax error"); return 0;
      }
   }
}
