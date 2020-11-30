#include <stdlib.h>
#include "tests/test_utilities.h"
#include "./priority_queue.h"


#define NUMBER_TESTS 1

static PQElementPriority copyIntGeneric(PQElementPriority n) {
    if (!n) {
        return NULL;
    }
    int *copy = malloc(sizeof(*copy));
    if (!copy) {
        return NULL;
    }
    *copy = *(int *) n;
    return copy;
}

static void freeIntGeneric(PQElementPriority n) {
    free(n);
}

static int compareIntsGeneric(PQElementPriority n1, PQElementPriority n2) {
    return (*(int *) n1 - *(int *) n2);
}

static bool equalIntsGeneric(PQElementPriority n1, PQElementPriority n2) {
    return *(int *) n1 == *(int *) n2;
}

bool testPQCreateDestroy() {
    bool result = true;

    PriorityQueue pq = pqCreate(copyIntGeneric, freeIntGeneric, equalIntsGeneric, copyIntGeneric, freeIntGeneric, compareIntsGeneric);
    ASSERT_TEST(pq != NULL, returnPQCreateDestroy);
    ASSERT_TEST(pqGetSize(pq) == 0,destroyPQCreateDestroy);
    ASSERT_TEST(pqGetFirst(pq) == NULL,destroyPQCreateDestroy);

destroyPQCreateDestroy:
    pqDestroy(pq);
returnPQCreateDestroy:
    return result;
}

bool testPQInsertAndSize() {
    bool result = true;
    PriorityQueue pq = pqCreate(copyIntGeneric, freeIntGeneric, equalIntsGeneric, copyIntGeneric, freeIntGeneric, compareIntsGeneric);
    ASSERT_TEST(pqGetSize(pq) == 0,destroyPQInsertAndSize);
    int to_add = 1;
    ASSERT_TEST(pqInsert(pq, &to_add, &to_add) == PQ_SUCCESS,destroyPQInsertAndSize);
    ASSERT_TEST(pqGetSize(pq) == 1,destroyPQInsertAndSize);

destroyPQInsertAndSize:
    pqDestroy(pq);
    return result;
}

bool testPQGetFirst() {
    bool result = true;
    PriorityQueue pq = pqCreate(copyIntGeneric, freeIntGeneric, equalIntsGeneric, copyIntGeneric, freeIntGeneric, compareIntsGeneric);
    ASSERT_TEST(pqGetSize(pq) == 0, destroyPQGetFirst);
    int to_add = 1;
    ASSERT_TEST(pqInsert(pq, &to_add, &to_add) == PQ_SUCCESS, destroyPQGetFirst);
    int* first_value = pqGetFirst(pq);
    ASSERT_TEST(first_value != NULL, destroyPQGetFirst);
    ASSERT_TEST(*first_value == to_add, destroyPQGetFirst);

destroyPQGetFirst:
    pqDestroy(pq);
    return result;
}

bool testPQIterator() {
    
    bool result = true;
    PriorityQueue pq = pqCreate(copyIntGeneric, freeIntGeneric, equalIntsGeneric, copyIntGeneric, freeIntGeneric, compareIntsGeneric);

    int max_value = 10;

    
    for(int i=0; i< max_value; i++){
        ASSERT_TEST(pqInsert(pq, &i, &i) == PQ_SUCCESS, destroyPQIterator);
    }
    
    int i = 0;
    PQ_FOREACH(int*, iter, pq) {

        if (i != max_value) {
            ASSERT_TEST(iter != NULL,destroyPQIterator);
        } else {
            ASSERT_TEST(iter == NULL,destroyPQIterator);
        }
        i++;
    }

destroyPQIterator:
    pqDestroy(pq);
    return result;
}

bool testPQCopyMultipleElement() {
    bool result = true;
    PQ pq = getMultipleElementPQ();
    PQ new_pq = pqCopy(pq);
    ASSERT_TEST(new_pq != NULL, destroy);

    int *new_pq_current_elem = pqGetFirst(new_pq);
    PQ_FOREACH(int *, pq_current_elem, pq) {
        ASSERT_TEST(compareIntsGeneric(pq_current_elem, new_pq_current_elem) == 0, destroy);
        ASSERT_TEST(pq_current_elem != new_pq_current_elem,
                    destroy); // Copy should create a new copy so the pointers shouldn't be pointing to the same address
        new_pq_current_elem = pqGetNext(new_pq);
    }

    destroy:
    pqDestroy(pq);
    pqDestroy(new_pq);
    return result;
}

bool (*tests[]) (void) = {
        testPQCreateDestroy,
        testPQInsertAndSize,
        testPQGetFirst,
        testPQIterator
};

const char* testNames[] = {
        "testPQCreateDestroy",
        "testPQInsertAndSize",
        "testPQGetFirst",
        "testPQIterator"
};

int main(int argc, char *argv[]) {
    if (argc == 1) {
        for (int test_idx = 0; test_idx < NUMBER_TESTS; test_idx++) {
            RUN_TEST(tests[test_idx], testNames[test_idx]);
        }
        return 0;
    }
    if (argc != 2) {
        fprintf(stdout, "Usage: priority_queue_tests <test index>\n");
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


