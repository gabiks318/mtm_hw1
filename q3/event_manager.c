#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "event_manager.h"
#include "date.h"
#include "priority_queue.h"

typedef struct node{
    Member member;
    struct node *next;
} * Node;

static Node nodeCreate(Member member);
static void nodeDestroy(Node node);

static Node nodeCreate(Member member)
{
    if(member == NULL) 
    {
        return EM_NULL_ARGUMENT;
    }
    Node node = malloc(sizeof(*node));
    if(node == NULL)
    {
        return EM_OUT_OF_MEMORY;
    }
    node->member = member;
    node->next = NULL;
    return node;
}
static void nodeDestroy(Node node)
{
    // Doesn't free the member inside, should be done seperately
    free(Node);
}


typedef struct member{
    int member_id;
    char* member_name;
} *Member;

static Member memberCreate(int member_id, char* member_name);
static void memberDestroy(Memeber member);

static Member memberCreate(int member_id, char* member_name){
    if(member_name == NULL){
        return EM_NULL_ARGUMENT;
    }
    if(member_id < 0){
        return EM_INVALID_MEMBER_ID;
    }

    Member member = malloc(sizeof(*member));
    if(member == NULL){
        return EM_OUT_OF_MEMORY;
    }

    member->member_id = member_id;
    member->member_name = member_name;
    member->next = NULL;

    return member;
}

static void memberDestroy(Memeber member)
{
    free(member);
}

typedef struct event{
    int event_id;
    char* event_name;
    Node members_list_event;
} *Event;

static Event eventCreate(char* event_name, int event_id);
static void eventDestroy(Event event);
static Event eventCopy(Event event);
static bool eventEqual(Event event_1, Event event_2);
static bool eventCheckMemberExist(int member_id);
static int eventAddMember(Member member);
static int eventRemoveMember(Member member);


static Event eventCreate(char* event_name, int event_id)
{
    if(event_name == NULL)
    {
        return EM_NULL_ARGUMENT;
    }
    if(event_id < 0)
    {
        return EM_INVALID_EVENT_ID;
    }
    Event event = malloc(sizeof(*event));
    if(event == NULL){
        return EM_OUT_OF_MEMORY;
    }
    
    event->event_id = event_id;
    event->event_name = event_name;
    event->members_list = NULL;

    return event;
}

static void eventDestroy(Event event)
{
    if(event == NULL){
        return;
    }

    Node node_to_destroy = event->members_list_event;
    Node temp = NULL;
    while(next != NULL){
        temp = node_to_destroy->next;
        free(node_to_destroy);
        node_to_destroy = temp;
    }
    
    free(event);
}

static Event eventCopy(Event event){
    if(event == NULL){
        return NULL;
    }
    Event event_copy = eventCreate(event->event_id, event->event_id);
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
    
}

EventManagerResult emAddEventByDiff(EventManager em, char* event_name, int days, int event_id)
{
    
}

EventManagerResult emRemoveEvent(EventManager em, int event_id)
{
    
}

EventManagerResult emChangeEventDate(EventManager em, int event_id, Date new_date)
{
    
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