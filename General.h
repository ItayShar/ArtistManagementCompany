#pragma once
#define MAX_STR_LEN 255
#define CHECK_VALIDITY(val,returnVal) if(!val) {return returnVal;}
#define FREE_IF_FAILED(val,valToFree,returnVal) if(!val) {free(valToFree); return returnVal;}
#define IS_READ_CORRECTLY(readFunc, val, returnVal) if(readFunc != val) {return returnVal;}
#define EXEC_IF_FAILED(val,execFunc,returnVal) if(!val){execFunc; return returnVal;}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "list.h"

char* myGets(char* buffer, int size, FILE* f);
char* getName(const char * msg);
char* dupStrUppercase(const char* str);
int getNumberGreaterThan(int min, const char* msg);
int getNumberInRange(int min, int max, const char* msg);
int getNonNegativeNumber(const char* msg);
int getSelectionFromEnumList(const char** enumStrArr, int arrSize, const char* msg, const char* invalidMsg);
int printLinkedListInRows(const LIST* pList, void (*print)(const void*));
NODE* removeNodeFromListByIndex(LIST* pList, int index);
void generalArrayFunction(void* arr, int numOfElements, size_t sizeofElement, void f(void* element));
int saveStringToBinFile(const char* str, FILE * f);
char* loadStringFromBinFile(FILE * f);
void uppercaseStr(char* str);
