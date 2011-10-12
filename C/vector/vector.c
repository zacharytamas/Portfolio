/*
 *  COP 3530, University of West Florida
 *  Project 3 - vector.c
 *
 *  @author Zachary Jones <zacharytamas@gmail.com>
 *  @releasedate October 11, 2011
 */

#include <stdlib.h>
#include <stdio.h>

#include "node.h"
#include "vector.h"

VectorT* newVector(void)
{
   VectorT* vector = (VectorT*) malloc(sizeof(VectorT));
   vector->tail = NULL;
   vector->anchor = NULL;
   return vector;
}

void addElement(VectorT *vector, NodeItemT item)
{
   NodeT* nodeToAdd = newNode(item);
   if (vector->anchor == NULL) {
      vector->anchor = nodeToAdd;
      vector->tail = nodeToAdd;
   } else {
      NodeT* lastElement = vector->tail;
      setNextNode(lastElement, nodeToAdd);
      vector->tail = nodeToAdd;
   }
}

NodeItemT* get(const VectorT *vector, int index)
{
   if (index == 0) 
      return &(vector->anchor->info);
   
   NodeT* node = traverseToAndReturn((VectorT*) vector, index);
   return (node == NULL) ? NULL : &(node->info);
}

int set(VectorT *vector, NodeItemT item, int index)
{
   NodeT* node = traverseToAndReturn(vector, index);
   
   if (node == NULL) { // node could not be reached
      return -1;
   }
   setInfo(node, item);
   return index;
}

void removeElement(VectorT *vector, int index)
{
   if (index == 0) { // remove first element, we must touch the anchor
      NodeT* secondNode = getNextNode(vector->anchor);
      free(vector->anchor);
      vector->anchor = secondNode;
   } else {
      // index-1 gets the node BEFORE the node we want to remove.
      NodeT* linkPoint = traverseToAndReturn(vector, index-1); 
      if (linkPoint == NULL) return; // no element exists at this index
      
      NodeT* newLink = getNextNode(linkPoint->next);
      free(linkPoint->next);
      setNextNode(linkPoint, newLink);
   }
}

int size(const VectorT *vector)
{
   if (vector->anchor == NULL) {
      return 0;
   } else {
      return lengthUntilEnd(vector->anchor);
   }
}

int lengthUntilEnd(NodeT* node)
{
   if (node->next == NULL) {
      return 1;
   } else {
      return 1 + lengthUntilEnd(node->next);
   }
}

int contains(const VectorT *vector, NodeItemT item)
{
   if (indexOf(vector, item) == -1) {
      return 0;
   } else {
      return 1;
   }
}

int indexOf(const VectorT *vector, NodeItemT item)
{
   NodeT* node = vector->anchor;
   int index = 0;
   
   while (node != NULL) {
      if (node->info == item) {
         return index;
      }
      ++index;
      node = getNextNode(node);
   }
   
   return -1;
}

void clear(VectorT *vector)
{
   NodeT* node = vector->anchor;
   NodeT* nextNode;
   
   while (node != NULL) {
      nextNode = getNextNode(node);
      free(node);
      node = nextNode;
   }
   vector->anchor = NULL;
   vector->tail = NULL;
}

void deleteVector(VectorT *vector) 
{
   // Deallocate the linked list's items.
   clear(vector);
   
   // Deallocate the vector itself
   free(vector);
}

NodeT* traverseToAndReturn(VectorT* vector, int index)
{
   
   if (index < 0) {
      return vector->tail;
   }
   
   int count = 0;
   NodeT* node = vector->anchor;
   
   while (node != NULL) {
      
      if (count == index) {
         return node;
      }
      node = getNextNode(node);
      ++count;
   }
   
   // The index was never met while traversing through the list. 
   // It's likely that the index given was larger than the 
   // length of the list.
   return NULL;
}

void dumpVector(VectorT* vector) 
{
   int i=0;
   NodeT* node = vector->anchor;
   while (node != NULL) {
      printf("%d:%c ", i, node->info);
      node = node->next;
      ++i;
   }
   if (i == 0)
      printf("Vector was empty.");
   printf("\n");
}
