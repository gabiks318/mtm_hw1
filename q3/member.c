#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "member.h"

#define PRINT false


void memberPrintDebug(char* text){
    if(PRINT){
        printf(text);
        printf("\n");
    }
}

struct Member_t{
    int member_id;
    char* member_name;
};

Member memberCreate(int member_id, char* member_name){
    if(member_name == NULL || member_id < 0){
        return NULL;
    }
    
    Member member = (Member)malloc(sizeof(*member));
    if(member == NULL){
        return NULL;
    }

    member->member_id = member_id;
    member->member_name = member_name;

    return member;
}

void memberDestroy(Member member)
{
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