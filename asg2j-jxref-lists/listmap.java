// Brian Lin bjlin
// $Id: listmap.java,v 1.5 2013-10-16 17:10:32-07 - - $

import java.util.Iterator;
import java.util.Map.Entry;
import java.util.NoSuchElementException;
import static java.lang.System.*;

class listmap implements Iterable<Entry<String,intqueue>> {

   private class node implements Entry<String,intqueue> {
      String key;
      intqueue queue = new intqueue();
      node link;
      public String getKey() {
         return key;
      }
      public intqueue getValue() {
         return queue;
      }
      public intqueue setValue (intqueue queue) {
         throw new UnsupportedOperationException();
      }
   }
   private node head = null;

   public listmap() {
      // Not needed, since head defaults to null anyway.
   }

   public void insert (String key, int linenr) {
      node newNode = new node();
      newNode.key = key;
      newNode.queue.insert(linenr);
      
      node currNode=head;
      if(currNode==null){
         head=newNode;
         return;
      }else if(key.compareTo(currNode.key)<0){
         newNode.link = currNode;
         head = newNode;
         return;
      }else if(key.compareTo(currNode.key)==0){
         currNode.queue.insert(linenr);
         return;
      }

      int foundValue=0; //0:!found,1:match,2:normal,3:list end
      while(foundValue==0){
         if(currNode.link==null) foundValue=3;
         else if(key.compareTo(currNode.link.key)<0)
            foundValue=2;
         else if(key.compareTo(currNode.link.key)==0)
            foundValue=1;
         else currNode=currNode.link;
      }
      if(foundValue==1){
         currNode.link.queue.insert(linenr);
      }else if(foundValue==2){
         newNode.link = currNode.link;
         currNode.link = newNode;
      }else if(foundValue==3){
         currNode.link = newNode;
      }
   }

   public Iterator<Entry<String,intqueue>> iterator() {
      return new iterator();
   }


   private class iterator
           implements Iterator<Entry<String,intqueue>> {
      node curr = head;

      public boolean hasNext() {
         return curr != null;
      }

      public Entry<String,intqueue> next() {
         if (curr == null) throw new NoSuchElementException();
         node next = curr;
         curr = curr.link;
         return next;
      }

      public void remove() {
         throw new UnsupportedOperationException();
      }

   }

}
