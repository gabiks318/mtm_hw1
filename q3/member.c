#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "member.h"

#define DEBUG false


static void printDebug(char* text){
    if(DEBUG){
        printf(text);
        printf("\n");
    }
}

struct Member_t{
    int member_id;
    char* member_name;
};

Member memberCreate(int member_id, char* member_name){
    printDebug("Creating member");
    if(member_name == NULL || member_id < 0){
        return NULL;
    }
    
    Member member = (Member)malloc(sizeof(*member));
    if(member == NULL){
        return NULL;
    }

    member->member_id = member_id;
    member->member_name = member_name;
    printDebug("member created");
    return member;
}

void memberDestroy(Member member)
{
    free(member->member_name);
    free(member);
}

int memberGetId(Member member){
    if(member == NULL){
        return -1;
    }

    return member->member_id;
}

char* memberGetName(Member member){
    if(member == NULL){
        return '\0';
    }

    return member->member_name;
}

bool memberEqual(Member member1, Member member2)
{
    printDebug("Comparing members");
    return member1->member_id == member2->member_id;
}

Member memberCopy(Member member)
{
    printDebug("Copying member");
    char* member_copy_name = malloc(sizeof(char)*(strlen(member->member_name) + 1));
    if(member_copy_name == NULL)
    {
        printDebug("malloc is null");
        return NULL;
    }
    printDebug("created space for string name");
    strcpy(member_copy_name, member->member_name);
    printDebug("copied the member name");
    Member member_copy = memberCreate(member->member_id, member_copy_name);
    if(member_copy == NULL)
    {
        return NULL;
    }
    printDebug("copied the member");
    return member_copy;
}