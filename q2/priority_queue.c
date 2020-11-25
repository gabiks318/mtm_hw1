#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "priority_queue.h"

#define PRINT false

// Struct Declarations

typedef struct node
{
    PQElement pq_element;
    PQElementPriority pq_element_priority;
    struct node *next;
} * Node;

struct PriorityQueue_t
{
    CopyPQElement copy_element;
    FreePQElement free_element;
    EqualPQElements equal_elements;
    CopyPQElementPriority copy_priority;
    FreePQElementPriority free_priority;
    ComparePQElementPriorities compare_priority;
    Node list;
    Node iterator;
    bool iterator_defined;
};

//function declaration
static Node createNode(PQElement element, PQElementPriority priority, PriorityQueue queue);
static void destroyNode(Node node, PriorityQueue queue);

//creates a new priority queue
PriorityQueue pqCreate(CopyPQElement copy_element, FreePQElement free_element,
                       EqualPQElements equal_elements, CopyPQElementPriority copy_priority,
                       FreePQElementPriority free_priority, ComparePQElementPriorities compare_priority)
{
    if (!copy_element || !free_element || !equal_elements || !copy_priority || !free_priority || !compare_priority)
    {
        return NULL;
    }

    PriorityQueue priority_queue = malloc(sizeof(*priority_queue));
    if (priority_queue == NULL)
        return NULL;

    priority_queue->copy_priority = copy_priority;
    priority_queue->free_priority = free_priority;
    priority_queue->compare_priority = compare_priority;
    priority_queue->copy_element = copy_element;
    priority_queue->free_element = free_element;
    priority_queue->equal_elements = equal_elements;
    priority_queue->list = NULL;
    priority_queue->iterator = NULL;
    priority_queue->iterator_defined = true;

    return priority_queue;
}

void pqDestroy(PriorityQueue queue)
{
    if (queue == NULL)
    {
        return;
    }
    pqClear(queue);
    free(queue);
}

PriorityQueue pqCopy(PriorityQueue queue)
{
    if (queue == NULL)
    {
        return NULL;
    }
    PriorityQueue new_queue = pqCreate(queue->copy_element, queue->free_element,
                                       queue->equal_elements, queue->copy_priority, queue->free_priority, queue->compare_priority);

    if (new_queue == NULL)
    {
        return NULL;
    }
    pqGetFirst(queue);
    pqGetFirst(new_queue);

    while (queue->iterator)
    {
        pqInsert(new_queue, queue->iterator->pq_element, queue->iterator->pq_element);
        pqGetNext(queue);
    }

    queue->iterator_defined = false;
    new_queue->iterator_defined = false;

    return new_queue;
}

int pqGetSize(PriorityQueue queue)
{
    if (PRINT)
    {
        printf("Getting size\n");
    }

    if (queue == NULL)
    {
        return -1;
    }

    int size = 0;
    if (pqGetFirst(queue) == NULL)
        return size;

    while(queue->iterator != NULL)
    {

        size++;
        
        pqGetNext(queue);
    }
    if (PRINT)
    {
        printf("ending count \n");
    }

    return size;
}

bool pqContains(PriorityQueue queue, PQElement element)
{
    if (queue == NULL || element == NULL)
    {
        return false;
    }
    pqGetFirst(queue);
    while (queue->iterator)
    {
        if (queue->equal_elements(element, queue->iterator->pq_element))
        {
            return true;
        }
        pqGetNext(queue);
    }
    return true;
}

PriorityQueueResult pqInsert(PriorityQueue queue, PQElement element, PQElementPriority priority)
{
    if (queue == NULL || element == NULL || priority == NULL)
    {
        return PQ_NULL_ARGUMENT;
    }
    if (PRINT)
    {
        printf("Creating Node\n");
    }
    Node newNode = createNode(element, priority, queue);
    if (newNode == NULL)
    {
        return PQ_OUT_OF_MEMORY;
    }
    if (PRINT)
    {
        printf("Node created\n");
    }

    if (queue->list == NULL) //if this is the first element in the queue
    {
        if (PRINT)
        {
            printf("Inserting first node\n");
        }

        queue->list = newNode;
        return PQ_SUCCESS;
    }

    bool node_added = false;
    Node temp_node = NULL;
    pqGetFirst(queue);
    if (PRINT)
    {
        printf("Inserting Node\n");
    }

    while (queue->iterator)
    {

        if (queue->compare_priority(priority, queue->iterator->pq_element_priority) > 0) //priority>curent priority so we add
        {

            if (temp_node == NULL)
            {
                queue->list = newNode;
                newNode->next = queue->iterator;
            }
            else
            {
                temp_node->next = newNode;
                newNode->next = queue->iterator;
            }
            node_added = true;
            break;
        }
        if (PRINT)
        {
            printf("In the loop \n");
        }
        temp_node = queue->iterator;
        pqGetNext(queue);
    }
    if (!node_added)
    {
        temp_node->next = newNode;
    }

    if (PRINT)
    {
        printf("Node Inserted\n");
    }

    return PQ_SUCCESS;
}

PriorityQueueResult pqChangePriority(PriorityQueue queue, PQElement element,
                                     PQElementPriority old_priority, PQElementPriority new_priority)
{
    if (queue == NULL || element == NULL || old_priority == NULL || new_priority == NULL)
    {
        return PQ_NULL_ARGUMENT;
    }
    if (!pqContains(queue, element))
    {
        return PQ_ELEMENT_DOES_NOT_EXISTS;
    }
    PQElement temp_element = queue->iterator->pq_element;
    pqRemoveElement(queue, queue->iterator->pq_element);
    if (pqInsert(queue, temp_element, new_priority) == PQ_OUT_OF_MEMORY)
    {
        return PQ_OUT_OF_MEMORY;
    }

    return PQ_SUCCESS;
}

PriorityQueueResult pqRemove(PriorityQueue queue)
{
    if (queue == NULL)
    {
        return PQ_NULL_ARGUMENT;
    }
    pqGetFirst(queue);
    Node temp_node = queue->iterator;
    pqGetNext(queue);
    queue->list = queue->iterator;
    queue->iterator_defined = false;
    destroyNode(temp_node, queue);

    return PQ_SUCCESS;
}

PriorityQueueResult pqRemoveElement(PriorityQueue queue, PQElement element)
{
    if (queue == NULL || element == NULL)
    {
        return PQ_NULL_ARGUMENT;
    }
    bool found = false;
    Node temp_node = NULL, node_to_remove = NULL;
    pqGetFirst(queue);

    while (queue->iterator)
    {
        if (queue->equal_elements(queue->iterator->pq_element, element))
        {
            found = true;
            if (temp_node == NULL)
            {
                node_to_remove = queue->iterator;
                queue->list = pqGetNext(queue);
                destroyNode(node_to_remove, queue);
            }
            else
            {
                node_to_remove = queue->iterator;
                pqGetNext(queue);
                destroyNode(node_to_remove, queue);
                temp_node->next = queue->iterator;
            }
        }
        temp_node = queue->iterator;
        pqGetNext(queue);
    }

    if (!found)
    {
        return PQ_ELEMENT_DOES_NOT_EXISTS;
    }

    return PQ_SUCCESS;
}

PQElement pqGetFirst(PriorityQueue queue)
{
    if (PRINT)
    {
        printf("Getting first\n");
    }

    if (queue == NULL)
    {
        return NULL;
    }

    if (queue->list == NULL)
    {
        return NULL;
    }
    if (PRINT)
    {
        printf("Got First\n");
    }

    queue->iterator = queue->list;
    return queue->list->pq_element;
}

PQElement pqGetNext(PriorityQueue queue)
{
    if (PRINT)
    {
        printf("Getting next\n");
    }

    if (queue == NULL || queue->iterator == NULL)
    {
        if (PRINT)
        {
            printf("pqGetNext: returning null\n");
        }

        return NULL;
    }
   // if(queue->iterator->next == NULL)
    queue->iterator = queue->iterator->next;
    if (PRINT)
    {
        if(queue->iterator == NULL){
            
        }
        printf("Got next\n");
    }
    if(queue->iterator == NULL){
        if(PRINT){
            printf("iterator is null\n");
        }
        
        return NULL;
    }

    return queue->iterator->pq_element;
}

PriorityQueueResult pqClear(PriorityQueue queue)
{
    if (queue == NULL)
    {
        return PQ_NULL_ARGUMENT;
    }

    int queue_size = pqGetSize(queue);
    for (int i = 0; i < queue_size; i++)
    {
        pqRemove(queue);
    }

    return PQ_SUCCESS;
}

static Node createNode(PQElement element, PQElementPriority priority, PriorityQueue queue)
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

static void destroyNode(Node node, PriorityQueue queue)
{
    if (node == NULL)
    {
        return;
    }

    queue->free_element(node->pq_element);
    queue->free_priority(node->pq_element);
    free(node);
}