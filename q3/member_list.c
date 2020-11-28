#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "member_list.h"

#define PRINT false

struct Node_t{
    Member member;
    struct Node_t *next;
};

void memberListPrintDebug(char* text){
    if(PRINT){
        printf(text);
        printf("\n");
    }
}

Node nodeCreate(Member member)
{
    if(member == NULL) 
    {
        return NULL;
    }
    Node node = malloc(sizeof(*node));
    if(node == NULL)
    {
        return NULL;
    }
    node->member = member;
    node->next = NULL;
    return node;
}

void nodeDestroy(Node node)
{
    // Doesn't free the member inside, should be done seperately
    free(node);
}

void nodeDestroyAll(Node node){
    memberListPrintDebug("Destroying all");
    if(node == NULL){
        return;
    }
    Node temp = node->next;
    while(node != NULL){
        memberListPrintDebug("Destroying node");
        nodeDestroy(node);
        node = temp;
        if(temp != NULL){
            temp = node->next;
        }
    }
    memberListPrintDebug("All nodes Destroyed");
}

NodeResult nodeAddNext(Node node, Member member){
    memberListPrintDebug("Adding Node");
    
    if(node == NULL || member == NULL){
        return NODE_NULL_ARGUMENT;
    }
    
    memberListPrintDebug("vars Checked");
    Node new_node = nodeCreate(member);
    if(new_node == NULL){
        return NODE_OUT_OF_MEMORY;
    }
    memberListPrintDebug("new node created");
    Node temp = node->next;
    Node current_node = node;
    while (temp != NULL)
    {
        current_node = temp;
        temp = temp->next;
    }
    
    current_node->next = new_node;

    memberListPrintDebug("Node added");
    
    return NODE_SUCCESS;
}

Node nodeGetNext(Node node){
    if(node == NULL){
       return NULL; 
    }

    return node->next;
}

Node nodeCopy(Node node){
    if(node == NULL){
        return NULL;
    }

    Node copy_node = nodeCreate(node->member);
    return copy_node;
}

Node nodeCopyAll(Node node)
{
    memberListPrintDebug("Copying all");
    if(node == NULL){
        return NULL;
    }

    Node first_node = nodeCreate(node->member);
    memberListPrintDebug("First node copied");
    if(first_node == NULL){
        return NULL;
    }
    Node current_node = first_node;
    Node next_node = node->next;
    NodeResult result;
    while(next_node != NULL){
        memberListPrintDebug("Copying node");
        result = nodeAddNext(current_node, next_node->member);
        if(result == NODE_SUCCESS){
            current_node = current_node->next;
            next_node = next_node->next;
        } else {
            nodeDestroyAll(first_node);
            return NULL;
        }
    }
    memberListPrintDebug("All copied");
    return first_node;
}
Member nodeGetMember(Node node){
    if(node == NULL){
        return NULL;
    }
    return node->member;
}

