#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "event.h"
#include "member_list.h"
#include "member.h"
#include "date.h"

#define DEBUG true

static void debugPrint(char* text){
    if(DEBUG){
        printf(text);
        printf("\n");
    }
}

struct Event_t{
    int event_id;
    char* event_name;
    Node event_members_list;
    Date event_date;
}; 


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
        eventDestroy(event);
        return NULL;
    }

    event->event_id = event_id;

    char* event_name_copy = malloc(sizeof(char)*(strlen(event_name) + 1));
    if(event_name_copy == NULL)
    {
        eventDestroy(event);
        return NULL;
    }
    strcpy(event_name_copy,event_name);
    event->event_name = event_name_copy;

    event->event_members_list = NULL;

    return event;
}

void eventDestroy(Event event)
{
    if(event == NULL){
        return;
    }
    free(event->event_name);
    nodeDestroyAll(event->event_members_list);
    dateDestroy(event->event_date);
    free(event);
}

Event eventCopy(Event event)
{
    if(event == NULL)
    {
        return NULL;
    }

    Event event_copy = eventCreate(event->event_name, event->event_id, dateCopy(event->event_date));
    if(event_copy == NULL)
    {
        return NULL;
    }

    event_copy->event_members_list = nodeCopyAll(event->event_members_list);

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

Node eventGetMemberList(Event event)
{
    if(event == NULL){
        debugPrint("event is null");
        return NULL;
    }
    return event->event_members_list;
}

EventResult eventAddMember(Event event, Member member)
{
    if(event == NULL || member == NULL)
    {
        return EVENT_NULL_ARGUMENT;
    }
    if(event->event_members_list == NULL)// here I fixed if we are adding the first member so do this, we might want to move it to nodeAddNext
    {
        event->event_members_list = nodeCreate(member);
        if(event->event_members_list == NULL)
        {
            return EVENT_OUT_OF_MEMORY;
        }
        return EVENT_SUCCESS;

    }
    if(nodeAddNext(event->event_members_list, member) == NODE_OUT_OF_MEMORY)
    {
        return EVENT_OUT_OF_MEMORY;
    }
    return EVENT_SUCCESS;
}

EventResult eventRemoveMember(Event event, Member member)
{
    if(nodeMemberExists(event->event_members_list,member))
    {
         debugPrint("member exists");
         nodeMemberRemove(event->event_members_list,member);
         debugPrint("member removed");
         return EVENT_SUCCESS;
    }
    return EVENT_IVALID_MEMBER_ID;   
}