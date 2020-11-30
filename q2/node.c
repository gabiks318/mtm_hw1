#include <stdlib.h>

#include "node.h"

struct Node_t
{
    PQElement pq_element;
    PQElementPriority pq_element_priority;
    Node next;
};


Node createNode(PQElement element, PQElementPriority priority, PriorityQueue queue)
{

    Node ptr = malloc(sizeof(*ptr));
    if (!ptr)
    {
        return NULL;
    }

    ptr->pq_element = queue->copy_element(element);
    if (element == NULL)
    {
        free(ptr);
        return NULL;
    }
    ptr->pq_element_priority = queue->copy_priority(priority);
    if (ptr->pq_element_priority == NULL)
    {
        queue->free_element(element);
        free(ptr);
        return NULL;
    }
    ptr->next = NULL;
    return ptr;
}

void destroyNode(Node node, PriorityQueue queue)
{
    if (node == NULL)
    {
        return;
    }

    queue->free_element(node->pq_element);
    queue->free_priority(node->pq_element_priority);
    free(node);
}