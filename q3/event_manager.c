#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "event_manager.h"
#include "date.h"
#include "priority_queue.h"
#include "member_list.h"
#include "member.h"
#include "event.h"

static void swap(int *p, int *q);
static int findIndexofMax(int* arr, int size);
static void maxSortTwoArrays(int* arr1, int* arr2, int size);
static bool eventManagerEventExists(EventManager em, Date date, char* event_name);
static Event eventManagerfindEventByID(EventManager em, int event_id);
static EventManagerResult eventManagerAddMember(EventManager em, Member member);
static Member eventManagerFindMemberbyID(EventManager em, int member_id);
static int eventManagerMembersAmount(EventManager em);

struct EventManager_t{
    Date event_manager_date_created;
    PriorityQueue event_manager_event_list;
    Node event_manager_member_list;
};

static int eventManagerMembersAmount(EventManager em)
{
    int member_amount = 0;
    NODE_FOREACH(em->event_manager_member_list, iterator_member)
    {
        if(iterator_member != NULL)
        {
            member_amount++;
        }
    }
    return member_amount;
}


static Member eventManagerFindMemberbyID(EventManager em, int member_id)
{
    return nodeFindMemberById(em->event_manager_member_list, member_id);
}

static EventManagerResult eventManagerAddMember(EventManager em, Member member)
{
    if(em->event_manager_member_list == NULL)
    {
        em->event_manager_member_list = nodeCreate(member);
        if(em->event_manager_member_list == NULL)
        {
            return EM_OUT_OF_MEMORY;
        }
        return EM_SUCCESS;

    }
    if(nodeAddNext(em->event_manager_member_list, member) == NODE_OUT_OF_MEMORY)
    {
        return EM_OUT_OF_MEMORY;
    }
    return EM_SUCCESS;
}

static bool eventManagerEventExists(EventManager em, Date date, char* event_name)
{
    if(em == NULL || date == NULL || event_name == NULL){
        return false;
    }
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

static Event eventManagerfindEventByID(EventManager em, int event_id){
    if(em == 0 || event_id < 0){
        return NULL;
    }

    PQ_FOREACH(Event, iterator, em->event_manager_event_list){
        if(eventGetID(iterator) == event_id){
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

    event_manager->event_manager_date_created = dateCopy(date);
    if(event_manager->event_manager_date_created == NULL)
    {
        free(event_manager);
        return NULL;
    }

   // PriorityQueue priority_queue = pqCreate((Event)(eventCopy)(Event), (void)(eventDestroy)(Event), (bool)(eventEqual)(Event,Event), (Date)(dateCopy)(Date), (void)(*dateDestroy)(Date), (int)(dateCompare)(Date,Date));
    PriorityQueue priority_queue = pqCreate((eventCopy), (eventDestroy), (eventEqual), (dateCopy), (dateDestroy), (dateCompare) );

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
    if(em == NULL || event_name == NULL || date == NULL)
    {
        return EM_NULL_ARGUMENT;
    }
    if(dateCompare(date, em->event_manager_date_created) < 0) // TODO: need to ask if >= or >
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
    if(pqContains(em->event_manager_event_list, event_to_add))
    {
        eventDestroy(event_to_add);
        return EM_EVENT_ID_ALREADY_EXISTS;
    }
    if(pqInsert(em->event_manager_event_list, event_to_add, date) == PQ_OUT_OF_MEMORY)
    {
        eventDestroy(event_to_add);
        destroyEventManager(em);
        return EM_OUT_OF_MEMORY;
    }
    eventDestroy(event_to_add);
    return EM_SUCCESS;

}

EventManagerResult emAddEventByDiff(EventManager em, char* event_name, int days, int event_id)
{
    if(em == NULL || event_name == NULL)
    {
        return EM_NULL_ARGUMENT;
    }

    if(event_id < 0){
        return EM_INVALID_EVENT_ID;
    }

    if(days < 0)
    {
        return EM_INVALID_DATE;
    }

    Date date = dateCopy(em->event_manager_date_created);
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
    dateDestroy(date);
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
    if(em == NULL || new_date == NULL)
    {
        return EM_NULL_ARGUMENT;
    }
    if(dateCompare(new_date, em->event_manager_date_created) < 0) // TODO: Check if < or <=
    {
        return EM_INVALID_DATE;
    }
    if(event_id < 0)
    {
        return EM_INVALID_EVENT_ID;
    }
    Event event_to_change = eventManagerfindEventByID(em, event_id);
    if(event_to_change == NULL)
    {
        return EM_EVENT_ID_NOT_EXISTS;
    }
    if(eventManagerEventExists(em ,new_date, eventGetName(event_to_change)))
    {
        return EM_EVENT_ALREADY_EXISTS;
    }
    if(pqChangePriority(em->event_manager_event_list, event_to_change, new_date, eventGetDate(event_to_change)) == PQ_OUT_OF_MEMORY)
    {
        destroyEventManager(em);
        return EM_OUT_OF_MEMORY;
    }
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
    if(nodeMemberExists(em->event_manager_member_list,member))
    {
        memberDestroy(member);
        return EM_MEMBER_ID_ALREADY_EXISTS;
    }
    if(eventManagerAddMember(em, member) == EM_OUT_OF_MEMORY)
    {
        memberDestroy(member);
        destroyEventManager(em);
        return EM_OUT_OF_MEMORY;
    }
    return EM_SUCCESS;
    

}

EventManagerResult emAddMemberToEvent(EventManager em, int member_id, int event_id)
{
    if(em == NULL){
        return EM_NULL_ARGUMENT;
    }
    if(event_id < 0){
        return EM_INVALID_EVENT_ID;
    }
    if(member_id < 0){
        return EM_INVALID_MEMBER_ID;
    }

    Event event = eventManagerfindEventByID(em, event_id);
    if(event == NULL){
        return EM_EVENT_ID_NOT_EXISTS;
    }
    Member member = nodeFindMemberById(em->event_manager_member_list ,member_id);
    if(member == NULL){
        return EM_MEMBER_ID_NOT_EXISTS;
    }

    if(eventMemeberExists(event, member)){
        return EM_EVENT_AND_MEMBER_ALREADY_LINKED;
    }
    
    if(eventAddMember(event, member) == EVENT_OUT_OF_MEMORY){
        destroyEventManager(em);
        return EM_OUT_OF_MEMORY;
    }

    return EM_SUCCESS;
}

EventManagerResult emRemoveMemberFromEvent (EventManager em, int member_id, int event_id)
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
    if(eventRemoveMember(event ,member) == EVENT_IVALID_MEMBER_ID)
    {
        return EM_EVENT_AND_MEMBER_NOT_LINKED;
    }
    return EM_SUCCESS;

}

EventManagerResult emTick(EventManager em, int days)
{
    if(em == NULL){
        return EM_NULL_ARGUMENT;
    }
    if(days <= 0){
        return EM_INVALID_DATE;
    }
    
    for(int i = 0; i < days; i++){
        dateTick(em->event_manager_date_created);
    }
    
    return EM_SUCCESS;
    
}

int emGetEventsAmount(EventManager em)
{
    if(em == NULL){
        return -1;
    }
    
    return pqGetSize(em->event_manager_event_list);
}

char* emGetNextEvent(EventManager em)
{
    if(em == NULL){
        return NULL;
    }

    Event next_event = pqGetFirst(em->event_manager_event_list);
    return eventGetName(next_event);
}

void emPrintAllEvents(EventManager em, const char* file_name)
{
    if(em == NULL || file_name == NULL){
        return;
    }
    FILE* stream = fopen(file_name, "w");
    if(stream == NULL){
        return;
    }
    char* current_name, *current_member_name;
    Date current_date;
    int* day, *month, *year;
    PQ_FOREACH(Event, iterator, em->event_manager_event_list)
    {
        if(iterator != NULL){
            current_name = eventGetName(iterator);
            current_date = eventGetDate(iterator);
            dateGet(current_date, day, month, year);
            fprintf(stream,"%s,%d.%d.%d", current_name, *day, *month, *year);
            NODE_FOREACH(eventGetMemberList(iterator),member_iterator)
            {
                if(member_iterator != NULL){
                   current_member_name =  memberGetName(nodeGetMember(member_iterator));
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
    if(em == NULL || file_name == NULL){
        return;
    }
    
    int member_amount = eventManagerMembersAmount(em);
    int* member_ids = malloc(sizeof(int) * member_amount);
    
    if(member_ids == NULL)
    {
        destroyEventManager(em);
        return;
    }
    int* member_count = malloc(sizeof(int) * member_amount);
    if(member_count == NULL){
        free(member_ids);
        destroyEventManager(em);
        return;
    }
    // Count how many events each member is responsible for
    int i = 0;
    Member current_member;
    NODE_FOREACH(em->event_manager_member_list, member_iterator){
        if(member_iterator != NULL){
            current_member = nodeGetMember(member_iterator);
            member_ids[i] = memberGetId(current_member);
            member_count[i] = 0;
            PQ_FOREACH(Event, iterator, em->event_manager_event_list)
            {
                if(iterator != NULL)
                {
                    if(eventMemeberExists(iterator,current_member))
                    {
                        member_count[i]++;
                    }
                }
                
            }
            i++;
        }
    }
    
    // Sort the count array, and also the member array
    maxSortTwoArrays(member_ids, member_count, member_amount);

    FILE* stream = fopen(file_name, "w");
    if(stream == NULL){
        free(member_count);
        free(member_ids);
        return;
    }

    char* current_name;
    for(int j = member_amount - 1; member_amount >= 0; member_amount--){
        current_member = nodeFindMemberById(em->event_manager_member_list, member_ids[j]);
        current_name = memberGetName(current_member);
        fprintf(stream ,"%s,%d\n", current_name, member_count[j]);
    }
   
    free(member_count);
    free(member_ids);
    fclose(stream);
}

static void maxSortTwoArrays(int* arr1, int* arr2, int size)
{
    int length = size;
    for(; length > 1; length--){
        int i_max = findIndexofMax(arr1, length);
        swap(&arr1[length - 1], &arr1[i_max]);
        swap(&arr2[length - 1], &arr2[i_max]);
    }
}

static int findIndexofMax(int* arr, int size)
{
    int max = arr[0], current_max, index_of_max = 0;
    for(int i = 1; i < size; i++)
    {
        current_max = arr[i];
        if(current_max > max)
        {
            max = current_max;
            index_of_max = i;
        }
    }
    return index_of_max;
}
static void swap(int *p, int *q)
{
    int temp = *p;
    *p = *q;
    *q = temp;
}