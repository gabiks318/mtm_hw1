#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string.h>

#include "test_utilities.h"
#include "../member.h"

#define NUMBER_TESTS 1

bool testMemberCopyDestroy(){
    bool result = true;
    int member_id = 1;
    char* member_name = "gabi";

    Member member_1 = memberCreate(member_id, member_name);
    Member member_2 = memberCopy(member_1);
    ASSERT_TEST(memberGetId(member_1) == memberGetId(member_2),destroyMemberCreateDestroy);
    ASSERT_TEST(strcmp(memberGetName(member_1),memberGetName(member_2)) == 0 ,destroyMemberCreateDestroy);

    destroyMemberCreateDestroy:
        memberDestroy(member_1);
        memberDestroy(member_2);
        return result;
}

bool (*tests[])(void) = {
    testMemberCopyDestroy
};

const char* testNames[] = {
    "testMemberCopyDestroy"
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