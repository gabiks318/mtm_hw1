#ifndef MEMBER_H
#define MEMBER_H

typedef struct Member_t *Member;

Member memberCreate(int member_id, char* member_name);
void memberDestroy(Member member);
int memberGetId(Member member);
char* memberGetName(Member member);
#endif