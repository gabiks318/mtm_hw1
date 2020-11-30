#include "set.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>


/*----------------------------------------------------------------------
                         Implementation constants
 ----------------------------------------------------------------------*/


// The factor by which the set's memory is expanded
#define EXPAND_RATE 2

// The initial size of the set
#define INITIAL_SIZE 10

// Returned by find() when a value is not in the set
#define ELEMENT_NOT_FOUND -1

/**
 * Struct representing a generic set implemented as an array
 */
struct set {
  SetElement* elements;     // a dynamic array of elements
  int size;                 // the current number of elements in the set
  int maxSize;              // allocated size of the elements array (>=size)
  int iterator;             // current position of the internal iterator

  /* Element operations */
  SetCopyFunction copy;
  SetFreeFunction free;
  SetCompareFunction compare;
};


/*----------------------------------------------------------------------
                             Static helper functions
 ----------------------------------------------------------------------*/


// The minimum of a and b
static inline int min(int a, int b) {
  return a < b ? a : b;
}

// The maximum of a and b
static inline int max(int a, int b) {
  return a > b ? a : b;
}

/**
 * Expands the internal array so it can contain more elements.
 *
 * Returns:
 *   SET_SUCCESS if the operation was successful
 *   SET_OUT_OF_MEMORY if there was not enough memory
 */
static SetResult expand(Set set) {

  int newSize = EXPAND_RATE * set->maxSize;

  SetElement* newElements = realloc(set->elements, newSize*sizeof(SetElement));
  if (newElements == NULL) {
    return SET_OUT_OF_MEMORY;
  }

  set->elements = newElements;
  set->maxSize = newSize;

  return SET_SUCCESS;
}

/**
 * Returns the index of an element in the internal array, or ELEMENT_NOT_FOUND
 * if the element is not in the set.
 */
static int find(Set set, SetElement element) {
  assert(set != NULL);

  for (int i = 0; i < set->size; ++i) {
    if (set->compare(set->elements[i], element) == 0) {
      return i;
    }
  }

  return ELEMENT_NOT_FOUND;
}

/**
 * Attempts to add an element to the set. In case there is not enough memory,
 * destroys the set and returns SET_OUT_OF_MEMORY.
 */
static SetResult addOrDestroy(Set set, SetElement element) {
  SetResult result = setAdd(set, element);
  if (result == SET_OUT_OF_MEMORY) {
    setDestroy(set);
  }
  return result;
}

/**
 * Attempts to add all elements of 'toAdd' to 'set'. In case there is not
 * enough memory, destroys 'set'.
 *
 * Returns:
 * - SET_OUT_OF_MEMORY if a memory allocation error occurs. In this case,
 *   'set' will be destroyed.
 * - SET_SUCCESS if all elements were inserted without error (some may
 *   have already existed in the set).
 */
static SetResult addAllOrDestroy(Set set, Set toAdd) {
  for (int i = 0; i < toAdd->size; ++i) {
    if (addOrDestroy(set, toAdd->elements[i]) == SET_OUT_OF_MEMORY) {
      return SET_OUT_OF_MEMORY;
    }
  }
  return SET_SUCCESS;
}

/**
 * Create a new empty set that has the same element type as the specified set.
 * The returned set uses the same element operations as the supplied set, but
 * contains no elements.
 */
static Set setCreateSameType(Set set)
{
  return setCreate(set->copy, set->free, set->compare);
}

/**
 * Determines if set1 and set2 contain the same type of element. Two sets
 * are considered to have the same element type only if all three element
 * operations (copy, free and compare) are the same function.
 */
static bool sameTypeSets(Set set1, Set set2) {
  return set1->copy == set2->copy &&
         set1->free == set2->free &&
         set1->compare == set2->compare;
}


/*----------------------------------------------------------------------
                             Creation functions
 ----------------------------------------------------------------------*/

// Create an empty set
Set setCreate(SetCopyFunction copy,
    SetFreeFunction destroy,
    SetCompareFunction compare)
{
  assert(copy != NULL && destroy != NULL && compare != NULL);

  Set set = malloc(sizeof(*set));
  if (set == NULL) {
    return NULL;
  }

  set->elements = malloc(INITIAL_SIZE * sizeof(SetElement));
  if (set->elements == NULL) {
    free(set);
    return NULL;
  }

  set->size = 0;
  set->maxSize = INITIAL_SIZE;
  set->iterator = 0;

  set->copy = copy;
  set->free = destroy;
  set->compare = compare;

  return set;
}

// Duplicate an existing set
Set setCopy(Set set) {
  if (set == NULL) {
    return NULL;
  }

  Set newSet = setCreate(set->copy, set->free, set->compare);
  if (newSet == NULL) {
    return NULL;
  }
  if (addAllOrDestroy(newSet, set) == SET_OUT_OF_MEMORY) {
    return NULL;
  }

  newSet->iterator = set->iterator;
  return newSet;
}

// Destroy a set
void setDestroy(Set set) {
  if (set == NULL) {
    return;
  }

  while(!setIsEmpty(set)) {
    setRemove(set, setGetFirst(set));
  }

  free(set->elements);
  free(set);
}


/*----------------------------------------------------------------------
                              Set operations
 ----------------------------------------------------------------------*/

// Add an element to the set
SetResult setAdd(Set set, SetElement element) {
  assert(set != NULL);

  if (element == NULL || setContains(set, element)) {
    return SET_FAIL;
  }

  if (set->size == set->maxSize) {
    if (expand(set) == SET_OUT_OF_MEMORY) {
      return SET_OUT_OF_MEMORY;
    }
  }

  SetElement copy = set->copy(element);
  if (copy == NULL) {
    return SET_OUT_OF_MEMORY;
  }

  set->elements[set->size++] = copy;
  return SET_SUCCESS;
}

// Remove an element from the set
SetResult setRemove(Set set, SetElement element) {
  assert(set != NULL);

  int index = find(set, element);
  if (index == ELEMENT_NOT_FOUND) {
    return SET_FAIL;
  }

  set->free(set->elements[index]);
  set->elements[index] = set->elements[set->size - 1];

  set->size--;
  set->iterator = 0;

  return SET_SUCCESS;
}

// Check if an element is in the set
bool setContains(Set set, SetElement element) {
  assert(set != NULL);
  return find(set, element) != ELEMENT_NOT_FOUND;
}

// Find an element is in the set
SetElement setFind(Set set, SetElement element) {
  assert(set != NULL);
  int index = find(set, element);
  return index == ELEMENT_NOT_FOUND ? NULL : set->elements[index];
}

// Return the size of the set
int setGetSize(Set set) {
  assert(set != NULL);
  return set->size;
}

// Check if the set is empty
bool setIsEmpty(Set set) {
  return setGetSize(set) == 0;
}

// Filter the set
Set setFilter(Set set, SetConditionFuncion condition, void* param) {
  assert(set != NULL);

  Set newSet = setCreateSameType(set);
  if (newSet == NULL) {
    return NULL;
  }

  for(int i = 0; i < set->size; ++i) {
    if (!condition(set->elements[i], param)) {
      continue;
    }
    if (addOrDestroy(newSet, set->elements[i]) == SET_OUT_OF_MEMORY) {
      return NULL;
    }
  }

  return newSet;
}

/**
 * Helper macro, executes a print command and then advances the buffer in
 * memory and reduces its size by the number of characters written. Also,
 * the macro increments chars_total by the value returns from the print command
 * (which should be the number of characters that would be written if bufsize
 * were large enough).
 */
#define WRITE_AND_ADVANCE(buffer, bufsize, printCommand, chars_total) \
do { \
  int numtotal = printCommand; \
  int numwritten = max(min(numtotal, bufsize-1),0); \
  buffer += numwritten; \
  bufsize -= numwritten; \
  chars_total += numtotal; \
} while(0)

int setToString(Set set, SetToStringFunction toString,
                char buffer[], int size)
{
  int chars_total = 0;
  WRITE_AND_ADVANCE(buffer, size, snprintf(buffer,size,"{ "), chars_total);

  bool first = true;
  for (int i = 0; i < set->size; i++) {
    if (!first) {
      WRITE_AND_ADVANCE(buffer, size, snprintf(buffer,size,", "), chars_total);
    }
    WRITE_AND_ADVANCE(buffer, size, toString(set->elements[i],buffer,size), chars_total);
    first = false;
  }

  WRITE_AND_ADVANCE(buffer, size, snprintf(buffer,size," }"), chars_total);
  return chars_total;
}


/*----------------------------------------------------------------------
                        Operations on pairs of sets
 ----------------------------------------------------------------------*/

// Check if set1 is a subset of set2
bool setIsSubsetOf(Set set1, Set set2) {
  assert(set1 != NULL && set2 != NULL);
  for(int i = 0; i < set1->size; ++i) {
    if (!setContains(set2, set1->elements[i])) {
      return false;
    }
  }
  return true;
}

// Check if set1 and set2 are identical
bool setEquals(Set set1, Set set2) {

  assert(set1!=NULL && set2!=NULL);

  if (!sameTypeSets(set1, set2)) {
    return false;
  }

  return setIsSubsetOf(set1, set2) && setIsSubsetOf(set2, set1);
}

// Unite set1 and set2
Set setUnion(Set set1, Set set2) {

  assert(set1!=NULL && set2!=NULL);
  assert(sameTypeSets(set1, set2));

  Set newSet = setCopy(set1);
  if (newSet == NULL) {
    return NULL;
  }

  if (addAllOrDestroy(newSet, set2) == SET_OUT_OF_MEMORY) {
    return NULL;
  }

  return newSet;
}

// Intersect set1 and set2
Set setIntersection(Set set1, Set set2) {

  assert(set1!=NULL && set2!=NULL);
  assert(sameTypeSets(set1, set2));

  Set newSet = setCreateSameType(set1);
  if (newSet == NULL) {
    return NULL;
  }

  for(int i = 0; i < set1->size; i++) {
    if (!setContains(set2, set1->elements[i])) {
      continue;
    }
    if (addOrDestroy(newSet, set1->elements[i]) == SET_OUT_OF_MEMORY) {
      return NULL;
    }
  }

  return newSet;
}


/*----------------------------------------------------------------------
                               Set iteration
 ----------------------------------------------------------------------*/

// Get first set element
SetElement setGetFirst(Set set) {
  assert(set != NULL);
  set->iterator = 0;
  return setGetNext(set);
}

// Get next set element
SetElement setGetNext(Set set) {
  assert(set != NULL);
  if (set->iterator >= set->size) {
    return NULL;
  }
  return set->elements[set->iterator++];
}
