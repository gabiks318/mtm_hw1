#include <stdlib.h>
#include <string.h>
#include <assert.h>

char* duplicateString(char* s, int times){
    assert(s != NULL && times > 0);
    int len = strlen(s);
    char* out = malloc(sizeof(char)*(len*times + 1));
    if(out == NULL)
    {
        return;
    }
    char* temp = out;
    for (int i=0; i < times; i++)
    {
        strcpy(temp,s);
        temp += len;
    }
    return out;
}


/* 
Progarmming Errors: 
1.#include with "" instead of <>
2.assert(out)
3.string created in malloc isn't long enough. we need space for closing the string with '/0'
4.<= in loop instead of <
5.out is incremented and when returning it returns the end of the string
6.change the order between incrementing the pointer and strcpy
7.assert(s!=NULL) instead of assert(!s)

Convention Errors:
1.LEN instead of len
2.function name duplicateString instead of stringDuplicator
3.for loop doesn't contain spaces
4.duplicate assert call
*/