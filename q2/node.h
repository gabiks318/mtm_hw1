#ifndef NODE_H
#define NODE_H

#include "priority_queue.h"

typedef struct Node_t *Node;


Node nodeCreate(PQElement element,
                PQElementPriority priority,
                CopyPQElement copy_element,
                FreePQElement free_element,
                CopyPQElementPriority copy_priority,
                FreePQElementPriority free_priority
                );

void nodeDestroy(Node node,
                 FreePQElement free_element,
                 FreePQElementPriority free_priority
                 );

PQElement nodeGetElement(Node node);

PQElementPriority nodeGetElementPriority(Node node);

void nodeSetNext(Node node,Node next_node);

Node nodeGetNext(Node node);

#endif