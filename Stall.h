#pragma once
#include "Employee.h"
#include "Merch.h"
#include "list.h"
typedef struct
{
	Employee * stallEmployee;
	Merch merch;
	int merchAmount;
	int soldMerch;
	int activeHours;
} Stall;

int initStall(Stall* pStall, LIST* employeeList, const Merch** merchArr, int merchArrSize);
Employee* getEmployeeFromList(LIST* employeeList);
const Merch * getMerchFromArr(const Merch** merchArr, int merchArrSize);
void printStall(const void* v);
int	 calcStallRevenue(const Stall* pStall);
void freeStall(Stall* pStall);
int writeStallToTextFile(const Stall* pStall, FILE* f);
Stall* readStallFromTextFile(FILE* f);
int saveStallToBinFile(const Stall* pStall, FILE* f);
Stall* loadStallFromBinFile(FILE* f);
