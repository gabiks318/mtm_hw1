#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "priority_queue.h"

#define DEBUG false

static void printDebug(char* text){
    if(DEBUG){
        printf("%s",text);
        printf("\n");
    }
}

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
};

//function declaration
static Node createNode(PQElement element, PQElementPriority priority, PriorityQueue queue);
static void destroyNode(Node node, PriorityQueue queue);
static Node pqCheckElementExists(PriorityQueue queue, PQElement element, PQElementPriority priority);
static PriorityQueueResult pqRemoveElementPriority(PriorityQueue queue, PQElement element, PQElementPriority priority);
/* static CopyPQElement getElementCopyFunction(PriorityQueue queue);
static FreePQElement getElementFreeFunction(PriorityQueue queue);
static EqualPQElements getElementEqualFunction(PriorityQueue queue);
static CopyPQElementPriority getElementPriorityCopyFunction(PriorityQueue queue);
static FreePQElementPriority getElementPriorityFreeFunction(PriorityQueue queue);
static ComparePQElementPriorities getElementPriorityCompareFunction(PriorityQueue queue); */

//creates a new priority queue
PriorityQueue pqCreate(CopyPQElement copy_element, FreePQElement free_element,
                       EqualPQElements equal_elements, CopyPQElementPriority copy_priority,
                       FreePQElementPriority free_priority, ComparePQElementPriorities compare_priority)
{
    printDebug("Creating pq");
    if (!copy_element || !free_element || !equal_elements || !copy_priority || !free_priority || !compare_priority)
    {
        return NULL;
    }

    PriorityQueue priority_queue = malloc(sizeof(*priority_queue));
    if (priority_queue == NULL){
        return NULL;
    }

    priority_queue->copy_priority = copy_priority;
    priority_queue->free_priority = free_priority;
    priority_queue->compare_priority = compare_priority;
    priority_queue->copy_element = copy_element;
    priority_queue->free_element = free_element;
    priority_queue->equal_elements = equal_elements;
    priority_queue->list = NULL;
    priority_queue->iterator = NULL;

    printDebug("PQ created");
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
        pqInsert(new_queue, queue->iterator->pq_element, queue->iterator->pq_element_priority);
        pqGetNext(queue);
    }

    return new_queue;
}

int pqGetSize(PriorityQueue queue)
{
    printDebug("Getting size");
    
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
    printDebug("ending count");

    return size;
}

bool pqContains(PriorityQueue queue, PQElement element)
{
    printDebug("Checking if conatins");
    if (queue == NULL || element == NULL)
    {
        return false;
    }
    pqGetFirst(queue);
    while (queue->iterator)
    {
        if (queue->equal_elements(element, queue->iterator->pq_element))
        {
            printDebug("Element in queue");
            return true;
        }
        pqGetNext(queue);
    }
    return false;
}

PriorityQueueResult pqInsert(PriorityQueue queue, PQElement element, PQElementPriority priority)
{
    if (queue == NULL || element == NULL || priority == NULL)
    {
        return PQ_NULL_ARGUMENT;
    }
    printDebug("Creating Node");
    Node newNode = createNode(element, priority, queue);
    if (newNode == NULL)
    {
        return PQ_OUT_OF_MEMORY;
    }
    printDebug("Node created");

    if (queue->list == NULL) //if this is the first element in the queue
    {
        queue->list = newNode;
        return PQ_SUCCESS;
    }

    bool node_added = false;
    Node temp_node = NULL;
    pqGetFirst(queue);
    printDebug("Inserting node");

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
        temp_node = queue->iterator;
        pqGetNext(queue);
    }
    if (!node_added)
    {
        temp_node->next = newNode;
    }

    printDebug("Node Inserted");

    return PQ_SUCCESS;
}

PriorityQueueResult pqChangePriority(PriorityQueue queue, PQElement element,
                                     PQElementPriority old_priority, PQElementPriority new_priority)
{
    printDebug("Changing element priority");
    if (queue == NULL || element == NULL || old_priority == NULL || new_priority == NULL)
    {
        return PQ_NULL_ARGUMENT;
    }
    Node temp_element = pqCheckElementExists(queue, element, old_priority);
    if (temp_element == NULL)
    {
        printDebug("Changing element priority, element not in pq");
        return PQ_ELEMENT_DOES_NOT_EXISTS;
    }
    pqRemoveElementPriority(queue, temp_element->pq_element, temp_element->pq_element_priority);
    if (pqInsert(queue, element, new_priority) == PQ_OUT_OF_MEMORY)
    {
        return PQ_OUT_OF_MEMORY;
    }

    printDebug("Changing element priority success");
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
                pqGetNext(queue);
                queue->list = queue->iterator;
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
    printDebug("Getting first");

    if (queue == NULL)
    {
        return NULL;
    }

    if (queue->list == NULL)
    {
        return NULL;
    }
    
    printDebug("Got First");

    queue->iterator = queue->list;
    return queue->list->pq_element;
}

PQElement pqGetNext(PriorityQueue queue)
{
    printDebug("Getting next");

    if (queue == NULL || queue->iterator == NULL)
    {

        return NULL;
    }
   // if(queue->iterator->next == NULL)
    queue->iterator = queue->iterator->next;
    if(queue->iterator == NULL){

        
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

static Node pqCheckElementExists(PriorityQueue queue, PQElement element, PQElementPriority priority){
    if(queue == NULL || element == NULL || priority == NULL){
        return NULL;
    }

    pqGetFirst(queue);
    while (queue->iterator)
    {
        if(queue->equal_elements(element, queue->iterator->pq_element) && queue->compare_priority(priority, queue->iterator->pq_element_priority) == 0){
            return queue->iterator;
        }
        pqGetNext(queue);
    }
    return NULL;
    
}

static PriorityQueueResult pqRemoveElementPriority(PriorityQueue queue, PQElement element, PQElementPriority priority)
{
    if (queue == NULL || element == NULL)
    {
        return PQ_NULL_ARGUMENT;
    }
    bool found = false;
    Node temp_node = NULL, node_to_remove = NULL, current_node;
    pqGetFirst(queue);

    while (queue->iterator)
    {
        current_node = queue->iterator;
        if (queue->equal_elements(current_node->pq_element, element) && queue->compare_priority(current_node->pq_element_priority, priority) == 0)
        {
            found = true;
            if (temp_node == NULL)
            {
                // When the element to remove is the first element in the list
                node_to_remove = queue->iterator;
                pqGetNext(queue);
                queue->list = queue->iterator;
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

/* static CopyPQElement getElementCopyFunction(PriorityQueue queue){
    if(queue == NULL){
        return NULL;
    }

    return queue->copy_element;
}

static FreePQElement getElementFreeFunction(PriorityQueue queue){
    if(queue == NULL){
        return NULL;
    }

    return queue->free_element;
}

static EqualPQElements getElementEqualFunction(PriorityQueue queue){
    if(queue == NULL){
        return NULL;
    }

    return queue->equal_elements;
}

static CopyPQElementPriority getElementPriorityCopyFunction(PriorityQueue queue){
    if(queue == NULL){
        return NULL;
    }

    return queue->copy_priority;
}

static FreePQElementPriority getElementPriorityFreeFunction(PriorityQueue queue){
    if(queue == NULL){
        return NULL;
    }

    return queue->free_priority;
}

static ComparePQElementPriorities getElementPriorityCompareFunction(PriorityQueue queue){
    if(queue == NULL){
        return NULL;
    }

    return queue->compare_priority;
} */

static Node createNode(PQElement element, PQElementPriority priority, PriorityQueue queue)
{
    if(priority == NULL || element == NULL || queue == NULL )
        return NULL;
    
    Node ptr = malloc(sizeof(*ptr));
    if (!ptr)
    {
        return NULL;
    }
    ptr->next = NULL;

    PQElement element_copy = queue->copy_element(element);
    if (element_copy == NULL)
    {
        free(ptr);
        return NULL;
    }
    ptr->pq_element = element_copy;

    PQElementPriority priority_copy = queue->copy_priority(priority);
    if(priority_copy == NULL){
        free(element_copy);
        free(ptr);
        return NULL;
    }
    ptr->pq_element_priority = priority_copy;
    
    return ptr;
}

static void destroyNode(Node node, PriorityQueue queue)
{
    if (node == NULL)
    {
        return;
    }

    queue->free_element(node->pq_element);
    queue->free_priority(node->pq_element_priority);
    free(node);
}