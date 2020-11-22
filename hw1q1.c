#include <stdlib.h>
#include <string.h>
#include <assert.h>

har* stringDuplicator(char* s, int times){
    assert(!s);
    assert(times > 0);
    int LEN = strlen(s);
    char* out = malloc(LEN*times);
    assert(out);
    for (int i=0; i<=times; i++){
    out = out + LEN;
    strcpy(out,s);
    }
    return out;
}
