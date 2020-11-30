#ifndef NODE_H
#define NODE_H

#include "priority_queue.h"

typedef struct Node_t *Node;

Node createNode(PQElement element, PQElementPriority priority, PriorityQueue queue);
void destroyNode(Node node, PriorityQueue queue);

#endif