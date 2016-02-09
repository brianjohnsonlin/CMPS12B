// Brian Lin bjlin
// $Id: linked_stack.java,v 1.1 2013-10-16 12:58:29-07 - - $
//
// NAME
//    class linked_stack - implementation of stack
//

import java.util.NoSuchElementException;

class linked_stack<item_t> {

   private class node {
      item_t value;
      node link;
   }

   private node top = null;

   public boolean empty() {
      return top == null;
   }

   public item_t pop() {
      if (empty())
         throw new NoSuchElementException ("linked_stack.pop");
      node currNode=top;
      item_t item=null;
      if(top.link==null){
         item=top.value;
         top=null;
      }else{
         while(currNode.link.link!=null)currNode=currNode.link;
         item=currNode.link.value;
         currNode.link=null;
      }
      return item;
   }

   public void push (item_t value) {
      node newNode = new node();
      newNode.value = value;
      if(top == null) top = newNode;
      else{
         node currNode=top;
         while(currNode.link!=null)currNode=currNode.link;
         currNode.link = newNode;
      }
   }

}
