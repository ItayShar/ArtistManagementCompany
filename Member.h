#pragma once
#include "General.h"
typedef enum {eSinger, eGuitarist, eDrummer, eBass, ekeyboardist, eNumOfRoles} eRole;
static const char* roleStr[eNumOfRoles] = { "Singer", "Guitarist", "Drummer", "Bass", "keyboardist" };

typedef struct {
	char* name;
	eRole role;
} Member;

int initMember(Member* pMember);
int initSoloMember(Member* pMember);
int writeMemberToTextFile(const Member* pMember, FILE* f);
void printMember(const Member* pMember);
void freeMember(Member* pMember);
Member* readMemberFromTextFile(FILE* f);
Member* loadMemberFromBinFile(FILE* f);
int saveMemberToBinFile(const Member* pMember, FILE* f);
