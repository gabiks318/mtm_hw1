#include <stdlib.h>
#include <string.h>

#include "test_utilities.h"
#include "../member_list.h"

#define NUMBER_TESTS 4

bool testMemberListCreateDestroy(){
    bool result = true;
    int member_id = 1;
    char* member_name = "gabi";

    Member member_1 = memberCreate(member_id, member_name);
    ASSERT_TEST(member_1 != NULL,returnMemberCreateDestroy);
    Node node = nodeCreate(member_1);
    ASSERT_TEST(node != NULL, destroyMemberListCreateDestroy);
    ASSERT_TEST(nodeGetMember(node) == member_1, destroyMemberListCreateDestroy);
    ASSERT_TEST(nodeGetNext(node) == NULL, destroyMemberListCreateDestroy);
    ASSERT_TEST(member_1 != NULL, destroyMemberListCreateDestroy);

    destroyMemberListCreateDestroy:
        memberDestroy(member_1);
        nodeDestroy(node);
    returnMemberCreateDestroy:
        return result;
}

bool testMemberListAddGetNext(){
    bool result = true;
    int member_id = 1;
    char* member_name = "gabi";

    Member member_1 = memberCreate(member_id, member_name);
    ASSERT_TEST(member_1 != NULL,returnMemberAddGetNext);
    Node node_1 = nodeCreate(member_1);
    ASSERT_TEST(node_1 != NULL, destroyMemberListAddGetNext);
    nodeAddNext(node_1, member_1);
    Node node_2 = nodeGetNext(node_1);
    ASSERT_TEST(node_2 != NULL, destroyMemberListAddGetNext2);
    ASSERT_TEST(node_2 != node_1, destroyMemberListAddGetNext2);
    nodeAddNext(node_1, member_1);
    Node node_3 = nodeGetNext(node_2);
    ASSERT_TEST(node_3 != NULL, destroyMemberListAddGetNext3);
    ASSERT_TEST(node_2 != node_3, destroyMemberListAddGetNext3);

    destroyMemberListAddGetNext3:
        nodeDestroy(node_3);
    destroyMemberListAddGetNext2:
        nodeDestroy(node_2);
    destroyMemberListAddGetNext:
        nodeDestroy(node_1);
        memberDestroy(member_1);
    returnMemberAddGetNext:
        return result;
}

bool testMemberListDestroyAll(){
    bool result = true;
    int member_id = 1;
    char* member_name = "gabi";

    Member member_1 = memberCreate(member_id, member_name);
    ASSERT_TEST(member_1 != NULL,returnMemberListDestroyAll);
    Node node_1 = nodeCreate(member_1);
    ASSERT_TEST(node_1 != NULL, destroyMemberListDestroyAll);
    nodeAddNext(node_1, member_1);
    Node node_2 = nodeGetNext(node_1);
    ASSERT_TEST(node_2 != NULL, destroyMemberListDestroyAll2);
    nodeAddNext(node_1, member_1);
    Node node_3 = nodeGetNext(node_2);
    ASSERT_TEST(node_3 != NULL, destroyMemberListDestroyAll3);
    nodeDestroyAll(node_1);
    
    destroyMemberListDestroyAll3:
        nodeDestroy(node_3);
    destroyMemberListDestroyAll2:
        nodeDestroy(node_2);
    destroyMemberListDestroyAll:
        nodeDestroy(node_1);        
    returnMemberListDestroyAll:
        memberDestroy(member_1);
        return result;
}

bool testMemberListCopy(){
    bool result = true;
    int member_id = 1;
    char* member_name_1 = "gabi";
    char* member_name_2 = "yan";
    char* member_name_3 = "borat";
    Member member_1 = memberCreate(member_id, member_name_1);
    Member member_2 = memberCreate(member_id + 1, member_name_2);
    Member member_3 = memberCreate(member_id + 2, member_name_3);

    Node node = nodeCreate(member_1);
    nodeAddNext(node, member_2);
    nodeAddNext(node, member_3);
    Node copy_node = nodeCopyAll(node);
    ASSERT_TEST(copy_node != NULL, returnMemberListCopy);
    ASSERT_TEST(nodeGetMember(copy_node) == member_1 , returnMemberListCopy);
    Node copy_node_2 = nodeGetNext(copy_node);
    ASSERT_TEST(copy_node_2 != NULL, returnMemberListCopy);
    ASSERT_TEST(nodeGetMember(copy_node_2) == member_2 , returnMemberListCopy);
    Node copy_node_3 = nodeGetNext(copy_node_2);
    ASSERT_TEST(copy_node_3 != NULL, returnMemberListCopy);
    ASSERT_TEST(nodeGetMember(copy_node_3) == member_3 , returnMemberListCopy);

    returnMemberListCopy:
        nodeDestroyAll(node);
        nodeDestroyAll(copy_node);
        memberDestroy(member_1);
        memberDestroy(member_2);
        memberDestroy(member_3);
        return result;
}

bool (*tests[])(void) = {
    testMemberListCreateDestroy,
    testMemberListAddGetNext,
    testMemberListDestroyAll,
    testMemberListCopy
};

const char* testNames[] = {
    "testMemberListCreateDestroy",
    "testMemberListAddGetNext",
    "testMemberListDestroyAll",
    "testMemberListCopy"
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