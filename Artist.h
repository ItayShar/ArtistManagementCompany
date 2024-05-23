#pragma once
#define SOLO 1
#include "list.h"
#include "Merch.h"
#include "Member.h"
#include "Employee.h"
#include "Concert.h"
#include "Event.h"

enum{ eReturnToMenu, ePrintArtist, eAddSong, eAddEmp, eAddMerch, eAddConcert, eAddEvent, eRenew};

typedef struct {
	Member member;
} SoloArtist;

typedef struct{
	Member* bandMembers;
} Band;

typedef struct Artist_ {
	void (*printFunc) (const struct Artist_ *);
	void (*freeFunc) (struct Artist_*);

	int numOfMembers;
	char* name;
	int songAmount;
	LIST songList;
	int employeeAmount;
	Employee* employeeList;
	int merchAmount;
	Merch** merchArr;
	int concertAmount;
	Concert** concertArr;
	int eventAmount;
	LIST schedule;
	Date * contractExpireDate;
	double revenue;
	union {
		SoloArtist soloArtist;
		Band band;
	}type;
}Artist;

int initArtist(Artist* pArtist);
int initSolo(Artist* pArtist);
int initBand(Artist* pArtist);
int generateSongList(Artist* pArtist);
int generateMerchList(Artist* pArtist);
int generateEmployeeList(Artist* pArtist);
int generateConcertList(Artist* pArtist);
int generateSchedule(Artist* pArtist);
int getContractDate(Artist* pArtist);
int getArtistType();
int freeBandMembers(Artist* pArtist, int numOfMembers);
int freeEmployeeList(Artist* pArtist, int numOfEmployees);
int freeConcertList(Artist* pArtist, int numOfConcerts);
int failedCreation(Artist* pArtist, int step);
int addNewSong(Artist* pArtist);
int addNewEmployee(Artist* pArtist);
int addNewConcert(Artist* pArtist);
int addNewMerch(Artist* pArtist);
int addNewEvent(Artist* pArtist);
int calcConcertsRevenue(const Artist* pArtist);
int writeArtistToTextFile(const Artist* pArtist, FILE* f);
int writeSongListToTextFile(const Artist* pArtist, FILE* f);
int writeEmployeeArrToTextFile(const Artist* pArtist, FILE* f);
int writeMerchArrToTextFile(const Artist* pArtist, FILE* f);
int writeConcertArrToTextFile(const Artist* pArtist, FILE* f);
int writeScheduleToTextFile(const Artist* pArtist, FILE* f);
int readArtistNameFromTextFile(FILE* f, Artist* newArtist);
int readArtistMembersFromTextFile(FILE* f, Artist* newArtist);
int readSongListFromTextFile(FILE* f, Artist* newArtist);
int readEmployeeArrFromTextFile(FILE* f, Artist* newArtist);
int readMerchArrFromTextFile(FILE* f, Artist* newArtist);
int readConcertArrFromTextFile(FILE* f, Artist* newArtist);
int readScheduleFromTextFile(FILE* f, Artist* newArtsit);
int addConcertToEventList(Artist* pArtist, Concert* pConcert, int CurrentNumOfConcerts);
int compareArtistsByName(const void* v1, const void* v2);
int compareArtistsByRevenue(const void* v1, const void* v2);
int compareArtistsByContractEndDate(const void* v1, const void* v2);
int saveArtistToBinFile(const Artist* pArtist, FILE* f);
int saveMembersToBinFile(const Artist* pArtist, FILE* f);
int saveSongListToBinFile(const Artist* pArtist, FILE* f);
int saveEmployeeListToBinFile(const Artist* pArtist, FILE* f);
int saveMerchArrToBinFile(const Artist* pArtist, FILE* f);
int saveConcertArrToBinFile(const Artist* pArtist, FILE* f);
int saveScheduleToBinFile(const Artist* pArtist, FILE* f);
int loadMembersFromBinFile(Artist* newArtist, FILE* f);
int loadSongListFromBinFile(Artist* newArtist, FILE* f);
int loadEmployeeListFromBinFile(Artist* newArtist, FILE* f);
int loadMerchArrFromBinFile(Artist* newArtist, FILE* f);
int loadConcertArrFromBinFile(Artist* newArtist, FILE* f);
int loadScheduleFromBinFile(Artist* newArtist, FILE* f);
int calcOverallPlaycount(const Artist* pArtist);
int calcOverallTicketsSold(const Artist* pArtist);
void printGeneralInfo(const void* v);
void printArtist(const Artist* pArtist);
void printSolo(const Artist* pArtist);
void printBand(const Artist* pArtist);
void printArtistEmployees(const Artist* pArtist);
void printArtistMerch(const Artist* pArtist);
void printArtistConcerts(const Artist* pArtist);
void printArtistMenu();
void freeArtist(Artist* pArtist);
void freeSolo(Artist* pArtist);
void freeBand(Artist* pArtist);
void renewContract(Artist* pArtist);
void doActionOnArtist(Artist* pArtist);
NODE* findEventInsertPosition(LIST* schedule, Event* newEvent);
LIST* generateListFromArr(const Employee* arr, int amount);
LIST* duplicateList(const LIST* source);
double calcSongsRevenue(const Artist* pArtist);
double calcArtistRevenueForCompany(const Artist* pArtist);
Artist* readArtistFromTextFile(FILE* f);
Artist* failedToReadArtistFromFile(Artist* pArtist, int step);
Artist* loadArtistFromBinFile(FILE* f);