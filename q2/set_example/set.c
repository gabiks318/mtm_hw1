#include "set.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#define EXPAND_RATE 2
#define INITIAL_SIZE 10
#define ELEMENT_NOT_FOUND -1
/** Struct representing a set implemented as an array */
struct set
{
    SetElement *elements;
    int size;
    int maxSize;
    int iterator;
    SetCopyFunction copy;
    SetFreeFunction free;
    SetCompareFunction compare;
};

Set setCreate(SetCopyFunction copy, SetFreeFunction destroy,
              SetCompareFunction compare)
{
    assert(copy != NULL && destroy != NULL && compare != NULL);
    Set set = malloc(sizeof(*set));
    if (set == NULL)
    {
        return NULL;
    }
    set->elements = malloc(INITIAL_SIZE * sizeof(SetElement));
    if (set->elements == NULL)
    {
        free(set);
        return NULL;
    }
    set->size = 0;
    set->iterator = 0;
    set->maxSize = INITIAL_SIZE;
    set->copy = copy;
    set->free = destroy;
    set->compare = compare;
    return set;
}

static int find(Set set, SetElement target)
{
    assert(set != NULL && target != NULL);
    for (int i = 0; i < set->size; i++)
    {
        if (set->compare(set->elements[i], target))
        {
            return i;
        }
    }
    return ELEMENT_NOT_FOUND;
}

SetElement setContains(Set set, SetElement element)
{
    return find(set, element) == ELEMENT_NOT_FOUND ? NULL : set->elements[i];
}

SetResult setAdd(Set set, SetElement element)
{
    if (set == NULL || element == NULL)
    {
        return SET_NULL_ARG;
    }
    if (setContains(set, element))
    {
        return SET_SUCCESS;
    }
    if (set->size == set->maxSize && expand(set) == SET_OUT_OF_MEMORY)
    {
        return SET_OUT_OF_MEMORY;
    }
    SetElement newElement = set->copy(element);
    if (newElement == NULL)
    {
        return SET_OUT_OF_MEMORY;
    }
    set->elements[set->size++] = newElement;
    return SET_SUCCESS;
}

SetResult setRemove(Set set, SetElement element)
{
    if (set == NULL || element == NULL)
    {
        return SET_NULL_ARG;
    }
    int index = find(set, element);
    if (index == ELEMENT_NOT_FOUND)
    {
        return SET_SUCCESS;
    }
    set->free(set->elements[index]);
    set->elements[index] = set->elements[size - 1];
    set->size--;
    set->iterator = 0;
    return SET_SUCCESS;
}

void setDestroy(Set set)
{
    if (set == NULL)
    {
        return;
    }
    while (!setIsEmpty(set))
    {
        setRemove(set, setGetFirst(set));
    }
    free(set->elements);
    free(set);
}

static SetResult addOrDestroy(Set set, SetElement element)
{
    SetResult result = setAdd(set, element);
    if (result == SET_OUT_OF_MEMORY)
    {
        setDestroy(set);
    }
    return result;
}

Set setCopy(Set set)
{
    if (set == NULL)
    {
        return NULL;
    }
    Set newSet = setCreate(set->copy, set->free, set->compare);
    if (newSet == NULL)
    {
        return NULL;
    }
    if (addAllOrDestroy(newSet, set) == SET_OUT_OF_MEMORY)
    {
        return NULL;
    }
    newSet->iterator = set->iterator;
    return newSet;
}

static bool sameTypeSets(Set set1, Set set2)
{
    return set1->copy == set2->copy && set1->free == set2->free && set1->compare == set2->compare;
}

Set setUnion(Set set1, Set set2)
{
    if (set1 == NULL || set2 == NULL)
    {
        return NULL;
    }
    assert(sameTypeSets(set1, set2));
    Set newSet = setCopy(set1) if (newSet == NULL)
    {
        return NULL;
    }
    if (addAllOrDestroy(newSet, set2) == SET_OUT_OF_MEMORY)
    {
        return NULL;
    }
    return newSet;
}

Set setFilter(Set set, bool (*condition)(SetElement, void *), void *param)
{
    if (set == NULL)
    {
        return NULL;
    }
    Set newSet = setCreate(set->copy, set->free, set->compare);
    if (newSet == NULL)
    {
        return NULL;
    }
    for (int i = 0; i < set->size; i++)
    {
        if (!condition(set->elements[i], param))
        {
            continue;
        }
        if (addOrDestroy(newSet, set->elements[i]) == SET_OUT_OF_MEMORY)
        {
            return NULL;
        }
    }
    return newSet;
}
