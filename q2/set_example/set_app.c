#include "set.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/*----------------------------------------------------------------------
                Functions for operating on string elements
 ----------------------------------------------------------------------*/

static SetElement copyString(SetElement str) {
  char* copy = malloc(strlen((char*)str) + 1);
  if (copy == NULL) {
    return NULL;
  }
  return strcpy(copy, (char*)str);
}

static void freeString(SetElement str) {
  free(str);
}

static int compareString(SetElement str1, SetElement str2) {
  return strcmp((char*)str1, (char*)str2);
}

static int printString(SetElement str, char buffer[], int size) {
  return snprintf(buffer, size, "%s", (char*)str);
}

static bool shorterThan(SetElement str, void* param) {
  return strlen((char*)str) < *(int*)param;
}


/*----------------------------------------------------------------------
                             Helper functions
 ----------------------------------------------------------------------*/

// Utility function for printing the contents of a set
static void printSet(Set set, SetToStringFunction toStringFunc) {

  printf("Set has %d elements: ", setGetSize(set));

  int length = setToString(set, toStringFunc, NULL, 0);
  char* buffer = malloc(length+1);

  if (buffer) {
    setToString(set, toStringFunc, buffer, length+1);
    printf("%s\n", buffer);
    free(buffer);
  }
  else {
    printf("[ Error: not enough memory to print set ]\n");
  }
}

// Print a string set using printSet() above
static inline void printStringSet(Set set) {
  printSet(set, printString);
}


/*----------------------------------------------------------------------
                             Main function
 ----------------------------------------------------------------------*/

int main() {

  Set set1 = setCreate(copyString, freeString, compareString);
  Set set2 = setCreate(copyString, freeString, compareString);

  setAdd(set1, "red");
  setAdd(set1, "blue");
  setAdd(set1, "green");
  setAdd(set1, "yellow");
  setAdd(set1, "magenta");

  setAdd(set2, "black");
  setAdd(set2, "white");
  setAdd(set2, "magenta");

  Set unionSet = setUnion(set1, set2);
  Set intersectionSet = setIntersection(set1, set2);

  printStringSet(set1);
  printStringSet(set2);
  printStringSet(unionSet);
  printStringSet(intersectionSet);

  int sum = 0;
  SET_FOREACH(char*, str, unionSet) {  // compute sum of string lengths
    sum += strlen(str);
  }

  printf("Sum of lengths of all strings in the union is: %d\n", sum);

  int threshold = 6;
  Set filteredSet = setFilter(unionSet, shorterThan, &threshold);
  printStringSet(filteredSet);

  setDestroy(set1);
  setDestroy(set2);
  setDestroy(unionSet);
  setDestroy(intersectionSet);
  setDestroy(filteredSet);

  return 0;
}
