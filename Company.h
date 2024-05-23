#pragma once
#define MANAGEMENT_FEE 2000
#define BIN_FILE_NAME "company.bin"
#define TEXT_FILE_NAME "company.txt"
#include "Artist.h"
#include "Studio.h"
typedef enum {eNameSorted, eRevenueSorted, eDateSorted, eNumOfSorts, eUnsorted} sortType;
static const char* sortTypeStr[eNumOfSorts] = { "name", "revenue for company", "contract end date"};


typedef struct {
	char* name;
	Artist** artistArr;
	int artistAmount;
	Studio** studioArr;
	int studioAmount;
	sortType artistArrSortType;
} Company;

int initCompanyFromFile(Company* newCompany);
int initCompany(Company* pComp);
int freeArtistArr(Company* pComp, int numOfArtists);
int freeStudioArr(Company* pComp, int numOfStudios);
int addNewArtist(Company* pComp);
int addNewStudio(Company* pComp);
int readCompanyNameFromTextFile(FILE* f, Company* newCompany);
int readStudioArrFromTextFile(FILE* f, Company* newCompany);
int readArtistArrFromTextFile(FILE* f, Company* newCompany);
int writeCompanyToTextFile(const Company* pComp);
int saveCompanyToBinFile(const Company* pComp);
int loadstudioArrFromBinFile(Company* newCompany, FILE* f);
int loadArtistArrFromBinFile(Company* newCompany, FILE* f);
int createRecording(Company* pComp);
int addRecordingToArtist(Artist* artist, const Studio* studio);
int loadCompanyFromBinFile(FILE* f, Company* newCompany);
int readCompanyFromTextFile(FILE* f, Company* newCompany);
void freeCompany(Company* pComp);
void failedReadingCompanyFromTextFile(Company* pComp, int step);
void sortArtistArr(Company* pComp);
void searchArtist(const Company* pComp);
void printCompany(const Company* pComp);
void artistAwards(const Company* pComp);
double calcCompanyRevenue(const Company* pComp);
Studio* chooseStudio(const Company* pComp);
Artist* chooseArtist(const Company* pComp);
Artist* getMostTrending(const Company* pComp, int* mostSongs);
Artist* getMostTicketsSold(const Company* pComp, int* mostTicketsSold);
Artist* getArtistOfTheYear(const Company* pComp, double* mostRevenue);
Artist** searchByName(const Company* pComp);
Artist** searchByRevenue(const Company* pComp);
Artist** searchByDate(const Company* pComp);