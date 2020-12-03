#ifndef MEMBER_H
#define MEMBER_H

typedef struct Member_t *Member;

Member memberCreate(int member_id, char* member_name);
void memberDestroy(Member member);
Member memberCopy(Member member);
int memberGetId(Member member);
char* memberGetName(Member member);
bool memberEqual(Member member1, Member member2);
#endif