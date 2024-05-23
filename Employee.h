#pragma once
#include "General.h"

typedef struct {
	char* name;
	int salary;
} Employee;

int initEmployee(Employee* pEmployee);
int writeEmployeeToTextFile(const Employee* pEmployee, FILE* f);
int saveEmployeeToBinFile(const Employee* pEmployee, FILE* f);
void printEmployee(const Employee* pEmployee);
void printEmployeeWithEnter(const void* v);
void freeEmployee(Employee* pEmployee);
void freeEmployeePtr(void* v);
Employee* copyEmployee(const Employee* pEmployee);
Employee* readEmployeeFromTextFile(FILE* f);
Employee* loadEmployeeFromBinFile(FILE* f);