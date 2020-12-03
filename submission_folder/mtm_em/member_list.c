#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "member_list.h"

#define DEBUG false

struct Node_t{
    Member member;
    Node next;
};

static void printDebug(char* text){
    if(DEBUG){
        printf(text);
        printf("\n");
    }
}

Node nodeCreate(Member member)
{
    printDebug("Creating node");
    if(member == NULL) 
    {
        return NULL;
    }
    Node node = malloc(sizeof(*node));
    if(node == NULL)
    {
        return NULL;
    }
    printDebug("copying member");
    Member copy_member = memberCopy(member);
    if(copy_member == NULL){
        free(node);
        return NULL;
    }
    printDebug("member copied");
    node->member = copy_member;
    node->next = NULL;
    printDebug("Node created");
    return node;
}

void nodeDestroy(Node node)
{
    printDebug("Destroying node");
    memberDestroy(node->member);
    free(node);
}

void nodeDestroyAll(Node node){
    printDebug("Destroying all");
    if(node == NULL){
        return;
    }
    Node temp = node->next;
    while(node != NULL){
        printDebug("Destroying node");
        nodeDestroy(node);
        node = temp;
        if(temp != NULL){
            temp = node->next;
        }
    }
    printDebug("All nodes Destroyed");
}

NodeResult nodeAddNext(Node node, Member member){
    printDebug("Adding Node");
    
    if(member == NULL){
        printDebug("nodeAddNext NULL argument");
        return NODE_NULL_ARGUMENT;
    }
    
    if(nodeMemberExists(node ,member)){
        printDebug("nodeAddNext exist");
        return NODE_MEMBER_ALREADY_EXISTS;
    }

    Node new_node = nodeCreate(member);
    if(new_node == NULL){
        return NODE_OUT_OF_MEMORY;
    }
    printDebug("new node created");
    Node temp = node->next;
    Node current_node = node;
    while (temp != NULL)
    {
        current_node = temp;
        temp = temp->next;
    }
    
    current_node->next = new_node;

    printDebug("Node added");
    
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
    printDebug("Copying all");
    if(node == NULL)
    {
        return NULL;
    }

    Node first_node = nodeCreate(node->member);
    printDebug("First node copied");
    if(first_node == NULL)
    {
        return NULL;
    }
    Node current_node = first_node;
    Node next_node = node->next;
    NodeResult result;
    while(next_node != NULL){
        printDebug("Copying node");
        result = nodeAddNext(current_node, next_node->member);
        if(result == NODE_SUCCESS){
            current_node = current_node->next;
            next_node = next_node->next;
        } else {
            nodeDestroyAll(first_node);
            return NULL;
        }
    }
    printDebug("All copied");
    return first_node;
}

bool nodeMemberExists(Node node, Member member){
    if(node == NULL || member == NULL)
    {
        return false;
    }
    Node temp = node;
    while(temp != NULL)
    {
        if(memberEqual(temp->member, member))
        {
            return true;
        }
        temp = temp->next;
    }

    return false;
}

Member nodeGetMember(Node node){
    if(node == NULL)
    {
        printDebug("node is Null");
        return NULL;
    }
    return node->member;
}

Member nodeFindMemberById(Node node, int member_id){
    if(node == NULL){
        return NULL;
    }

    Member current_member = nodeGetMember(node);
    Node current_node = node;
    while(current_node != NULL){
        if(memberGetId(current_member) == member_id){
            return current_member;
        }
        current_node = current_node->next;
        if(current_node != NULL){
            current_member = nodeGetMember(current_node);
        }
    }

    return NULL;
}

Node nodeMemberRemove(Node node, Member member){
    // If first node removed, derefrence node and make it's value the second node or null
    if(node == NULL || member == NULL){
        return NULL;
    }

    Node current_node = node;
    Node first_node = node;
    Node temp_node = NULL;
    Member current_member = current_node->member;
    while(current_node != NULL){
        if(memberEqual(current_member, member)){
            if(temp_node == NULL){
                if(current_node->next == NULL){
                    first_node = NULL;
                } else {
                    first_node = current_node->next;
                }
                nodeDestroy(current_node);
                return first_node;
            }

            temp_node->next = current_node->next;
            nodeDestroy(current_node);
            return first_node;
        }
        temp_node = current_node;
        current_node = current_node->next;
        if(current_node != NULL){
            current_member = nodeGetMember(current_node);
        }
    }

    return first_node;
}
