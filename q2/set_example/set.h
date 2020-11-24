#ifndef SET_H_
#define SET_H_
#include <stdbool.h>
typedef struct set *Set;
typedef enum
{
    SET_SUCCESS,
    SET_OUT_OF_MEMORY,
    SET_NULL_ARG
} SetResult;

typedef void *SetElement;
typedef SetElement (*SetCopyFunction)(SetElement);
typedef void (*SetFreeFunction)(SetElement);
typedef bool (*SetCompareFunction)(SetElement, SetElement);
typedef char *(*SetToStringFunction)(SetElement);
typedef bool (*SetConditionFunction)(SetElement, void *key);
Set setCreate(SetCopyFunction copy, SetFreeFunction destroy,
              SetCompareFunction compare);
Set setCopy(Set set);
void setDestroy(Set set);
SetResult setAdd(Set set, SetElement element);
SetResult setRemove(Set set, SetElement element);
SetElement setContains(Set set, SetElement element);
int setGetSize(Set set);
Set setUnion(Set set1, Set set2);
Set setIntersection(Set set1, Set set2);
Set setFilter(Set set, SetConditionFunction condition, void *key);
void setPrint(Set set, SetToStringFunction toString);
SetElement setGetFirst(Set set);
SetElement setGetNext(Set set);
#define SET_FOREACH(Type, element, set)   \
    for (Type element = setGetFirst(set); \
         element != NULL;                 \
         element = setGetNext(set))
#endif
