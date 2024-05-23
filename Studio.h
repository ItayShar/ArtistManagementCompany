#pragma once
#include "Gear.h"
#define CODE_LEN 3
typedef struct
{
	char code[CODE_LEN + 1];
	Gear* gearArr;
	int gearArrSize;
	int rentPrice;

}Studio;


int initStudio(Studio* pStudio, const Studio** existingStudioArr, int studioArrSize);
int calcStudioCost(const Studio* pStudio);
int getGear(Studio* pStudio);
int	isSameCode(char* code, const Studio** existingStudioArr, int arrSize);
int writeStudioToTextFile(const Studio * pStudio, FILE * f);
int saveStudioToBinFile(const Studio* pStudio, FILE* f);
void freeStudio(Studio* pStudio, int numOfGear);
void printStudio(const void* v);
void printRegStudio(const Studio* pStudio);
void getCode(Studio* pStudio, const Studio** existingStudioArr, int studioArrSize);
Studio* readStudioFromTextFile(FILE* f);
Studio* loadStudioFromBinFile(FILE* f);