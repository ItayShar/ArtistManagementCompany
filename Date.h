#pragma once
#include "General.h"
static const int dayChecker[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
typedef struct {
	int day;
	int month;
	int year;
}Date;
int checkYear(int year);
int checkMonth(int month);
int checkDay(int day, int month);
int checkDate(Date* pDate, char* str);
int checkFormat(Date* pDate, char* str);
int compareDates(const Date* d1, const Date* d2);
void initDate(Date* pDate);
void printDate(const Date* pDate);
int writeDateToTextFile(const Date* pDate, FILE* f);
Date* readDateFromTextFile(FILE* f);