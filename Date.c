#include "Date.h"

void initDate(Date* pDate)
{
	char buffer[MAX_STR_LEN];
	printf("Use the following format: dd/mm/yyyy\n");
	while (1)
	{
		myGets(buffer, sizeof(buffer), stdin);
		if (checkDate(pDate, buffer))
			break;
		else
			printf("Invalid date entered, please try again:\n");
	}
}

int checkDate(Date* pDate, char* str)
{
	//checkFormat executes first and assigns values to pDate fields, which are checked in the other 3 functions
	if (checkFormat(pDate, str) && checkYear(pDate->year) && checkMonth(pDate->month) && checkDay(pDate->day, pDate->month))
		return 1;
	return 0;
}
int checkYear(int year)
{
	return year >= 1;
}

int checkMonth(int month)
{
	return month >= 1 && month <= 12;
}
int checkDay(int day, int month)
{
	if (day > dayChecker[month - 1] || day < 1) // checking if the day inserted is greater than the maximum day of the month inserted
		return 0;
	return 1;
}
int checkFormat(Date* pDate, char* str)
{
	//assigning values into pDate fields while checking format was entered correctly
	return sscanf(str, "%2d/%2d/%4d", &pDate->day, &pDate->month, &pDate->year) == 3;
}

void printDate(const Date* pDate)
{
	printf("%d/%d/%d", pDate->day, pDate->month, pDate->year);
}


int compareDates(const Date* d1, const Date* d2)
{	//returns 1 if d2 > d1
	//returns -1 if d1 > d2
	//returns 0 if d1 = d2
	if (d1->year != d2->year) {
		if (d2->year < d1->year)
			return 1;
		return -1; // d1->year < d2->year
	}
	if (d1->month != d2->month) {
		if (d2->month < d1->month)
			return 1;
		return -1; // d1->month > d2->month
	}
	if (d1->day != d2->day) {
		if (d2->day < d1->day)
			return 1;
		return -1; // d1->day > d2->day
	}
	return 0; // dates are identical
}

int writeDateToTextFile(const Date* pDate, FILE* f)
{
	if (!f) return 0;
	fprintf(f, "%d %d %d\n", pDate->day, pDate->month, pDate->year);
	return 1;
}
Date* readDateFromTextFile(FILE* f)
{
	if (!f) return NULL;
	Date* newDate = (Date*)malloc(sizeof(Date));
	if (!newDate) return NULL;
	if (fscanf(f, "%d %d %d\n", &newDate->day, &newDate->month, &newDate->year) != 3)
	{
		free(newDate);
		return NULL;
	}
	return newDate;
}