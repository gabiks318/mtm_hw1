#ifndef SET_H_
#define SET_H_

#include <stdbool.h>


/***************************************************************************
 *
 * Generic set ADT
 *
 ***************************************************************************/


/*----------------------------------------------------------------------
                               Type definition
 ----------------------------------------------------------------------*/

typedef struct set* Set;

// Type used for reporting errors in Set
typedef enum {
  SET_SUCCESS,
  SET_OUT_OF_MEMORY,
  SET_FAIL
} SetResult;

// Type of element kept in the set (for readability purposes)
typedef void* SetElement;

/**
 * Type of function used by the set to copy elements into the set.
 * This function should create a new copy of the element sent to it.
 * In case of failure, NULL should be returned.
 **/
typedef SetElement (*SetCopyFunction)(SetElement);

/**
 * Type of function used to free a previously copied element.
 * If NULL is sent to this function, it should do nothing.
 */
typedef void (*SetFreeFunction)(SetElement);

/**
 * Type of function used by the set to compare elements.
 * The function should return a positive integer if a>b, a negative
 * integer if a<b, and zero if a==b.
 */
typedef int (*SetCompareFunction)(SetElement a, SetElement b);

/**
 * The type of function used by the set to convert an element in the set to
 * a string. The function receives an element, a buffer to write the string
 * into, and the size of the buffer.
 *
 * The function should not overflow the buffer, i.e., it should write at most
 * 'bufsize' characters including '\0'.
 *
 * The return value of this function should be the number of characters that
 * would be written to the buffer (excluding '\0') if bufsize were large enough.
 * Specifically, if this function is called with 'buffer'==NULL and 'size'==0,
 * it should simply return this value without writing anything to the buffer.
 * For example, if we want to print "abcde", this function should return 5.
 */
typedef int (*SetToStringFunction)(SetElement, char buffer[], int bufsize);

/**
 * The type of function used by setFilter() to create a filtered copy of the
 * set. The function is called with an element of the set and an additional
 * parameter (param) which is provided by the user of setFilter().
 * The function should return true if the element should be included in the
 * filtered set, or false otherwise.
 */
typedef bool (*SetConditionFuncion)(SetElement, void* param);


/*----------------------------------------------------------------------
                             Creation functions
 ----------------------------------------------------------------------*/

// Return a new empty set, or NULL if out of memory
Set setCreate(SetCopyFunction copy,
              SetFreeFunction destroy,
              SetCompareFunction compare);

// Return a new set identical to the specified set, or NULL if out of memory.
// The new set will use the same functions supplied to the original set.
Set setCopy(Set set);

// Free all the resources associated with the specified set
void setDestroy(Set set);


/*----------------------------------------------------------------------
                              Set operations
 ----------------------------------------------------------------------*/


/**
 * Adds the specified element to the set. If the element already exists in the
 * set, no changes are done to the set. The element is copied and compared
 * using the functions provided during the set's initialization.
 *
 * Parameters:
 *   set     - The set to add the element to
 *   element - The element to be added
 *
 * Returns:
 *   SET_SUCCESS if the element is added successfully
 *   SET_FAIL if the element is already in the set
 *   SET_OUT_OF_MEMORY if a memory allocation error occurs
 */
SetResult setAdd(Set set, SetElement element);


/**
 * Removes an element from the set. The element is freed using the provided
 * free function.
 *
 * Parameters:
 *   set     - The set from which to remove the element
 *   element - The element to be removed
 *
 * Returns:
 *   SET_SUCCESS if the element was found and removed
 *   SET_FAIL if the element was not found in the set
 *
 * Notes:
 *   If an element is successfully removed from the set (i.e., setRemove
 *   returns SET_SUCCESS), any existing iterator becomes invalid and must
 *   be reset using setGetFirst).
 */
SetResult setRemove(Set set, SetElement element);


/**
 * Returns true if 'element' is in the set, or false otherwise. The function
 * uses the provided compare function to determine if an existing element
 * in the set is equal to the specified element.
 */
bool setContains(Set set, SetElement element);


/**
 * Returns the element in the set equal to the specified element, or NULL
 * if no such element is in the set.
 *
 * Note: The returned element is not copied, and therefore it is the user's
 * responsibility not to modify it in any way which may affect its order
 * relative to the other elements in the set (e.g., make it equal to another
 * element).
 */
SetElement setFind(Set set, SetElement element);


/**
 * Returns the number of elements in the set
 */
int setGetSize(Set set);


/**
 * Returns true if the specified set is empty
 */
bool setIsEmpty(Set set);


/**
 * Creates a filtered copy of the specified set.
 *
 * The filtered copy contains all the elements from the original set which
 * satisfy the supplied condition.
 *
 * Parameters:
 *   set       - The set to filter
 *   condition - A function which determines whether an element will be in
 *               the filtered set. For each element x in 'set', x is included
 *               in the filtered set if condition(x,param) returns true.
 *   param     - Passed as the second argument to the condition() function.
 *               Can be used to pass additional information to the condition
 *               function, or set to NULL if not needed.
 *
 * Returns:
 *   A new set containing all the elements from 'set' for which the condition
 *   is 'true', or NULL if a memory allocation error occurs.
 */
Set setFilter(Set set, SetConditionFuncion condition, void* param);


/**
 * Writes a string representation of the set to the supplied buffer, up to
 * 'bufsize' characters. Returns the number of characters that would have
 * been written if the buffer were large enough.
 *
 * If 'size' is 0, no characters are written (and 'buffer' may be NULL).
 *
 * The output string is formatted as "{ x1, x2, ..., xN }", where each xi
 * is the string representation of the i-th element, as returned by the
 * provided toString() function.
 *
 * Parameters:
 *   set      - The set to print
 *   toString - A function which converts a single element from the set to
 *              a string.
 *   buffer   - The character array to write to
 *   bufsize  - Maximal number of characters that can be written to 'buffer',
 *              including the terminating NULL.
 *
 * Returns:
 *   The number of characters that would have been written to the buffer if
 *   it were large enough (excluding the terminating NULL).
 */
int setToString(Set set, SetToStringFunction toString, char buffer[], int size);


/*----------------------------------------------------------------------
                        Operations on pairs of sets
 ----------------------------------------------------------------------*/

/**
 * Returns true if 'set1' is a subset of 'set2', or false otherwise.
 */
bool setIsSubsetOf(Set set1, Set set2);


/**
 * Returns a new set which is the union of 'set1' and 'set2', or NULL if
 * a memory allocation error occurs.
 */
Set setUnion(Set set1, Set set2);


/**
 * Returns a new set which is the intersection of 'set1' and 'set2', or NULL
 * if a memory allocation error occurs.
 */
Set setIntersection(Set set1, Set set2);


/**
 * Checks whether two sets are identical. The sets are identical if they
 * contain the same type of element, and all their elements are equal.
 */
bool setEquals(Set set1, Set set2);


/*----------------------------------------------------------------------
                               Set iteration
 ----------------------------------------------------------------------*/

/**
 * Sets the internal iterator of the set to the first element and returns it.
 * No particular order is guaranteed, except that using setGetFirst() followed
 * by setGetNext() until receiving NULL will always iterate through all
 * elements of the set.
 *
 * The returned element is not copied, and therefore it is the user's
 * responsibility not to modify it in any way which may affect its order
 * relative to the other elements in the set (e.g., make it equal to another
 * element).
 *
 * Returns: The first element in the set, or NULL if the set is empty.
 */
SetElement setGetFirst(Set set);


/**
 * Advances the internal iterator by one and returns the next element in
 * the set.
 *
 * The returned element is not copied, and therefore it is the user's
 * responsibility not to modify it in any way which may affect its order
 * relative to the other elements in the set (e.g., make it equal to another
 * element).
 *
 * Returns: The next element in the set, or NULL if there are no more
 * elements to iterate.
 */
SetElement setGetNext(Set set);


/**
 * Macro to enable simple iteration over the set elements
 */
#define SET_FOREACH(Type, element, set) \
  for(Type element = setGetFirst(set); \
      element != NULL; \
      element = setGetNext(set))

#endif
