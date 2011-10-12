#ifndef _NODE_H
#define _NODE_H

// -----------------------------------------
// Definition of an item stored with a Node.
// -----------------------------------------
typedef char NodeItemT;

#define EMPTY_ITEM '\0';

// ----------------------
// Definition of a Node
// ----------------------
typedef struct NodeT
{
	NodeItemT	 info;
	struct NodeT *next;
} NodeT;

// ----------------------
// Interface to Node ADT
// ----------------------

/**
 * Creates a new node and stores the specified item in it.
 *
 * @param item the item to be stored in the node.
 */
NodeT* newNode(NodeItemT item);

/**
 * Returns the item of a node.
 *
 * @return a new node.
 */
NodeItemT getInfo(NodeT* node);

/**
 * Sets the item in a node.
 *
 * @param node a node whose item must be set
 * @param item the item of the node
 */
void setInfo(NodeT* node, NodeItemT item);

/**
 * Returns the next node linked by this node.
 *
 * @return a node or null if this node does not link to a node.
 */
NodeT* getNextNode(NodeT* node);

/**
 * Sets the next node for a given node.
 * 
 * @param node a node whose next node must be set
 * @param nextNode the next node that his node will point to
 */
void setNextNode(NodeT* node, NodeT* nextNode);

/**
 * Frees up the item of a node.
 *
 * @param node the node to be deallocated from memory
 */
void freeNode(NodeT * node);

#endif