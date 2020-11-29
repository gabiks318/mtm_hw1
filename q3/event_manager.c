#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "event_manager.h"
#include "date.h"
#include "priority_queue.h"
#include "member_list.h"
#include "member.h"
#include "event.h"

static bool eventManagerEventExists(EventManager em, Date date, char* event_name); 
static Event eventManagerfindEventByID(EventManager em, int event_id);//need to write!

struct EventManager_t{
    Date event_manager_date_created;
    PriorityQueue event_manager_event_list;
    Node event_manager_member_list;
}

static bool eventManagerEventExists(EventManager em, Date date, char* event_name)
{
    PQ_FOREACH(Event, iterator, em->event_manager_event_list)
    {
        if(iterator != NULL){
            if(dateCompare(eventGetDate(iterator), date) == 0 && strcmp(event_name,eventGetName(iterator)) == 0)
            {
                return true;
            }
        }
    }
    return false;
    
}

EventManager createEventManager(Date date)
{
    if(date == NULL)
    {
        return NULL;
    }
    EventManager event_manager = malloc(sizeof(*event_manager));
    if(event_manager == NULL)
    {
        return NULL;
    }

    event_manager->event_manager_date_created = copyDate(date);
    if(event_manager->event_manager_date_created == NULL)
    {
        free(event_manager);
        return NULL;
    }

    PriorityQueue priority_queue = pqCreate(eventCopy, eventDestroy, eventEqual, dateCopy, dateDestroy, dateCompare);
    if(priority_queue == NULL)
    {
        dateDestroy(event_manager->event_manager_date_created);
        free(event_manager);
        return NULL;
    }
    event_manager->event_manager_event_list = priority_queue;

    event_manager->event_manager_member_list = NULL;

    return event_manager;   
}

void destroyEventManager(EventManager em)
{
    dateDestroy(em->event_manager_date_created);
    pqDestroy(em->event_manager_event_list);
    nodeDestroyAll(em->event_manager_member_list);
    free(em);
}

EventManagerResult emAddEventByDate(EventManager em, char* event_name, Date date, int event_id)
{
    if(em == NULL || event_name == NULL || date == NULL || event_id == NULL)
    {
        return EM_NULL_ARGUMENT;
    }
    if(dateCompare(date, em->event_manager_date_created) > 0) // TODO: need to ask if >= or >
    {
        return EM_INVALID_DATE;
    }
    if(event_id < 0)
    {
        return EM_INVALID_EVENT_ID;
    }
    
    if(!eventManagerEventExists(em, date, event_name))
    {
        return EM_EVENT_ALREADY_EXISTS;
    }
  
    Event event_to_add = eventCreate(event_name, event_id, date);
    if(event_to_add == NULL)
    {
        destroyEventManager(em);
        return EM_OUT_OF_MEMORY;
    }
    if(pqContains(em, event_to_add))
    {
        eventDestroy(event_to_add);
        return EM_EVENT_ID_ALREADY_EXISTS;
    }
    if(pqInsert(em, event_to_add, date) == PQ_OUT_OF_MEMORY)
    {
        destroyEventManager(em);
        return EM_OUT_OF_MEMORY;
    }
    return EM_SUCCESS;

}

EventManagerResult emAddEventByDiff(EventManager em, char* event_name, int days, int event_id)
{
    if(em == NULL || event_name == NULL || days == NULL || event_id == NULL)
    {
        return EM_NULL_ARGUMENT;
    }
    Date date = dateCopy(em->date_created);
    if(date == NULL)
    {
        destroyEventManager(em);
        return EM_OUT_OF_MEMORY;
    }
    for(int i = 0; i < days; i++)
    {
        dateTick(date);
    }
    EventManagerResult result = emAddEventByDate(em, event_name, date, event_id);
    return result;
}


EventManagerResult emRemoveEvent(EventManager em, int event_id)
{
    if(em == NULL || event_id == NULL)
    {
        return EM_NULL_ARGUMENT;
    }
    if(event_id < 0)
    {
        return EM_INVALID_EVENT_ID;
    }
    Event event_to_remove = findEventByID(event_id);
    if(event_to_remove == NULL)
    {
        return EM_EVENT_NOT_EXISTS;
    }
    PriorityQueueResult result = pqRemoveElement(em, event_to_remove);
    if(result == PQ_SUCCESS)
    {
        return EM_SUCCESS;
    }
}

EventManagerResult emChangeEventDate(EventManager em, int event_id, Date new_date)
{
    if(em == NULL || event_id == NULL || new_date == NULL)
    {
        EM_NULL_ARGUMENT;
    }
    if(dateCompare(new_date, em->date_created) >= 0)
    {
        return EM_INVALID_DATE;
    }
    if(event_id < 0)
    {
        return EM_INVALID_EVENT_ID;
    }
    Event event_to_change = findEventByID(event_id);
    if(event_to_change == NULL)
    {
        return EM_EVENT_ID_NOT_EXISTS;
    }
    if(eventExists(new_date, event_to_change->event_name))
    {
        return EM_EVENT_ALREADY_EXISTS;
    }
    If(pqChangePriority(em, event_to_change, new_date, dateofevent_id ) == PQ_OUT_OF_MEMORY)
    {
        destroyEventManager(em);
        return EM_OUT_OF_MEMORY;
    }
    return EM_SUCCESS;
}

EventManagerResult emAddMember(EventManager em, char* member_name, int member_id)
{

}

EventManagerResult emAddMemberToEvent(EventManager em, int member_id, int event_id)
{

}

EventManagerResult emRemoveMemberFromEvent (EventManager em, int member_id, int event_id)
{

}

EventManagerResult emTick(EventManager em, int days)
{

}

int emGetEventsAmount(EventManager em)
{

}

char* emGetNextEvent(EventManager em)
{

}

void emPrintAllEvents(EventManager em, const char* file_name)
{

}

void emPrintAllResponsibleMembers(EventManager em, const char* file_name)
{

}