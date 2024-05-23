#include "Employee.h"
int initEmployee(Employee* pEmployee)
{
	char* name = getName("Enter employee's name:");
	if (!name) return 0;
	pEmployee->name = name;
	pEmployee->salary = getNumberGreaterThan(0, "Enter employee's hourly salary:");
	return 1;
}

void printEmployee(const Employee* pEmployee)
{
	printf("Name: %s Salary: %d", pEmployee->name, pEmployee->salary);
}

void printEmployeeWithEnter(const void * v) {
	const Employee* pEmployee = (const Employee*)v;
	printf("Name: %s Salary: %d\n", pEmployee->name, pEmployee->salary);
}
void freeEmployee(Employee* pEmployee)
{
	free(pEmployee->name);
}

void freeEmployeePtr(void* v) {
	Employee* pEmployee = (Employee*)v;
	free(pEmployee->name);
	free(pEmployee);
}

int writeEmployeeToTextFile(const Employee* pEmployee, FILE* f)
{
	if (!f) return 0;
	fprintf(f, "%s\n%d\n", pEmployee->name, pEmployee->salary);
	return 1;
}


Employee* readEmployeeFromTextFile(FILE* f)
{
	if (!f) return NULL;
	Employee* newEmployee = (Employee*)malloc(sizeof(Employee));
	if (!newEmployee) return NULL;
	char buffer[MAX_STR_LEN];
	myGets(buffer, sizeof(buffer), f);
	newEmployee->name = _strdup(buffer);
	if (fscanf(f, "%d\n", &newEmployee->salary) != 1) {
		free(newEmployee->name);
		return NULL;
	}
	return newEmployee;
}

int saveEmployeeToBinFile(const Employee* pEmployee, FILE* f)
{
	CHECK_VALIDITY(f, 0);
	CHECK_VALIDITY(pEmployee, 0);
	CHECK_VALIDITY(saveStringToBinFile(pEmployee->name, f), 0); //writing name
	CHECK_VALIDITY(fwrite(&pEmployee->salary, sizeof(int), 1, f) == 1, 0); // writing salary
	return 1;
}

Employee* loadEmployeeFromBinFile(FILE* f)
{
	CHECK_VALIDITY(f, NULL);
	char* name = loadStringFromBinFile(f); //reading name
	CHECK_VALIDITY(name, NULL);
	Employee* newEmployee = (Employee*)malloc(sizeof(Employee));
	FREE_IF_FAILED(newEmployee, name, NULL);
	newEmployee->name = name;
	if(fread(&newEmployee->salary, sizeof(int), 1, f) != 1) //reading salary
	{
		freeEmployee(newEmployee);
		free(newEmployee);
		return NULL;
	}
	return newEmployee;
}

Employee* copyEmployee(const Employee* pEmployee)
{
	CHECK_VALIDITY(pEmployee, NULL);
	Employee* newEmployee = (Employee*)malloc(sizeof(Employee));
	CHECK_VALIDITY(newEmployee, NULL);
	newEmployee->name = _strdup(pEmployee->name);
	newEmployee->salary = pEmployee->salary;
	return newEmployee;
}