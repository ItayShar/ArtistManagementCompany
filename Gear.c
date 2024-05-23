#include "Gear.h"

void initGear(Gear* pGear)
{
	pGear->type = getSelectionFromEnumList(GearTypeStr, eNumOfGear, "Choose gear type:", "Invalid gear type, please try again");
	pGear->price = getNumberInRange(1, 8000, "Enter price:");
}

void printGear(const void * v)
{
	const Gear* pGear = (const Gear*)v;
	printf("%s - %d\n", GearTypeStr[pGear->type], pGear->price);
}

int writeGearToTextFile(const Gear* pGear, FILE* f)
{
	CHECK_VALIDITY(f, 0);
	fprintf(f, "%d %d\n", pGear->type, pGear->price);
	return 1;
}
Gear* readGearFromTextFile(FILE* f)
{
	CHECK_VALIDITY(f, NULL);
	Gear* newGear = (Gear*)malloc(sizeof(Gear));
	CHECK_VALIDITY(newGear, NULL);
	if (fscanf(f, "%d %d\n", &newGear->type, &newGear->price) != 2) {
		free(newGear);
		return NULL;
	}
	return newGear;
}

int saveGearToBinFile(const Gear* gear, FILE* f)
{
	CHECK_VALIDITY(f, 0);
	CHECK_VALIDITY(gear, 0);
	BYTE data[2];
	data[0] = gear->price >> 5;
	data[1] = (gear->price << 3) | gear->type;
	if (fwrite(data, sizeof(BYTE), 2, f) != 2) return 0;
	return 1;
}
Gear* loadGearFromBinFile(FILE* f)
{
	CHECK_VALIDITY(f, NULL);
	Gear* newGear = (Gear*)malloc(sizeof(Gear));
	CHECK_VALIDITY(newGear, NULL);
	BYTE data[2];
	IS_READ_CORRECTLY(fread(data, sizeof(char), 2, f), 2, NULL);
	newGear->type = data[1] & 0x7;
	newGear->price = (data[0] << 5) | (data[1] >> 3);
	return newGear;
}