#include "Merch.h"

void initMerch(Merch* pMerch, const Merch** exisitingMerchArr, int merchArrSize)
{
	pMerch->type = getSelectionFromEnumList(MerchTypeStr, eNumOfMerch, "Choose item type:", "Invalid input, please try again");
	pMerch->price = getNumberGreaterThan(0, "Enter item price:");
	getMerchCode(pMerch, exisitingMerchArr, merchArrSize);
}

void printMerch(const Merch* pMerch)
{
	printf("Code: %s Price: %d Type: %s", pMerch->code, pMerch->price, MerchTypeStr[pMerch->type]);
}

void printMerchWithEnter(const void * v)
{
	const Merch* pMerch = *(const Merch**)v;
	printf("Code: %s Price: %d Type: %s\n", pMerch->code, pMerch->price, MerchTypeStr[pMerch->type]);
}

void getMerchCode(Merch* pMerch, const Merch** exisitingMerchArr, int merchArrSize) {
	char buffer[MAX_STR_LEN];
	printf("Enter the product's code (%d characters):\n", CODE_LEN);
	while (1)
	{
		myGets(buffer, sizeof(buffer), stdin);
		uppercaseStr(buffer);
		if (strlen(buffer) != 3) {
			printf("Invalid code entered, please try again:\n");
			continue;
		}
		if (!isUniqueCode(buffer, exisitingMerchArr, merchArrSize)) {
			printf("code already exists, please try again:\n");
			continue;
		}
		break;
	}
	strcpy(pMerch->code, buffer);
}

int isUniqueCode(const char* code, const Merch** exisitingMerchArr, int merchArrSize)
{
	for (int i = 0; i < merchArrSize; i++) {
		if (strcmp(exisitingMerchArr[i]->code, code) == 0)
			return 0;
	}
	return 1;
}

int writeMerchToTextFile(const Merch* pMerch, FILE* f)
{
	if (!f) return 0;
	fprintf(f, "%s %d %d\n", pMerch->code, pMerch->type, pMerch->price);
	return 1;
}
Merch* readMerchFromTextFile(FILE* f)
{
	if (!f) return NULL;
	Merch* newMerch = (Merch*)malloc(sizeof(Merch));
	if (!newMerch) return NULL;
	char buffer[MAX_STR_LEN];
	myGets(buffer, CODE_LEN + 1 , f);
	strcpy(newMerch->code, buffer);
	if (fscanf(f, " %d\n", &newMerch->type) != 1) {
		free(newMerch);
		return NULL;
	}
	if (fscanf(f, " %d\n", &newMerch->price) != 1) {
		free(newMerch);
		return NULL;
	}
	return newMerch;
}

int saveMerchToBinFile(const Merch* pMerch, FILE* f)
{
	CHECK_VALIDITY(f, 0);
	CHECK_VALIDITY(pMerch, 0);
	CHECK_VALIDITY(fwrite(pMerch->code, sizeof(char), sizeof(pMerch->code), f) == sizeof(pMerch->code), 0); //writes code
	CHECK_VALIDITY(fwrite(&pMerch->type, sizeof(int), 1, f) == 1, 0); //writes type
	CHECK_VALIDITY(fwrite(&pMerch->price, sizeof(int), 1, f) == 1, 0); //writes price
	return 1;
}
Merch* loadMerchFromBinFile(FILE* f)
{
	CHECK_VALIDITY(f, NULL);
	Merch* newMerch = (Merch*)malloc(sizeof(Merch));
	CHECK_VALIDITY(newMerch, NULL);
	if (fread(newMerch->code, sizeof(char), sizeof(newMerch->code), f) != sizeof(newMerch->code) //reads code
		|| fread(&newMerch->type, sizeof(int), 1, f) != 1 //reads type
		|| fread(&newMerch->price, sizeof(int), 1, f) != 1) //writes price
	{
		free(newMerch);
		return NULL;
	}
	return newMerch;
}