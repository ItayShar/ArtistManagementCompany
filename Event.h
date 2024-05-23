#pragma once
#define RECORDING_DESCRIPTION_LEN 17
#include "General.h"
#include "Date.h"

typedef struct {
	Date* date;
	char* description;
}Event;

int initEvent(Event* pEvent, const Date* maxDate);
int writeEventToTextFile(const Event* pEvent, FILE* f);
int saveEventToBinFile(const Event* pEvent, FILE* f);
void printEvent(const Event* pEvent);
void freeEvent(void* v);
void getValidDate(Date* eventDate, const Date* maxDate);
Event* createConcertEvent(const Date * concertDate);
Event* createRecordingEvent(const char* studioCode, const Date* maxDate);
Event* readEventFromTextFile(FILE* f);
Event* loadEventFromBinFile(FILE* f);