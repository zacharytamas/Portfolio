/*
 *  COP 3530, University of West Florida
 *  Project 3 - vector.h
 *
 *  @author Zachary Jones <zacharytamas@gmail.com>
 *  @releasedate October 11, 2011
 */

#ifndef _VECTOR_H
#define _VECTOR_H

#include "node.h"

// ----------------------
// Definition of a Vector
// ----------------------
typedef struct VectorT
{
	NodeT* anchor;
   NodeT* tail;
} VectorT;

// ----------------------
// Interface to Vector ADT
// ----------------------

/**
 * Allocates a new vector and returns a pointer to it.
 */
VectorT* newVector(void);

/**
 * Appends a new element to the end of the vector's list.
 *
 * @param vector the vector to be modified
 * @param item   the item to be appended to the vector
 */
void addElement(VectorT *vector, NodeItemT item);


/**
 * Returns an element from the vector given a location.
 *
 * @param vector the vector to be accessed for an element
 * @param index  the location of the element to be returned
 *
 * @return the item in the vector at the location or NULL 
 *   if the location is out of the valid range of the vector.
 */
NodeItemT* get(const VectorT *vector, int index);

/**
 * Sets an item in a vector.
 *
 * @param vector the vector to be modified
 * @param item   the item to be set
 * @param index  the location where the item needs to be set
 *
 * @return the location where the item was set or -1, 
 *   if the item was not set because the location is
 *   out of the vector's range
 */
int set(VectorT *vector, NodeItemT item, int index);

/**
 * Removes an item in a vector at the specified location, 
 * adjusting the linked list structure appropriately.
 *
 * @param vector the vector to be modified
 * @param index  the location where the item needs to be removed
 */
void removeElement(VectorT *vector, int index);

/**
 * Returns the number of elements in the vector.
 *
 * @param vector the vector whose size is to be determined
 * 
 * @return the total number of elements stored in the vector
 */
int size(const VectorT *vector);

/**
 * Tests if the vector contains a specified element.
 *
 * @param vector the vector to be tested for containment of a 
 *   specific element
 * @param item   the item to be tested
 *
 * @return 1 if the vector contains the element; 0 otherwise
 */
int contains(const VectorT *vector, NodeItemT item);

/**
 * Returns the index for a specified item in the vector.
 *
 * @param vector the vector to be checked for an element
 * @param item   the item whose location in the vector 
 *   needs to be checked
 *
 * @return the index of the location of the item in the 
 *   vector or -1 if the item cannot be found
 */
int indexOf(const VectorT *vector, NodeItemT item);

/**
 * Removes all elements from the specified vector.
 *
 * @param vector the vector to be cleared
 */
void clear(VectorT *vector);

/**
 * Deletes the vector. Memory for storing elements will 
 * be deallocated.
 *
 * @param vector the vector to be deleted
 */
void deleteVector(VectorT *vector);

/**
 * Traverses to a given index and returns that node. If given index 
 * is negative it will return a pointer to the vector's tail.
 */
NodeT* traverseToAndReturn(VectorT* vector, int index);

/**
 * Returns the distance from the end of the linked list 
 * to the given node.
 */
int lengthUntilEnd(NodeT* node);

/**
 * For verification purposes, dumps a vector's linked list to 
 * standard out with index numbers and each corresponding 
 * node's NodeItemT.
 */
void dumpVector(VectorT* vector);

#endif
