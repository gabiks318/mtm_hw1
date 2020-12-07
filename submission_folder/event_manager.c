#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "event_manager.h"
#include "priority_queue.h"
#include "date.h"
#include "member.h"
#include "event.h"

#define DEBUG false
#define EQUAL 0

static void debugPrint(char* text){
    if(DEBUG){
        printf(text);
        printf("\n");
    }
}


static bool eventManagerEventExists(EventManager em, Date date, char* event_name);
static Event eventManagerfindEventByID(EventManager em, int event_id);
static Member eventManagerFindMemberbyID(EventManager em, int member_id);

static PQElement copyEvent(PQElement event);
static bool equalEvents(PQElement event1, PQElement event2);
static void destroyEvent(PQElement event);
static PQElementPriority copyDate(PQElementPriority date);
static int compareDates(PQElementPriority date1, PQElementPriority date2);
static void destroyDate(PQElementPriority date);

static PQElement copyMember(PQElement member);
static bool equalMembers(PQElement member1, PQElement member2);
static void destroyMember(PQElement member);
static PQElementPriority copyID(PQElementPriority ID);
static int compareID(PQElementPriority ID1, PQElementPriority ID2);
static void destroyID(PQElementPriority ID);


struct EventManager_t{
    Date event_manager_date;
    PriorityQueue event_manager_event_list;
    PriorityQueue event_manager_member_list;
};

static PQElement copyMember(PQElement member)
{
    return memberCopy(member);
}

static bool equalMembers(PQElement member1, PQElement member2)
{
    return memberEqual(member1, member2);
}

static void destroyMember(PQElement member)
{
    memberDestroy(member);
}

static PQElementPriority copyID(PQElementPriority ID)
{
    if(!ID)
    {
        return NULL;
    }
    int *copy = malloc(sizeof(*copy));
    if(!copy)
    {
        return NULL;
    }

    *copy = *(int*)ID;
    return copy;
}

static int compareID(PQElementPriority ID1, PQElementPriority ID2)
{
    return (*(int *)ID2 - *(int *)ID1);
}

static void destroyID(PQElementPriority ID)
{
    free(ID);
}


static PQElement copyEvent(PQElement event)
{
    return eventCopy(event);
}

static bool equalEvents(PQElement event1, PQElement event2)
{
    if(strcmp(eventGetName((Event)event1),eventGetName((Event)event2)) == EQUAL
    && dateCompare(eventGetDate((Event)event1),eventGetDate((Event)event2)) == EQUAL)
    {
        return true;
    }

    return false;
}

static void destroyEvent(PQElement event)
{
    eventDestroy(event);
}

static PQElementPriority copyDate(PQElementPriority date)
{
    return dateCopy(date);
}

static int compareDates(PQElementPriority date1, PQElementPriority date2)
{
    return dateCompare(date2, date1);
}

static void destroyDate(PQElementPriority date)
{
    dateDestroy(date);
}

static Member eventManagerFindMemberbyID(EventManager em, int member_id)
{
    PQ_FOREACH(Member, iterator, em->event_manager_member_list)
    {
        if(iterator != NULL && *memberGetId(iterator) == member_id)
        {
            return iterator;
        }
    }
    return NULL;
}

static bool eventManagerEventExists(EventManager em, Date date, char* event_name)
{
    if(em == NULL || date == NULL || event_name == NULL)
    {
        return false;
    }
    PQ_FOREACH(Event, iterator, em->event_manager_event_list)
    {
        if(iterator != NULL)
        {
            if(dateCompare(eventGetDate(iterator), date) == EQUAL && strcmp(event_name, eventGetName(iterator)) == EQUAL)
            {
                return true;
            }
        }
    }
    return false;    
}

static Event eventManagerfindEventByID(EventManager em, int event_id)
{
    if(em == NULL || event_id < 0)
    {
        return NULL;
    }
    PQ_FOREACH(Event, iterator, em->event_manager_event_list)
    {

        if(iterator!= NULL && eventGetID(iterator) == event_id)
        {
            return iterator;
        }
    }

    return NULL;
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

    event_manager->event_manager_date = dateCopy(date);
    if(event_manager->event_manager_date == NULL)
    {
        free(event_manager);
        return NULL;
    }

    PriorityQueue event_queue = pqCreate(copyEvent, destroyEvent, equalEvents, copyDate, destroyDate, compareDates);

    if(event_queue == NULL)
    {
        dateDestroy(event_manager->event_manager_date);
        free(event_manager);
        return NULL;
    }
    event_manager->event_manager_event_list = event_queue;

    PriorityQueue member_queue = pqCreate(copyMember, destroyMember, equalMembers, copyID, destroyID, compareID);

    if(member_queue == NULL)
    {
        dateDestroy(event_manager->event_manager_date);
        pqDestroy(event_queue);
        free(event_manager);
        return NULL;
    }
    event_manager->event_manager_member_list = member_queue;

    return event_manager;   
}

void destroyEventManager(EventManager em)
{
    dateDestroy(em->event_manager_date);
    pqDestroy(em->event_manager_event_list);
    pqDestroy(em->event_manager_member_list);
    free(em);
}

EventManagerResult emAddEventByDate(EventManager em, char* event_name, Date date, int event_id)
{
    debugPrint("adding event");
    if(em == NULL || event_name == NULL || date == NULL)
    {
        return EM_NULL_ARGUMENT;
    }
    if(dateCompare(date, em->event_manager_date) < 0) 
    {
        return EM_INVALID_DATE;
    }
    if(event_id < 0)
    {
        
        return EM_INVALID_EVENT_ID;
    }
    
    if(eventManagerEventExists(em, date, event_name))
    {
        return EM_EVENT_ALREADY_EXISTS;
    }
    
    debugPrint("adding event checks passed");
    Event event_to_add = eventCreate(event_name, event_id, date);
    if(event_to_add == NULL)
    {
        destroyEventManager(em);
        return EM_OUT_OF_MEMORY;
    }
    if(pqContains(em->event_manager_event_list, event_to_add))
    {
        eventDestroy(event_to_add);
        return EM_EVENT_ID_ALREADY_EXISTS;
    }
    debugPrint("adding event adding event");
    if(pqInsert(em->event_manager_event_list, event_to_add, date) == PQ_OUT_OF_MEMORY)
    {
        eventDestroy(event_to_add);
        destroyEventManager(em);
        return EM_OUT_OF_MEMORY;
    }
    eventDestroy(event_to_add);
    debugPrint("adding event succeded");
    return EM_SUCCESS;
}

EventManagerResult emAddEventByDiff(EventManager em, char* event_name, int days, int event_id)
{
    debugPrint("adding event by diff");
    if(em == NULL || event_name == NULL)
    {
        return EM_NULL_ARGUMENT;
    }

    if(event_id < 0)
    {
        return EM_INVALID_EVENT_ID;
    }

    if(days < 0)
    {
        return EM_INVALID_DATE;
    }

    debugPrint("adding event by diff checks passed");
    Date date = dateCopy(em->event_manager_date);
    if(date == NULL)
    {
        destroyEventManager(em);
        return EM_OUT_OF_MEMORY;
    }
    debugPrint("date copied");
    for(int i = 0; i < days; i++)
    {
        dateTick(date);
    }
    EventManagerResult result = emAddEventByDate(em, event_name, date, event_id);
    dateDestroy(date);
    debugPrint("adding event by diff finished");
    return result;
}


EventManagerResult emRemoveEvent(EventManager em, int event_id)
{
    if(em == NULL)
    {
        return EM_NULL_ARGUMENT;
    }

    if(event_id < 0)
    {
        return EM_INVALID_EVENT_ID;
    }

    Event event_to_remove = eventManagerfindEventByID(em, event_id);
    if(event_to_remove == NULL)
    {
        return EM_EVENT_NOT_EXISTS;
    }

    pqRemoveElement(em->event_manager_event_list, event_to_remove);
    return EM_SUCCESS;
}

EventManagerResult emChangeEventDate(EventManager em, int event_id, Date new_date)
{
    int day,month,year;
    dateGet(new_date,&day,&month,&year);
    printf("%d.%d.%d\n",day,month,year);
    debugPrint("0");
    if(em == NULL || new_date == NULL)
    {
        return EM_NULL_ARGUMENT;
    }
    debugPrint("1");

    if(dateCompare(new_date, em->event_manager_date) < 0)
    {
        return EM_INVALID_DATE;
    }
    debugPrint("2");

    if(event_id < 0)
    {
        return EM_INVALID_EVENT_ID;
    }
    debugPrint("going to search for event now");
    Event event_to_change = eventManagerfindEventByID(em, event_id);
    if(event_to_change == NULL)
    {
        debugPrint("returning event not found");

        return EM_EVENT_ID_NOT_EXISTS;
    }
        debugPrint("checking if exists");
    if(eventManagerEventExists(em ,new_date, eventGetName(event_to_change)))
    {
        return EM_EVENT_ALREADY_EXISTS;
    }
        debugPrint("changing");

    Event copy_event_to_change= copyEvent(event_to_change);
    char* event_name = eventGetName(copy_event_to_change);
    printf("%s\n",event_name);
    dateGet(eventGetDate(copy_event_to_change),&day,&month,&year);
    printf("%d.%d.%d\n",day,month,year);
    dateGet(new_date,&day,&month,&year);
    printf("%d.%d.%d\n",day,month,year);
    if(pqChangePriority(em->event_manager_event_list, copy_event_to_change, eventGetDate(event_to_change), new_date) == PQ_OUT_OF_MEMORY)//changed her
    {
        destroyEventManager(em);
        return EM_OUT_OF_MEMORY;
    }
    dateGet(eventGetDate(eventManagerfindEventByID(em,4)),&day,&month,&year);
    printf("%d.%d.%d\n",day,month,year);
    dateGet(eventGetDate(eventManagerfindEventByID(em,3)),&day,&month,&year);
    printf("%d.%d.%d\n",day,month,year);
     dateGet(eventGetDate(eventManagerfindEventByID(em,2)),&day,&month,&year);
    printf("%d.%d.%d\n",day,month,year);
     dateGet(eventGetDate(eventManagerfindEventByID(em,1)),&day,&month,&year);
    printf("%d.%d.%d\n",day,month,year);
    destroyEvent(copy_event_to_change);

    return EM_SUCCESS;
}

EventManagerResult emAddMember(EventManager em, char* member_name, int member_id)
{
    if(em == NULL || member_name == NULL)
    {
        return EM_NULL_ARGUMENT;
    }
    if(member_id < 0)
    {
        return EM_INVALID_MEMBER_ID;
    }

    Member member = memberCreate(member_id, member_name);
    if(member == NULL)
    {
        destroyEventManager(em);
        return EM_OUT_OF_MEMORY;
    }

    if(pqContains(em->event_manager_member_list, member))
    {
        memberDestroy(member);
        return EM_MEMBER_ID_ALREADY_EXISTS;
    }

    if(pqInsert(em->event_manager_member_list, member, memberGetId(member)) == PQ_OUT_OF_MEMORY)
    {
        memberDestroy(member);
        destroyEventManager(em);
        return EM_OUT_OF_MEMORY;
    }

    return EM_SUCCESS;
}

EventManagerResult emAddMemberToEvent(EventManager em, int member_id, int event_id)
{
    if(em == NULL)
    {
        return EM_NULL_ARGUMENT;
    }
    if(event_id < 0)
    {
        return EM_INVALID_EVENT_ID;
    }
    if(member_id < 0)
    {
        return EM_INVALID_MEMBER_ID;
    }

    Event event = eventManagerfindEventByID(em, event_id);
    if(event == NULL)
    {
        return EM_EVENT_ID_NOT_EXISTS;
    }

    Member member = eventManagerFindMemberbyID(em, member_id);
    if(member == NULL)
    {
        return EM_MEMBER_ID_NOT_EXISTS;
    }

    if(pqContains(eventGetMemberQueue(event), member))
    {
        return EM_EVENT_AND_MEMBER_ALREADY_LINKED;
    }
    
    if(pqInsert(eventGetMemberQueue(event), member, memberGetId(member)) == PQ_OUT_OF_MEMORY)
    {
        destroyEventManager(em);
        return EM_OUT_OF_MEMORY;
    }

    return EM_SUCCESS;
}

EventManagerResult emRemoveMemberFromEvent(EventManager em, int member_id, int event_id)
{
    if(em == NULL)
    {
        return EM_NULL_ARGUMENT;
    }
    if(event_id < 0)
    {
        return EM_INVALID_EVENT_ID;
    }
    if(member_id < 0)
    {
        return EM_INVALID_MEMBER_ID;
    }

    Event event = eventManagerfindEventByID(em, event_id);
    if(event == NULL)
    {
        return EM_EVENT_ID_NOT_EXISTS;
    }

    Member member = eventManagerFindMemberbyID(em, member_id);
    if(member == NULL)
    {
        return EM_MEMBER_ID_NOT_EXISTS;
    }

    if(pqRemoveElement(eventGetMemberQueue(event), member)== PQ_ELEMENT_DOES_NOT_EXISTS)
    {
        return EM_EVENT_AND_MEMBER_NOT_LINKED;
    }

    return EM_SUCCESS;

}

EventManagerResult emTick(EventManager em, int days)
{
    if(em == NULL)
    {
        return EM_NULL_ARGUMENT;
    }
    if(days <= 0)
    {
        return EM_INVALID_DATE;
    }
    
     for(int i = 0; i < days; i++)
    {
        Event first_event = pqGetFirst(em->event_manager_event_list);
        while(first_event != NULL && dateCompare(eventGetDate(first_event), em->event_manager_date) == EQUAL)
        {
            pqRemove(em->event_manager_event_list);
            first_event = pqGetFirst(em->event_manager_event_list);
        }
        dateTick(em->event_manager_date);
    }
    return EM_SUCCESS;
    
}

int emGetEventsAmount(EventManager em)
{
    if(em == NULL)
    {
        return -1;
    }
    
    return pqGetSize(em->event_manager_event_list);
}

char* emGetNextEvent(EventManager em)
{
    if(em == NULL)
    {
        return NULL;
    }

    Event next_event = pqGetFirst(em->event_manager_event_list);
    if(next_event == NULL)
    {
        return NULL;
    }
    return eventGetName(next_event);
}

void emPrintAllEvents(EventManager em, const char* file_name)
{
    if(em == NULL || file_name == NULL)
    {
        return;
    }

    FILE* stream = fopen(file_name, "w");

    if(stream == NULL)
    {
        return;
    }
    debugPrint("opened the file");
    char* current_name, *current_member_name;
    Date current_date;
    int day, month, year;
    PQ_FOREACH(Event, iterator, em->event_manager_event_list)
    {
        if(iterator != NULL)
        {
            debugPrint("going on events");
            current_name = eventGetName(iterator);
            current_date = eventGetDate(iterator);
            dateGet(current_date, &day, &month, &year);
            fprintf(stream,"%s,%d.%d.%d", current_name, day, month, year);
            debugPrint("going into members loop 1");
            PriorityQueue current_event_member_queue = eventGetMemberQueue(iterator);
            debugPrint("going into members loop 2");
            PQ_FOREACH(Member, member_iterator, current_event_member_queue)
            {
                if(member_iterator != NULL)
                {
                    debugPrint("going on members");
                   current_member_name =  memberGetName(member_iterator);
                   fprintf(stream, ",%s", current_member_name);
                }
            }
            fprintf(stream,"\n");          
        }
    }
    fclose(stream);
}

void emPrintAllResponsibleMembers(EventManager em, const char* file_name)
{
    if(em == NULL || file_name == NULL)
    {
        return;
    }
    
    int member_amount = pqGetSize(em->event_manager_member_list);
    int* member_count = malloc(sizeof(int) * member_amount);
    if(member_count == NULL)
    {
        destroyEventManager(em);
        return;
    }
    // Count how many events each member is responsible for
    int i = 0;
    PQ_FOREACH(Member ,member_iterator, em->event_manager_member_list)
    {
        if(member_iterator != NULL)
        {
            member_count[i] = 0;
            PQ_FOREACH(Event, iterator, em->event_manager_event_list)
            {
                if(iterator != NULL)
                {
                    if(pqContains(eventGetMemberQueue(iterator), member_iterator))
                    {
                        member_count[i]++;
                    }
                }
                
            }
            i++;
        }
    }
    FILE* stream = fopen(file_name, "w");
    if(stream == NULL)
    {
        free(member_count);
        return;
    }

    int event_amount = emGetEventsAmount(em);
    for(i = event_amount; i > 0; i--)
    {
        for(int j = 0; j < member_amount; j++)
        {
            if(member_count[j] == event_amount)
            {
                int place_in_pq = 0;
                PQ_FOREACH(Member, member_iterator, em->event_manager_member_list)
                {
                    if(place_in_pq == j)
                    {
                        fprintf(stream ,"%s,%d\n", memberGetName(member_iterator), member_count[j]);
                        break;
                    }
                    place_in_pq++;
                }

            }
        }
    }
    free(member_count);
    fclose(stream);
}

