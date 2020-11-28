#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "event_manager.h"
#include "date.h"
#include "priority_queue.h"
#include "member_list.h"
#include "member.h"

typedef struct Node_t* Node;


typedef struct event{
    Date event_date;
    int event_id;
    char* event_name;
    Node members_list_event;
} *Event;

static Event eventCreate(char* event_name, int event_id, Date date);
static void eventDestroy(Event event);
static Event eventCopy(Event event);
static bool eventEqual(Event event_1, Event event_2);
static bool eventCheckMemberExist(int member_id);//need to write?
static int eventAddMember(Member member);//need to write?
static int eventRemoveMember(Member member);//need to write?
static bool eventExists(Date date, char* event_name); //need to write!
static Event findEventByID(int event_id);//need to write!


static Event eventCreate(char* event_name, int event_id, Date date)
{
    if(event_name == NULL)
    {
        return EM_NULL_ARGUMENT;
    }
    if(event_id < 0)
    {
        return EM_INVALID_EVENT_ID;
    }
    // I think we neeed to also check if date is valid
    Event event = malloc(sizeof(*event));
    if(event == NULL){
        return EM_OUT_OF_MEMORY;
    }
    
    event->event_id = event_id;
    event->event_name = event_name;
    event->event_date = dateCopy(date);
    event->members_list_event = NULL;

    return event;
}

static void eventDestroy(Event event)
{
    if(event == NULL){
        return;
    }

    Node node_to_destroy = event->members_list_event;
    Node temp = NULL;
    while(nodeGetNext(node_to_destroy) != NULL){
        temp = nodeGetNext(node_to_destroy);
        free(node_to_destroy);
        node_to_destroy = temp;
    }
    free(node_to_destroy);
    
    free(event);
}

static Event eventCopy(Event event){
    if(event == NULL){
        return NULL;
    }
    Event event_copy = eventCreate(event->event_name, event->event_id, event->event_date);
    event_copy->members_list_event = event->members_list_event;
}

static bool eventEqual(Event event_1, Event event_2)
{
    return event_1->event_id == event_2->event_id;
}


struct EventManager_t{
    Date date_created;
    PriorityQueue event_list;
    Node member_list;
}


EventManager createEventManager(Date date)
{
    EventManager event_manager = malloc(sizeof(*event_manager));
    if(event_manager == NULL)
    {

    }
    event_manager->date_created = date;

    PriorityQueue priority_queue = pqCreate(eventCopy, eventDestroy, eventEqual, dateCopy, dateDestroy, dateCompare);
    if(priority_queue == NULL)
    {
        return NULL;
    }
    return event_manager;   
}

void destroyEventManager(EventManager em)
{
    pqDestroy(em->event_list);
    nodeDestroy(em->member_list);
    free(em);
    
}

EventManagerResult emAddEventByDate(EventManager em, char* event_name, Date date, int event_id)
{
    if(em == NULL || event_name == NULL || date == NULL || event_id == NULL)
    {
        return EM_NULL_ARGUMENT;
    }
    if(dateCompare(date, em->date_created) >= 0)
    {
        return EM_INVALID_DATE;
    }
    if(event_id < 0)
    {
        return EM_INVALID_EVENT_ID;
    }
    if(eventExists(date, event_name))
    {
        return EM_EVENT_ALREADY_EXISTS;
    }
  
    Event event_to_add = eventCreate(event_name, event_id);

    if(event_to_add == EM_OUT_OF_MEMORY)
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
inline EventManagerResult checkResult(EventManagerResult result)
{
    if(result == EM_INVALID_DATE)
    {
        return EM_INVALID_DATE;
    }
    if(result == EM_INVALID_EVENT_ID)
    {
        return EM_INVALID_EVENT_ID;
    }
    if(result == EM_EVENT_ALREADY_EXISTS)
    {
        return EM_EVENT_ALREADY_EXISTS;
    }
    if(result == EM_EVENT_ID_ALREADY_EXISTS)
    {
        return EM_EVENT_ID_ALREADY_EXISTS;
    }
    if(result == EM_OUT_OF_MEMORY)
    {
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
    return checkResult(result);
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
    If(pqChangePriority(em, event_to_change, new_date, event_to_change->event_date) == PQ_OUT_OF_MEMORY)
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