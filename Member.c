#include "Member.h"

int initMember(Member* pMember)
{
	char* name = getName("Enter member name");
	if (!name) return 0;
	pMember->name = name;
	pMember->role = getSelectionFromEnumList(roleStr, eNumOfRoles, "Choose a role from the following:", "Invalid input has been entered, please try again:");
	return 1;
}

int initSoloMember(Member* pMember) {
	char* name = getName("Enter member name");
	if (!name) return 0;
	pMember->name = name;
	pMember->role = 0; // solo member is the leading singer
	return 1;
}

void printMember(const Member* pMember)
{
	printf("%s - %s\n", pMember->name, roleStr[pMember->role]);
}
void freeMember(Member* pMember)
{
	free(pMember->name);
}

int writeMemberToTextFile(const Member* pMember, FILE* f)
{
	if (!f) return 0;
	fprintf(f, "%s\n%d\n", pMember->name, pMember->role);
	return 1;
}
Member* readMemberFromTextFile(FILE* f)
{
	if (!f) return NULL;
	Member* newMember = (Member*)malloc(sizeof(Member));
	if (!newMember) return NULL;
	char buffer[MAX_STR_LEN];
	myGets(buffer, sizeof(buffer), f);
	newMember->name = _strdup(buffer);
	if (fscanf(f, "%d\n", &newMember->role) != 1) {
		free(newMember->name);
		return NULL;
	}
	return newMember;
}

int saveMemberToBinFile(const Member* pMember, FILE* f)
{
	CHECK_VALIDITY(f, 0);
	CHECK_VALIDITY(pMember, 0);
	CHECK_VALIDITY(saveStringToBinFile(pMember->name, f), 0); //writing name
	CHECK_VALIDITY(fwrite(&pMember->role, sizeof(int), 1, f) == 1, 0); // writing role
	return 1;
}
Member* loadMemberFromBinFile(FILE* f)
{
	CHECK_VALIDITY(f, NULL);
	char * name = loadStringFromBinFile(f); //reads name
	CHECK_VALIDITY(name, NULL);
	Member *newMember = (Member*)malloc(sizeof(Member));
	FREE_IF_FAILED(newMember, name, NULL);
	newMember->name = name;
	if(fread(&newMember->role, sizeof(int), 1, f) != 1){ //reading role
		freeMember(newMember);
		free(newMember);
		return NULL;
	}
	return newMember;
}