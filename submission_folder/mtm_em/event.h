#ifndef EVENT_H
#define EVENT_H

#include <stdbool.h>
#include "member.h"
#include "date.h"
#include "member_list.h"

typedef struct Event_t *Event;

typedef enum EventResult_t {
    EVENT_SUCCESS,
    EVENT_OUT_OF_MEMORY,
    EVENT_NULL_ARGUMENT,
    EVENT_IVALID_MEMBER_ID,
    EVENT_INVALID_EVENT_ID
} EventResult;

/*eventCreate- creates a new event by name, id and date. event id should be positive.
               return- EVENT_OUT_OF_MEMORY if dynamic allocation failed.
                       EVENT_NULL_ARGUMENT if a NULL argument was sent
                       if succeed returns the new event created */  

Event eventCreate(char* event_name, int event_id, Date date);

/*eventDestroy- destroying the event that sent */

void eventDestroy(Event event);

/*eventCopy- copies the given event to a new one.
           return- EVENT_OUT_OF_MEMORY if dynamic allocation failed.
                   EVENT_NULL_ARGUMENT if a NULL argument was sent
                   if succeed returns the copied event created */

Event eventCopy(Event event);

/*eventEqual- compares the given event by their event id
            return- true if same event id
                    false if not same event id */

bool eventEqual(Event event_1, Event event_2);

/*getEventDate- return the date of the event that sent */

Date eventGetDate(Event event);

/*getEventName- return the name of the event that sent */

char* eventGetName(Event event);

/*getEventID- return the ID of the event that sent */

int eventGetID(Event event);

/*getEvenMemberList- return the pointer to the first member in the list of the members that incharge of the event that sent */

Node getFirstEventMemberList(Event event);   //?????

/*eventAddMember - adding a member to the event member list
                   return EVENT_OUT_OF_MEMORY if dynamic allocation failed.
                          EVENT_NULL_ARGUMENT if a NULL argument was sent.
                          EVENT_SUCCESS if succeed to add*/

EventResult eventAddMember(Event event, Member member);

/*eventRemoveMember - removing a member from the event*/

Node eventGetMemberList(Event event);

EventResult eventRemoveMember(Event event, Member member);

bool eventMemeberExists(Event event, Member member);

#endif