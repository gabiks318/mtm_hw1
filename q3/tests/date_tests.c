#include <stdlib.h>

#include "test_utilities.h"
#include "../date.h"

#define NUMBER_TESTS 4

bool testDateCreateValidDestroy(){
    bool result = true;
    Date date1 = dateCreate(1,1,2021);
    ASSERT_TEST(date1 != NULL, returnDateCreateValidDestroy);
    Date date2 = dateCreate(33 , 1 ,2021);
    ASSERT_TEST(date2 == NULL,destroyDateCreateValidDestroy2);
    Date date3 = dateCreate(-6 , 1 , 2021);
    ASSERT_TEST(date3 == NULL,destroyDateCreateValidDestroy3);
    Date date4 = dateCreate(10 , 14 , 2021);
    ASSERT_TEST(date4 == NULL,destroyDateCreateValidDestroy4);
    Date date5 = dateCreate(10 , -17 , 2021);
    ASSERT_TEST(date5 == NULL,destroyDateCreateValidDestroy5);

    destroyDateCreateValidDestroy5:
        dateDestroy(date5);
    destroyDateCreateValidDestroy4:
        dateDestroy(date1);
    destroyDateCreateValidDestroy3:
        dateDestroy(date3);
    destroyDateCreateValidDestroy2:
        dateDestroy(date2);
    returnDateCreateValidDestroy:
        dateDestroy(date1);
        return result;
}

bool testDateCopyGet(){
    bool result = true;
     // used to initialize the pointers, doesnt compile without it
    int demo = 1, demo2 = 1,demo3 = 1,demo4 = 4,demo5 = 1,demo6 = 6;
    int *day1 = &demo, *day2 = &demo2, *month1 = &demo3, *month2 = &demo4, *year1 = &demo5, *year2 = &demo6;
    Date date1 = dateCreate(1,1,2021);
    ASSERT_TEST(date1 != NULL, returnDateCopyGet);
    Date date2 = dateCopy(date1);
    ASSERT_TEST(date2 != NULL, destroyDateCopyGet);
    ASSERT_TEST(dateGet(date1, day1, month1, year1) == true, destroyDateCopyGet2);
    ASSERT_TEST(dateGet(date2, day1, month2, year2) == true, destroyDateCopyGet2);
    ASSERT_TEST(day1 != NULL, destroyDateCopyGet2);
    ASSERT_TEST(day2 != NULL, destroyDateCopyGet2);
    ASSERT_TEST(month1 != NULL, destroyDateCopyGet2);
    ASSERT_TEST(month2 != NULL, destroyDateCopyGet2);
    ASSERT_TEST(year1 != NULL, destroyDateCopyGet2);
    ASSERT_TEST(year2 != NULL, destroyDateCopyGet2);
    ASSERT_TEST(*day1 == *day2, destroyDateCopyGet2);
    ASSERT_TEST(*month1 == *month2, destroyDateCopyGet2);
    ASSERT_TEST(*year1 == *year2, destroyDateCopyGet2);

    destroyDateCopyGet2:
        dateDestroy(date2);
    destroyDateCopyGet:
        dateDestroy(date1);
    returnDateCopyGet:
        return result;
}

bool testDateCompare(){
    bool result = true;
    Date date1 = dateCreate(20,1,2021);
    Date date2 = dateCreate(10,1,2021);
    Date date3 = dateCreate(1,2,2021);
    Date date4 = dateCreate(29, 11 ,2020);

    ASSERT_TEST(dateCompare(date2, date1) < 0 ,returnDateCompare);
    ASSERT_TEST(dateCompare(date3, date1) > 0 ,returnDateCompare);
    ASSERT_TEST(dateCompare(date2, date1) < 0 ,returnDateCompare);
    ASSERT_TEST(dateCompare(date4, date1) < 0 ,returnDateCompare);
    ASSERT_TEST(dateCompare(date1, date1) == 0 ,returnDateCompare);

    returnDateCompare:
        dateDestroy(date1);
        dateDestroy(date2);
        dateDestroy(date3);
        dateDestroy(date4);
        return result;
}

bool testDateTick(){
    bool result = true;
    int demo = 1, demo2=1, demo3=1; // used to initialize the pointers, doesnt compile without it
    int *day = &demo, *month = &demo2, *year = &demo3;
    Date date = dateCreate(29,11,2020);
    Date date2 = dateCreate(30,12,2020);
    dateGet(date, day, month, year);
    dateTick(date);
    dateGet(date, day, month, year);
    ASSERT_TEST(*day == 30 ,returnDateTick);
    ASSERT_TEST(*month == 11 ,returnDateTick);
    ASSERT_TEST(*year == 2020 ,returnDateTick);

    dateTick(date);
    dateGet(date, day, month, year);
    ASSERT_TEST(*day == 1 ,returnDateTick);
    ASSERT_TEST(*month == 12 ,returnDateTick);
    ASSERT_TEST(*year == 2020 ,returnDateTick);

    dateTick(date2);
    dateGet(date2, day, month, year);
    ASSERT_TEST(*day == 1 ,returnDateTick);
    ASSERT_TEST(*month == 1 ,returnDateTick);
    ASSERT_TEST(*year == 2021 ,returnDateTick);


    returnDateTick:
        dateDestroy(date);
        dateDestroy(date2);
        return result;
}

bool (*tests[])(void) = {
    testDateCreateValidDestroy,
    testDateCopyGet,
    testDateCompare,
    testDateTick
};

const char* testNames[] = {
    "testDateCreateValidDestroy",
    "testDateCopyGet",
    "testDateCompare",
    "testDateTick"
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