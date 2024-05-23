#include "Studio.h"

int initStudio(Studio* pStudio, const Studio** existingStudioArr, int studioArrSize)
{
	getCode(pStudio, existingStudioArr, studioArrSize);
	pStudio->gearArrSize = getNumberGreaterThan(0, "Enter amount of gear: ");
	pStudio->gearArr = (Gear*)malloc(sizeof(Gear) * pStudio->gearArrSize);
	if (!pStudio->gearArr)
		return 0;
	if (!getGear(pStudio))
		return 0;
	pStudio->rentPrice = getNumberGreaterThan(0, "Enter amount for studio rent: ");
	return 1;
}


void getCode(Studio* pStudio, const Studio** existingStudioArr, int studioArrSize)
{
	char temp[MAX_STR_LEN];
	while (1)
	{
		printf("Enter studio code  - %d letters. \n", CODE_LEN);
		myGets(temp, sizeof(temp), stdin);
		uppercaseStr(temp);
		if (strlen(temp) != CODE_LEN)
		{
			printf("code should be %d letters\n", CODE_LEN);
			continue;
		}
		if (!(isSameCode(temp, existingStudioArr, studioArrSize)))
		{
			printf("Code is not unique, please enter a new code. \n");
			continue;
		}
		strcpy(pStudio->code, temp);
		break;
	}
}

int isSameCode(char* code, const Studio** existingStudioArr, int arrSize)
{
	for (int i = 0; i < arrSize; i++)
	{
		if ((strcmp(existingStudioArr[i]->code, code) == 0))
			return 0;
	}
	return 1;
}

int getGear(Studio* pStudio)
{
	Gear tempGear;
	for (int i = 0; i < pStudio->gearArrSize; i++)
	{
		initGear(&tempGear);
		pStudio->gearArr[i] = tempGear;
	}
	return 1;
}

void printRegStudio(const Studio * pStudio)
{
	printf("Studio code: %s \n", pStudio->code);
	printf("Rent: %d\n", pStudio->rentPrice);
	printf("Gear:\n");
	generalArrayFunction(pStudio->gearArr, pStudio->gearArrSize, sizeof(Gear), printGear);
	printf("\n");
}


void printStudio(const void * v)
{
	const Studio* pStudio = *(const Studio**)v;
	printf("Studio code: %s \n", pStudio->code);
	printf("Rent: %d\n", pStudio->rentPrice);
	printf("Gear:\n");
	generalArrayFunction(pStudio->gearArr, pStudio->gearArrSize, sizeof(Gear), printGear);
	printf("\n");
}

int  calcStudioCost(const Studio* pStudio)
{
	int sum = pStudio->rentPrice;
	for (int i = 0; i < pStudio->gearArrSize; i++)
	{
		sum += pStudio->gearArr[i].price;
	}
	return sum;
}

void freeStudio(Studio* pStudio, int numOfGear)
{
	free(pStudio->gearArr);
	free(pStudio);
}

int addNewGear(Studio* pStudio)
{
	Gear* newGear = (Gear *)malloc(sizeof(Gear));
	if (!newGear) return 0;
	initGear(newGear);
	Gear* tmpListPtr = (Gear*)realloc(pStudio->gearArr, (pStudio->gearArrSize + 1) * sizeof(Gear));
	if (!tmpListPtr) {
		free(newGear);
		return 0;
	}
	pStudio->gearArr = tmpListPtr;
	pStudio->gearArrSize++;
	pStudio->gearArr[pStudio->gearArrSize] = *newGear;
	return 1;
}

int writeStudioToTextFile(const Studio* pStudio, FILE* f)
{
	if (!f) return 0;
	fprintf(f, "%s\n", pStudio->code);
	fprintf(f, "%d\n", pStudio->gearArrSize);
	for (int i = 0; i < pStudio->gearArrSize; i++)
		writeGearToTextFile(&pStudio->gearArr[i], f);
	fprintf(f, "%d\n", pStudio->rentPrice);
	return 1;
}
Studio * readStudioFromTextFile(FILE* f)
{
	Gear* newGear;
	if (!f) return NULL;
	Studio* newStudio = (Studio*)malloc(sizeof(Studio));
	if (!newStudio) return NULL;
	char buffer[MAX_STR_LEN];
	myGets(buffer, CODE_LEN + 1, f);
	strcpy(newStudio->code, buffer);
	if (fscanf(f, "%d\n", &newStudio->gearArrSize) != 1) {
		free(newStudio);
		return NULL;
	}
	newStudio->gearArr = (Gear*)malloc(sizeof(Gear) * newStudio->gearArrSize);
	if (!newStudio->gearArr) {
		free(newStudio);
		return NULL;
	}
	for (int i = 0; i < newStudio->gearArrSize; i++) {
		newGear = readGearFromTextFile(f);
		if (!newGear) {
			freeStudio(newStudio, i);
			return NULL;
		}
		newStudio->gearArr[i] = *newGear;
		free(newGear);
	}
	if (fscanf(f, "%d\n", &newStudio->rentPrice) != 1) {
		freeStudio(newStudio, newStudio->gearArrSize);
		free(newStudio);
		return NULL;
	}
	return newStudio;
}

int saveStudioToBinFile(const Studio* pStudio, FILE* f)
{
	if (!f || !pStudio) return 0;
	CHECK_VALIDITY(fwrite(pStudio->code, sizeof(char), sizeof(pStudio->code), f) == sizeof(pStudio->code), 0);//writes code
	CHECK_VALIDITY(fwrite(&pStudio->gearArrSize, sizeof(int), 1, f) == 1, 0); //writes gearArrSize
	CHECK_VALIDITY(fwrite(&pStudio->rentPrice, sizeof(int), 1, f) == 1, 0); //writes rentPrice
	for (int i = 0; i < pStudio->gearArrSize; i++)//writes gearArr
		saveGearToBinFile(&pStudio->gearArr[i], f);
	return 1;
}

Studio* loadStudioFromBinFile(FILE* f)
{
	CHECK_VALIDITY(f, NULL);
	Gear* newGear;
	Studio* newStudio = (Studio*)malloc(sizeof(Studio));
	CHECK_VALIDITY(newStudio, NULL);
	if (fread(newStudio->code, sizeof(char), sizeof(newStudio->code), f) != sizeof(newStudio->code) //reads code
		|| fread(&newStudio->gearArrSize, sizeof(int), 1, f) != 1 //reads gearArrSize
		|| fread(&newStudio->rentPrice, sizeof(int), 1, f) != 1) { //reads rentPrice
		free(newStudio);
		return NULL;
	}
	newStudio->gearArr = (Gear*)malloc(sizeof(Gear) * newStudio->gearArrSize); //allocating gearArr
	FREE_IF_FAILED(newStudio->gearArr, newStudio, NULL);
	for (int i = 0; i < newStudio->gearArrSize; i++) { //reads gearArr
		newGear = loadGearFromBinFile(f);
		if (!newGear) {
			freeStudio(newStudio, i);
			free(newStudio);
			return NULL;
		}
		newStudio->gearArr[i] = *newGear;
		free(newGear);
	}
	return newStudio;
}