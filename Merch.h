#pragma once
#define CODE_LEN 3
#include "General.h"
typedef enum {eShirt, eHat, eCup, eDisc, ePoster, ePhoneCase, ePopDoll, eLighter, eNumOfMerch } eMerchType;
static const char* MerchTypeStr[eNumOfMerch] = { "Shirt", "Hat", "Cup", "Disc", "Poster", "Phone Case", "Pop Doll", "Lighter"};
typedef struct {
	eMerchType type;
	int price;
	char code[CODE_LEN + 1];
} Merch;

int isUniqueCode(const char* code, const Merch** exisitingMerchArr, int merchArrSize);
int writeMerchToTextFile(const Merch* pMerch, FILE* f);
int saveMerchToBinFile(const Merch* pMerch, FILE* f);
void initMerch(Merch* pMerch, const Merch ** exisitingMerchArr, int merchArrSize);
void printMerch(const Merch* pMerch);
void printMerchWithEnter(const void* v);
void getMerchCode(Merch* pMerch, const Merch** exisitingMerchArr, int merchArrSize);
Merch* readMerchFromTextFile(FILE* f);
Merch* loadMerchFromBinFile(FILE* f);
