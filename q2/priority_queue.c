#include <stdio.h>
#include "priority_queue.h"

struct PriorityQueue_t {
    CopyPQElement copy_element;
    FreePQElement free_element;
    EqualPQElements equal_elements;
    CopyPQElementPriority copy_priority;
    FreePQElementPriority free_priority;
    ComparePQElementPriorities compare_priority;
    typedef struct node {
    PQElement pq_element;
    PQElementPriority pq_element_priority;
    struct node* next;
    } *Node;
};




PriorityQueue pqCreate(CopyPQElement copy_element, FreePQElement free_element,
EqualPQElements equal_elements, CopyPQElementPriority copy_priority,
FreePQElementPriority free_priority, ComparePQElementPriorities compare_priority)
{
    if(!copy_element || !free_element || !equal_elements || !copy_priority || !free_priority || !compare_priority)
    {
        return NULL;
    }
    
    PriorityQueue priority_queue = malloc(sizeof(*priority_queue));
    if(priority_queue == NULL) return NULL;
    
    priority_queue->copy_priority = copy_priority;
    priority_queue->free_priority = free_priority;
    priority_queue->compare_priority = compare_priority;
    priority_queue->copy_element = copy_element;
    priority_queue->free_element = free_element;
    priority_queue->equal_elements = equal_elements;

    return priority_queue;
}

void pqDestroy(PriorityQueue queue){
    
}

PriorityQueue pqCopy(PriorityQueue queue){
    
}

int pqGetSize(PriorityQueue queue){

}

bool pqContains(PriorityQueue queue, PQElement element){

}

PriorityQueueResult pqInsert(PriorityQueue queue, PQElement element, PQElementPriority priority){
    
}

PriorityQueueResult pqChangePriority(PriorityQueue queue, PQElement element,
                                     PQElementPriority old_priority, PQElementPriority new_priority){

}

PriorityQueueResult pqRemove(PriorityQueue queue){
    
}

PriorityQueueResult pqRemoveElement(PriorityQueue queue, PQElement element){
    
}

PQElement pqGetFirst(PriorityQueue queue){

}

PQElement pqGetNext(PriorityQueue queue){

}

PriorityQueueResult pqClear(PriorityQueue queue){

}