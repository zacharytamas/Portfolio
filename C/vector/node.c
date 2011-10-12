/**
 * Implements the functions of NodeT.
 *
 * @author T. Reichherzer
 */

#include "node.h"
#include <stdlib.h>

NodeT* newNode(NodeItemT item)
{
	NodeT* node;
   
	node = (NodeT*) malloc (sizeof(NodeT));
	node->info = item;
   
	return node;
}

NodeItemT getInfo(NodeT *node)
{
	if (node != NULL)
		return node->info;
   
	return EMPTY_ITEM;
}

void setInfo(NodeT *node, NodeItemT item)
{
	if (node != NULL) 
		node->info = item;
}

NodeT* getNextNode(NodeT* node)
{
	if (node != NULL)
		return node->next;
   
	return NULL;
}

void setNextNode(NodeT* node, NodeT* nextNode)
{
	if (node != NULL)
		node->next = nextNode;
}

void freeNode(NodeT *node)
{
	if (node != NULL)
		free(node);
}