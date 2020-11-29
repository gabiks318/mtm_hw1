#include "event.h"
#include "member_list.h"
#include "member.h"
#include "date.h"
#include <stdbool.h>
#include <stdlib.h>


struct Event_t{
    int event_id;
    char* event_name;
    Node event_members_list;
    Date event_date;
} 


Event eventCreate(char* event_name, int event_id, Date date)
{
    if(event_name == NULL || event_id == NULL || date == NULL)
    {
        return EM_NULL_ARGUMENT;
    }

    Event event = malloc(sizeof(*event));
    if(event == NULL)
    {
        return EVENT_OUT_OF_MEMORY;
    }
    
    event->event_date = dateCopy(date);
    event->event_id = event_id;
    event->event_name = event_name;
    event->event_members_list = NULL;

    return event;
}

void eventDestroy(Event event)
{
    if(event == NULL){
        return;
    }
    free(event->event_name);
    nodeDestroyAll(event->event_member_list);
    dateDestroy(event->event_date);
    free(event);
}

Event eventCopy(Event event)
{
    if(event == NULL)
    {
        return EVENT_NULL_ARGUMENT;
    }
    char* event_copy_name = malloc(sizeof(char)*(strlen(event->event_name) + 1));
    strcpy(event_copy_name, event->event_name);
    Event event_copy = eventCreate(event_copy_name, event->event_id, copyDate(event->event_date));
    if(event_copy == EVENT_OUT_OF_MEMORY)
    {
        return EVENT_OUT_OF_MEMORY;
    }
    event_copy->event_members_list = nodeCopyAll(event->event_member_list);

    return event_copy;
}

bool eventEqual(Event event_1, Event event_2)
{
    return event_1->event_id == event_2->event_id;
}

Date eventGetDate(Event event)
{
    return event->event_date;
}

char* eventGetName(Event event)
{
    return event->event_name;
}

int eventGetID(Event event)
{
    return event->event_id;
}

Node eventGetMemberList(Event event)
{
    return event->event_member_list;
}

EventResult eventAddMember(Event event, Member member)
{
    if(event == NULL || member == NULL)
    {
        return EVENT_NULL_ARGUMENT;
    }
    if(nodeAddNext(event->event_member_list, member) == NODE_OUT_OF_MEMORY)
    {
        return EVENT_OUT_OF_MEMORY;
    }
    return EVENT_SUCCESS;
}

void eventRemoveMember(Event event, Member member)
{
    if(nodeMemberExists(event->event_member_list,member))
    {
         nodeMemberRemove(event->event_member_list,member);
         return EVENT_SUCCESS;
    }
    return EVENT_IVALID_MEMBER_ID;
    
}