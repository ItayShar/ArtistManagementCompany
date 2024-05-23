#include "Stall.h"
int initStall(Stall* pStall, LIST * employeeList ,const Merch** merchArr, int merchArrSize)
{
	Employee* tmpEmployee = getEmployeeFromList(employeeList);
	if (!tmpEmployee) return 0;
	pStall->stallEmployee = tmpEmployee;
	const Merch * tmpMerch = getMerchFromArr(merchArr, merchArrSize);
	if (!tmpMerch) {
		freeEmployee(pStall->stallEmployee);
		return 0;
	}
	pStall->merch = *tmpMerch;
	pStall->merchAmount = getNumberGreaterThan(0, "Enter amount of items available at the stall:");
	pStall->soldMerch = getNumberInRange(0, pStall->merchAmount, "Enter amount of items sold:");
	pStall->activeHours = getNumberGreaterThan(0, "How many hours was the stall active for?");
	return 1;
}

Employee * getEmployeeFromList(LIST * employeeList)
{
	if (!employeeList) return NULL;
	int option, numOfEmployees;
	printf("Choose an Employee from the list:\n");
	while (1) {
		numOfEmployees = printLinkedListInRows(employeeList, printEmployee);
		scanf("%d", &option);
		if (!(option >= 1 && option <= numOfEmployees)) {
			printf("Invalid selection, please try again:\n");
			continue;
		}
		break;
	}
	NODE* tmpNode = removeNodeFromListByIndex(employeeList, option);
	if (!tmpNode) return NULL;
	Employee* returnVal = ((Employee*)tmpNode->key);
	free(tmpNode);
	return returnVal;
}

const Merch * getMerchFromArr(const Merch** merchArr, int merchArrSize)
{
	if (!merchArr) return NULL;
	int option;
	printf("Choose an item from the list:\n");
	while (1)
	{
		for (int i = 0; i < merchArrSize; i++) {
			printf("%d. ", i + 1);
			printMerch(merchArr[i]);
			printf("\n");
		}
		scanf("%d", &option);
		if (!(option >= 1 && option <= merchArrSize)) {
			printf("Invalid selection, please try again:\n");
			continue;
		}
		break;
	}
	return merchArr[option - 1];
}

void printStall(const void* v)
{
	const Stall* pStall = *(const Stall**) v;
	printf("Employee - ");
	printEmployee(pStall->stallEmployee);
	printf(" | Merch - ");
	printMerch(&pStall->merch);
	printf(" | Amount - %d  Sold - %d  Active Hours - %d\n", pStall->merchAmount, pStall->soldMerch, pStall->activeHours);
}
int	 calcStallRevenue(const Stall* pStall)
{
	return (pStall->soldMerch * pStall->merch.price) - ( pStall->activeHours * pStall->stallEmployee->salary);
}
void freeStall(Stall* pStall)
{
	freeEmployee(pStall->stallEmployee);
	free(pStall->stallEmployee);
	free(pStall);
}


int writeStallToTextFile(const Stall* pStall, FILE* f)
{
	if (!f) return 0;
	if (!writeEmployeeToTextFile(pStall->stallEmployee, f)) return 0;
	if (!writeMerchToTextFile(&pStall->merch, f)) return 0;
	fprintf(f, "%d %d %d\n", pStall->merchAmount, pStall->soldMerch, pStall->activeHours);
	return 1;
}
Stall* readStallFromTextFile(FILE* f)
{
	Merch* merch;
	Stall* newStall = (Stall*)malloc(sizeof(Stall));
	if (!newStall) return NULL;
	if (!f) return NULL;
	newStall->stallEmployee = readEmployeeFromTextFile(f);
	if (!newStall->stallEmployee) return NULL;
	merch = readMerchFromTextFile(f);
	if (!merch) {
		freeEmployee(newStall->stallEmployee);
		return NULL;
	}
	newStall->merch = *merch;
	free(merch);
	if(fscanf(f, "%d %d %d\n", &newStall->merchAmount, &newStall->soldMerch, &newStall->activeHours) != 3)
	{
		freeEmployee(newStall->stallEmployee);
		free(&newStall->merch);
		return NULL;
	}
	return newStall;
}

int saveStallToBinFile(const Stall* pStall, FILE* f)
{
	CHECK_VALIDITY(f, 0);
	CHECK_VALIDITY(pStall, 0);
	CHECK_VALIDITY(fwrite(&pStall->merch, sizeof(Merch), 1, f) == 1, 0); // writes merch
	CHECK_VALIDITY(fwrite(&pStall->merchAmount, sizeof(int), 1, f) == 1, 0); //writes merchAmount
	CHECK_VALIDITY(fwrite(&pStall->soldMerch, sizeof(int), 1, f) == 1, 0); //writes soldMerch
	CHECK_VALIDITY(fwrite(&pStall->activeHours, sizeof(int), 1, f) == 1, 0); //writes activeHours
	CHECK_VALIDITY(saveEmployeeToBinFile(pStall->stallEmployee, f), 0);
	return 1;
}
Stall* loadStallFromBinFile(FILE* f)
{
	CHECK_VALIDITY(f, NULL);
	Stall* newStall = (Stall*)malloc(sizeof(Stall));
	CHECK_VALIDITY(newStall, NULL);
	if (fread(&newStall->merch, sizeof(Merch), 1, f) != 1 // reads merch
		|| fread(&newStall->merchAmount, sizeof(int), 1, f) != 1 //reads merchAmount
		|| fread(&newStall->soldMerch, sizeof(int), 1, f) != 1 //reads soldMerch
		|| fread(&newStall->activeHours, sizeof(int), 1, f) != 1) //reads activeHours
	{
		free(newStall);
		return NULL;
	}
	newStall->stallEmployee = loadEmployeeFromBinFile(f); // reads employee
	FREE_IF_FAILED(newStall->stallEmployee, newStall, NULL);
	return newStall;
}