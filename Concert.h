#pragma once
#include "Song.h"
#include "Stall.h"
#include "list.h"
#include "Date.h"
typedef struct {
	Song** songArr;
	int songArrSize;
	Stall** stallArr;
	int stallArrSize;
	int ticketPrice;
	int amountOfSoldTickets;
	Date * date;
} Concert;

int initConcert(Concert* pConcert, LIST* artistSongs, int amountOfArtistSongs, LIST* artistEmployees, int amountOfEmployees, const Merch** merchArr, int merchAmount, const Date* maxDate);
int getSongsFromArtist(Concert* pConcert, LIST * artistSongs, int amountOfArtistSongs);
int	getStalls(Concert* pConcert, LIST * artistEmployees, int employeeAmount, const Merch** merchArr, int merchAmount);
int	calcConcertRevenue(const Concert* pConcert);
int	freeStalls(Concert* pConcert, int numOfStalls);
int writeConcertToTextFile(const Concert* pConcert, FILE* f);
int writeSongArrToTextFile(const Concert* pConcert, FILE* f);
int writeStallArrToTextFile(const Concert* pConcert, FILE* f);
int readSongArrFromTextFile(Concert* pConcert, FILE* f);
int readStallArrFromTextFile(Concert* pConcert, FILE* f);
int saveConcertToBinFile(const Concert* pConcert, FILE* f);
int loadSongArrFromBinFile(Concert* newConcert, FILE* f);
int loadStallArrFromBinFile(Concert* newConcert, FILE* f);
void getNumberOfTicketsAndPrice(Concert* pConcert);
void printConcert(const void* v);
void freeConcert(Concert* pConcert);
void freeSongs(Concert* pConcert, int numOfStalls);
Concert* readConcertFromTextFile(FILE* f);
Concert* loadConcertFromBinFile(FILE* f);