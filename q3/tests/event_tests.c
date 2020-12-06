#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "test_utilities.h"
#include "../event.h"

#define NUMBER_TESTS 3

bool testEventCreateDestroy(){
    bool result = true;
    Date date = dateCreate(1,1,2020);
    int event_id = 1;
    char *event_name = "Student day";
    Event event = eventCreate(event_name, event_id, date);
    ASSERT_TEST(event != NULL,returnCreateDestroy);
    
    
    eventDestroy(event);
    returnCreateDestroy:
        dateDestroy(date);
        return result;
}

bool testEventCopyEqual()
{
    bool result = true;
    Date date = dateCreate(1,2,2789);
    Event event = eventCreate("bestevent",2050,date);
    Event event_copy = eventCopy(event);
    ASSERT_TEST(event_copy !=NULL, destroyEventCopy);
    ASSERT_TEST(strcmp(eventGetName(event),eventGetName(event_copy))== 0,destroyEventCopy2);
    ASSERT_TEST(dateCompare(eventGetDate(event),eventGetDate(event_copy))==0,destroyEventCopy2);
    ASSERT_TEST(eventGetID(event) == eventGetID(event_copy), destroyEventCopy2);
    ASSERT_TEST(eventEqual(event,event_copy), destroyEventCopy2);

    destroyEventCopy2:
        eventDestroy(event_copy);
    destroyEventCopy:
        dateDestroy(date);
        eventDestroy(event);
        return result;

}

bool testEventAddRemoveMember(){
    bool result = true;
    Date date = dateCreate(1,2,2789);
    Event event = eventCreate("bestevent",2050,date);
    Member member = memberCreate(12345, "gabi");
    Member member_2 = memberCreate(12346, "yan");
    Member member_3 = memberCreate(12347, "borat");
    ASSERT_TEST(eventAddMember(event,member) == EVENT_SUCCESS,returnEventAddRemoveMember);

    PriorityQueue member_list = eventGetMemberQueue(event);
    ASSERT_TEST(pqContains(member_list, member), returnEventAddRemoveMember);

    eventRemoveMember(event, member);
    ASSERT_TEST(!pqContains(member_list, member), returnEventAddRemoveMember);
    
    ASSERT_TEST(eventAddMember(event,member) == EVENT_SUCCESS,returnEventAddRemoveMember);
    ASSERT_TEST(eventAddMember(event,member_2) == EVENT_SUCCESS,returnEventAddRemoveMember);
    ASSERT_TEST(eventAddMember(event,member_3) == EVENT_SUCCESS,returnEventAddRemoveMember);

    eventRemoveMember(event, member_2);
    ASSERT_TEST(pqContains(member_list, member),returnEventAddRemoveMember);
    ASSERT_TEST(pqContains(member_list, member_3),returnEventAddRemoveMember);
    ASSERT_TEST(!pqContains(member_list, member_2),returnEventAddRemoveMember);

    eventRemoveMember(event, member);
    ASSERT_TEST(!pqContains(member_list, member),returnEventAddRemoveMember);
    ASSERT_TEST(pqContains(member_list, member_3),returnEventAddRemoveMember);
    ASSERT_TEST(!pqContains(member_list, member_2),returnEventAddRemoveMember);
    

    returnEventAddRemoveMember:
        memberDestroy(member_2);
        memberDestroy(member_3);
        memberDestroy(member);
        eventDestroy(event);
        dateDestroy(date);    
        return result;
}

bool (*tests[])(void) = {
    testEventCreateDestroy,
    testEventCopyEqual,
    testEventAddRemoveMember

};

const char* testNames[] = {
    "testEventCreateDestroy",
    "testEventCopyEqual",
    "testEventAddRemoveMember"
};

int main(int argc, char *argv[]) {
    if (argc == 1) {
        for (int test_idx = 0; test_idx < NUMBER_TESTS; test_idx++) {
            RUN_TEST(tests[test_idx], testNames[test_idx]);
        }
        return 0;
    }
    if (argc != 2) {
        fprintf(stdout, "Usage: member_tests <test index>\n");
        return 0;
    }

    int test_idx = strtol(argv[1], NULL, 10);
    if (test_idx < 1 || test_idx > NUMBER_TESTS) {
        fprintf(stderr, "Invalid test index %d\n", test_idx);
        return 0;
    }

    RUN_TEST(tests[test_idx - 1], testNames[test_idx - 1]);
    return 0;
}