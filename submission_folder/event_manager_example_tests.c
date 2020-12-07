#include "test_utilities.h"
#include "event_manager.h"
#include <stdlib.h>
#include <string.h>


#define DEBUG false

static void debugPrint(char* text){
    if(DEBUG){
        printf(text);
        printf("\n");
    }
}

#define NUMBER_TESTS 4

bool testEventManagerCreateDestroy() {
    bool result = true;
    Date start_date = dateCreate(1,12,2020); 
    EventManager em = createEventManager(start_date);

    ASSERT_TEST(em != NULL, destroyEventManagerCreateDestroy);
    ASSERT_TEST(emGetEventsAmount(em) == 0, destroyEventManagerCreateDestroy);
    ASSERT_TEST(emGetNextEvent(em) == NULL, destroyEventManagerCreateDestroy);

destroyEventManagerCreateDestroy:
    destroyEventManager(em);
    dateDestroy(start_date);
    return result;

}

bool testAddEventByDiffAndSize() {
    bool result = true;

    Date start_date = dateCreate(1,12,2020);
    EventManager em = createEventManager(start_date);

    char* event_name = "event1";
    ASSERT_TEST(emAddEventByDiff(em, event_name, 2, 1) == EM_SUCCESS, destroyAddEventByDiffAndSize);
    ASSERT_TEST(emGetEventsAmount(em) == 1, destroyAddEventByDiffAndSize);
    ASSERT_TEST(strcmp(event_name, emGetNextEvent(em)) == 0, destroyAddEventByDiffAndSize);

destroyAddEventByDiffAndSize:
    dateDestroy(start_date);
    destroyEventManager(em);
    return result;
}

bool testEMTick() {
bool result = true;

    Date start_date = dateCreate(1,12,2020);
    EventManager em = createEventManager(start_date);

    char* event_name = "event1";
    ASSERT_TEST(emAddEventByDiff(em, event_name, 1, 1) == EM_SUCCESS, destroyEMTick);

    ASSERT_TEST(emGetEventsAmount(em) == 1, destroyEMTick);
    ASSERT_TEST(emTick(em, 2) == EM_SUCCESS, destroyEMTick);
    ASSERT_TEST(emGetEventsAmount(em) == 0, destroyEMTick);
destroyEMTick:
    dateDestroy(start_date);
    destroyEventManager(em);
    return result;
}

bool testBigEventManager() {
    bool result = true;
    Date start_date = dateCreate(1,12,2020);
    EventManager em = createEventManager(start_date);
    char* event_name1 = "event1";
    char* event_name2 = "event2";
    char* event_name3 = "event3";
    char* event_name4 = "event4";
    char* event_name5 = "event5";
    Date event_date1 = dateCreate(1,12,2020);
    Date event_date2 = dateCreate(5,12,2020);
    Date event_date3 = dateCreate(10,12,2020);
    Date event_date4 = dateCreate(29,12,2021);
    Date event_date5 = dateCreate(1,11,2020);
    ASSERT_TEST(emAddEventByDate(em, event_name1, event_date1, 1) == EM_SUCCESS,destroyDates);
    ASSERT_TEST(emAddEventByDate(em, event_name2, event_date2, 2) == EM_SUCCESS,destroyDates);
    ASSERT_TEST(emAddEventByDate(em, event_name3, event_date3, 3) == EM_SUCCESS,destroyDates);
    ASSERT_TEST(emAddEventByDate(em, event_name4, event_date4, 4) == EM_SUCCESS,destroyDates);
    ASSERT_TEST(emAddEventByDate(em, event_name5, event_date5, 5) == EM_INVALID_DATE,destroyDates);
    ASSERT_TEST(emGetEventsAmount(em) == 4, destroyDates);
    Date new_date_to_event_4 = dateCreate(27,12,2020);
    debugPrint("1\n");
    ASSERT_TEST(emChangeEventDate(em, 8, new_date_to_event_4) == EM_EVENT_ID_NOT_EXISTS, destroyDates2);
        debugPrint("1");

    ASSERT_TEST(emChangeEventDate(em, 4, new_date_to_event_4) == EM_SUCCESS, destroyDates2);
        debugPrint("2");

    char* event_name_same_as_1 = "event1";
    ASSERT_TEST(emAddEventByDate(em, event_name_same_as_1, event_date3, 6) == EM_SUCCESS,destroyDates2);
        debugPrint("3");

    ASSERT_TEST(emChangeEventDate(em, 6, event_date1) == EM_EVENT_ALREADY_EXISTS,destroyDates2);
        debugPrint("4");

    ASSERT_TEST(emGetEventsAmount(em) == 5, destroyDates2);
    ASSERT_TEST(emRemoveEvent(em, 6) == EM_SUCCESS,destroyDates2);
        debugPrint("5");

    ASSERT_TEST(emGetEventsAmount(em) == 4, destroyDates2);

    ASSERT_TEST(strcmp(emGetNextEvent(em), event_name1) == 0, destroyDates2);
        debugPrint("6");

    //adding 5 members
    ASSERT_TEST(emAddMember(em, "yan1", 1) == EM_SUCCESS, destroyDates2);
    ASSERT_TEST(emAddMember(em, "yan2", 2) == EM_SUCCESS, destroyDates2);
    ASSERT_TEST(emAddMember(em, "yan3", 3) == EM_SUCCESS, destroyDates2);
    ASSERT_TEST(emAddMember(em, "yan4", 4) == EM_SUCCESS, destroyDates2);
    ASSERT_TEST(emAddMember(em, "yan5", 5) == EM_SUCCESS, destroyDates2);
    ASSERT_TEST(emAddMember(em, "yan6", 5) == EM_MEMBER_ID_ALREADY_EXISTS, destroyDates2);

    /*  member 1- events 1,2,3,4
        member 2 - events 2,3
        member 3 - events 2
        member 4 - events 2
        member 5 - events 2,4*/
    ASSERT_TEST(emAddMemberToEvent(em, 1 , 1) == EM_SUCCESS, destroyDates2);
    ASSERT_TEST(emAddMemberToEvent(em, 1 , 2) == EM_SUCCESS, destroyDates2);
    ASSERT_TEST(emAddMemberToEvent(em, 1 , 3) == EM_SUCCESS, destroyDates2);
    ASSERT_TEST(emAddMemberToEvent(em, 1 , 4) == EM_SUCCESS, destroyDates2);
    ASSERT_TEST(emAddMemberToEvent(em, 2 , 2) == EM_SUCCESS, destroyDates2);
    ASSERT_TEST(emAddMemberToEvent(em, 2 , 3) == EM_SUCCESS, destroyDates2);
    ASSERT_TEST(emAddMemberToEvent(em, 3 , 2) == EM_SUCCESS, destroyDates2);
    ASSERT_TEST(emAddMemberToEvent(em, 4 , 2) == EM_SUCCESS, destroyDates2);
    ASSERT_TEST(emAddMemberToEvent(em, 5 , 1) == EM_SUCCESS, destroyDates2);
    ASSERT_TEST(emAddMemberToEvent(em, 5 , 2) == EM_SUCCESS, destroyDates2);
    ASSERT_TEST(emAddMemberToEvent(em, 5 , 4) == EM_SUCCESS, destroyDates2);

    ASSERT_TEST(emAddMemberToEvent(em, 2 , 10) == EM_EVENT_ID_NOT_EXISTS, destroyDates2);
    ASSERT_TEST(emAddMemberToEvent(em, 8 , 1) == EM_MEMBER_ID_NOT_EXISTS, destroyDates2);
    ASSERT_TEST(emAddMemberToEvent(em, 1 , 1) == EM_EVENT_AND_MEMBER_ALREADY_LINKED, destroyDates2);
    ASSERT_TEST(emRemoveMemberFromEvent(em, 1 , 1) == EM_SUCCESS, destroyDates2);
    ASSERT_TEST(emAddMemberToEvent(em, 1 , 1) == EM_SUCCESS, destroyDates2);
    ASSERT_TEST(emRemoveMemberFromEvent(em, 2 , 10) == EM_EVENT_ID_NOT_EXISTS, destroyDates2);
    ASSERT_TEST(emRemoveMemberFromEvent(em, 8 , 1) == EM_MEMBER_ID_NOT_EXISTS, destroyDates2);
    ASSERT_TEST(emRemoveMemberFromEvent(em, 2 , 1) == EM_EVENT_AND_MEMBER_NOT_LINKED, destroyDates2);
   
    emPrintAllEvents(em, "fileyantest.txt");

destroyDates2:
    dateDestroy(new_date_to_event_4);
destroyDates:
    dateDestroy(event_date1);
    dateDestroy(event_date2);
    dateDestroy(event_date3);
    dateDestroy(event_date4);
    dateDestroy(event_date5);

    dateDestroy(start_date);
    destroyEventManager(em);
    return result;
}


bool (*tests[]) (void) = {
        testEventManagerCreateDestroy,
        testAddEventByDiffAndSize,
        testEMTick,
        testBigEventManager
};

const char* testNames[] = {
        "testEventManagerCreateDestroy",
        "testAddEventByDiffAndSize",
        "testEMTick",
        "testBigEventManager"
};

int main(int argc, char *argv[]) {
    if (argc == 1) {
        for (int test_idx = 0; test_idx < NUMBER_TESTS; test_idx++) {
            RUN_TEST(tests[test_idx], testNames[test_idx]);
        }
        return 0;
    }
    if (argc != 2) {
        fprintf(stdout, "Usage: event_manager_tests <test index>\n");
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
