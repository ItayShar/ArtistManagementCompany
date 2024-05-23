#include "Event.h"

int initEvent(Event* pEvent, const Date * maxDate)
{
	char* description = getName("Enter event description:");
	CHECK_VALIDITY(description, 0);
	char* duplicate = dupStrUppercase(description);
	CHECK_VALIDITY(duplicate, 0);
	if (strcmp(duplicate, "CONCERT") == 0)
	{
		printf("To create a new concert, please use the 'add new concert' option\n");
		free(description);
		free(duplicate);
		return 0;
	}
	if (strcmp(duplicate, "RECORDING") == 0)
	{
		printf("To create a new recording, please use the 'create new recording' option at company menu\n");
		free(description);
		free(duplicate);
		return 0;
	}
	free(duplicate);
	Date* eventDate = (Date*)malloc(sizeof(Date));
	if (!eventDate) return 0;
	getValidDate(eventDate, maxDate);
	pEvent->date = eventDate;
	pEvent->description = description;
	return 1;
}

void getValidDate(Date * eventDate, const Date * maxDate) {
	while (1) {
		printf("Enter event date:\n");
		initDate(eventDate);
		if (compareDates(eventDate, maxDate) == 1) // if eventDate > maxDate
		{
			printf("Please enter a date prior to ");
			printDate(maxDate);
			printf("\n");
			continue;
		}
		break;
	}
}

Event * createConcertEvent(const Date * concertDate)
{
	Event* newConcert = (Event*)malloc(sizeof(Event));
	if (!newConcert) return NULL;
	newConcert->description = _strdup("concert");
	newConcert->date = (Date*)malloc(sizeof(Date));
	if (!newConcert->date) {
		free(newConcert->description);
		free(newConcert);
		return NULL;
	}
	//values are copied so newConcert->date won't point to the same Date struct as the Concert struct
	newConcert->date->day = concertDate->day;
	newConcert->date->month = concertDate->month;
	newConcert->date->year = concertDate->year;
	return newConcert;
}

Event* createRecordingEvent(const char* studioCode, const Date* maxDate) {
	CHECK_VALIDITY(studioCode, NULL);
	CHECK_VALIDITY(maxDate, NULL);
	Event* newRecording = (Event*)malloc(sizeof(Event));
	CHECK_VALIDITY(newRecording, NULL);
	char buffer[RECORDING_DESCRIPTION_LEN];
	sprintf(buffer, "recording at %s", studioCode);
	newRecording->description = _strdup(buffer);
	Date* recordingDate = (Date*)malloc(sizeof(Date));
	EXEC_IF_FAILED(recordingDate, free(newRecording->description); free(newRecording), NULL);
	getValidDate(recordingDate, maxDate);
	newRecording->date = recordingDate;
	return newRecording;
}

void printEvent(const Event* pEvent)
{
	printf("%s - ", pEvent->description);
	printDate(pEvent->date);
}
void freeEvent(void * v)
{
	Event* pEvent = (Event*)v;
	free(pEvent->description);
	free(pEvent->date);
	free(pEvent);
}

int writeEventToTextFile(const Event* pEvent, FILE* f)
{
	if (!f) return 0;
	fprintf(f, "%s\n", pEvent->description);
	if (!writeDateToTextFile(pEvent->date, f)) return 0;
	return 1;
}

Event* readEventFromTextFile(FILE* f)
{
	if (!f) return NULL;
	Event* newEvent = (Event*)malloc(sizeof(Event));
	if (!newEvent) return NULL;
	char buffer[MAX_STR_LEN];
	myGets(buffer, sizeof(buffer), f);
	newEvent->description = _strdup(buffer);
	newEvent->date = readDateFromTextFile(f);
	if (!newEvent->date) {
		free(newEvent->description);
		free(newEvent);
		return NULL;
	}
	return newEvent;
}

int saveEventToBinFile(const Event* pEvent, FILE* f)
{
	CHECK_VALIDITY(f, 0);
	CHECK_VALIDITY(pEvent, 0);
	CHECK_VALIDITY(saveStringToBinFile(pEvent->description, f), 0); //saves description
	CHECK_VALIDITY(fwrite(pEvent->date, sizeof(Date), 1, f) == 1, 0); //saves date
	return 1;
}
Event* loadEventFromBinFile(FILE* f)
{
	CHECK_VALIDITY(f, NULL);
	char* description = loadStringFromBinFile(f); // reads description
	CHECK_VALIDITY(description, NULL);
	Event* newEvent = (Event*)malloc(sizeof(Event));
	FREE_IF_FAILED(newEvent, description, NULL);
	newEvent->description = description;
	newEvent->date = (Date*)malloc(sizeof(Date));
	EXEC_IF_FAILED(newEvent->date, free(newEvent->description); free(newEvent); , NULL);
	if (fread(newEvent->date, sizeof(Date), 1, f) != 1) { //reads date
		free(newEvent->date);
		free(newEvent->description);
		free(newEvent);
		return NULL;
	}
	return newEvent;
}