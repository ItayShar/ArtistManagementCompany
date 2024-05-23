#pragma once
#include "General.h"

typedef enum { eMicrophone, eGuitar, eDrums, eHeadPhones, eMixer, eNumOfGear } eGearType;
static const char* GearTypeStr[eNumOfGear] = { "Microphone", "guitar", "Drums", "HeadPhones", "Mixer" };

typedef struct {
	int price;
	eGearType type;
}Gear;


int writeGearToTextFile(const Gear* pGear, FILE * f);
int saveGearToBinFile(const Gear* gear, FILE* f);
void initGear(Gear* pGear);
void printGear(const void * v);
Gear* readGearFromTextFile(FILE* f);
Gear* loadGearFromBinFile(FILE* f);
