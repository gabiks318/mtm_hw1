#ifndef MEMBER_LIST_H
#define MEMBER_LIST_H

#include "member.h"

typedef struct Node_t *Node;

typedef enum NodeResult_t {
    NODE_SUCCESS,
    NODE_OUT_OF_MEMORY,
    NODE_NULL_ARGUMENT
} NodeResult;

Node nodeCreate(Member member);
void nodeDestroy(Node node);
void nodeDestroyAll(Node node);
NodeResult nodeAddNext(Node node, Member member);
Node nodeGetNext(Node node);
/* 
Copies single node
returns NULL in case of memory error, or node is NULL
*/
Node nodeCopy(Node node);
/*
Copies node and all the nodes connected to it
returns NULL in case of memory error, or node is NULL
*/
Node nodeCopyAll(Node node);
Member nodeGetMember(Node node);
#endif