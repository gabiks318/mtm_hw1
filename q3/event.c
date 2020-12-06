#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "event.h"


#define DEBUG false

static void debugPrint(char* text){
    if(DEBUG){
        printf(text);
        printf("\n");
    }
}

static PQElement copyMember(PQElement member);
static bool equalMembers(PQElement member1, PQElement member2);
static void destroyMember(PQElement member);
static PQElementPriority copyID(PQElementPriority ID);
static int compareID(PQElementPriority ID1, PQElementPriority ID2);
static void destroyID(PQElementPriority ID);

struct Event_t{
    int event_id;
    char* event_name;
    PriorityQueue member_queue;
    Date event_date;
}; 


static PQElement copyMember(PQElement member){
    return memberCopy(member);
}

static bool equalMembers(PQElement member1, PQElement member2){
    return memberEqual(member1, member2);
}

static void destroyMember(PQElement member){
    memberDestroy(member);
}

static PQElementPriority copyID(PQElementPriority ID){
    if(!ID){
        return NULL;
    }
    int *copy = malloc(sizeof(*copy));
    if(!copy){
        return NULL;
    }

    *copy = *(int*)ID;
    return copy;
}

static int compareID(PQElementPriority ID1, PQElementPriority ID2){
    return (*(int *)ID2 - *(int *)ID1);
}

static void destroyID(PQElementPriority ID){
    free(ID);
}



Event eventCreate(char* event_name, int event_id, Date date)
{
    if(event_name == NULL  || date == NULL)
    {
        return NULL;
    }

    Event event = malloc(sizeof(*event));
    if(event == NULL)
    {
        return NULL;
    }
    
    event->event_date = dateCopy(date);
    if(event->event_date == NULL)
    {
        free(event);
        return NULL;
    }

    event->event_id = event_id;

    char* event_name_copy = malloc(sizeof(char)*(strlen(event_name) + 1));
    if(event_name_copy == NULL)
    {
        free(event->event_date);
        free(event);
        return NULL;
    }
    strcpy(event_name_copy,event_name);
    event->event_name = event_name_copy;

    PriorityQueue member_queue = pqCreate(copyMember, destroyMember, equalMembers, copyID, destroyID, compareID);

    if(member_queue == NULL){
        free(event->event_date);
        free(event->event_name);
        free(event);
        return NULL;
    }
    event->member_queue = member_queue;

    return event;
}

void eventDestroy(Event event)
{
    if(event == NULL){
        return;
    }
    free(event->event_name);
    dateDestroy(event->event_date);
    pqDestroy(event->member_queue);
    free(event);
}

Event eventCopy(Event event)
{
    if(event == NULL)
    {
        return NULL;
    }

    PriorityQueue member_queue_copy = pqCopy(event->member_queue);
    if(member_queue_copy == NULL){
        
        return NULL;
    }

    Event event_copy = eventCreate(event->event_name, event->event_id, dateCopy(event->event_date));
    if(event_copy == NULL)
    {
        pqDestroy(member_queue_copy);
        return NULL;
    }

    event_copy->member_queue = member_queue_copy;

    return event_copy;
}

bool eventEqual(Event event_1, Event event_2)
{
    if(event_1 == NULL){
        return false;
    }
    return event_1->event_id == event_2->event_id;
}

Date eventGetDate(Event event)
{
    if(event == NULL){
        return NULL;
    }

    return event->event_date;
}

char* eventGetName(Event event)
{
    if(event == NULL){
        return NULL;
    }
    return event->event_name;
}

int eventGetID(Event event)
{
    if(event == NULL){
        return 0;
    }

    return event->event_id;
}


PriorityQueue eventGetMemberQueue(Event event)
{
    if(event == NULL){
        debugPrint("event is null");
        return NULL;
    }
    return event->member_queue;
}

EventResult eventAddMember(Event event, Member member)
{
    if(event == NULL || member == NULL)
    {
        return EVENT_NULL_ARGUMENT;
    }

    if(pqContains(event->member_queue, member)){
        return EVENT_MEMBER_ID_ALREADY_EXISTS;
    }
    
    if(pqInsert(event->member_queue, member, memberGetId(member)) == PQ_OUT_OF_MEMORY){
        return EVENT_OUT_OF_MEMORY;
    }
    return EVENT_SUCCESS;
}



EventResult eventRemoveMember(Event event, Member member)
{
    if(event == NULL || member == NULL){
        return EVENT_NULL_ARGUMENT;
    }
    if(pqContains(event->member_queue, member)){
        if(pqRemoveElement(event->member_queue, member) == PQ_SUCCESS){
            return EVENT_SUCCESS;
        } else{
            return EVENT_NULL_ARGUMENT;
        }
    }
    return EVENT_IVALID_MEMBER_ID;   
}

bool eventMemeberExists(Event event, Member member){
    if(event == NULL || member == NULL){
        return false;
    }

    return pqContains(event->member_queue, member);
}
