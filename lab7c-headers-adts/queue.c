// Brian Lin bjlin
// $Id: queue.c,v 1.9 2013-05-01 13:22:44-07 - - $

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

#define STUBPRINTF(...) fprintf (stderr, __VA_ARGS__);

typedef struct queue_node queue_node;
struct queue_node {
   queue_item_t item;
   queue_node *link;
};

struct queue {
   queue_node *front;
   queue_node *rear;
};

queue *new_queue (void) {
   queue *newQueue = malloc(sizeof(struct queue));
   assert(newQueue!= NULL);
   newQueue->front = NULL;
   newQueue->rear = NULL;
   return newQueue;
}

void free_queue (queue *this) {
   assert (isempty_queue (this));
   free (this);
}

void insert_queue (queue *this, queue_item_t item) {
   queue_node *newNode = malloc(sizeof(struct queue_node));
   assert(newNode!=NULL);
   newNode->item = item;
   newNode->link = NULL; 
   if(this->front == NULL) {
      this->front = newNode;
      this->rear = newNode;
   }else{
      this->rear->link = newNode;
      this->rear = newNode;
   }
}

queue_item_t remove_queue (queue *this) {
   assert (! isempty_queue (this));
   queue_node *tmp = this->front;
   this->front = tmp->link;
   queue_item_t nodeItem = tmp->item;
   free(tmp);
   return nodeItem;
}

bool isempty_queue (queue *this) {
   return this->front == NULL;
}

