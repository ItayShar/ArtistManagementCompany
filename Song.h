#pragma once
#define INCOME_PER_PLAY 3
#define FIRST_THRESH 100000 // 100K
#define SECOND_THRESH 1000000 //1M
#define THIRD_THRESH 10000000 //10M
#define FOURTH_THRESH 100000000 // 100M
#define FIRST_THRESH_PERCENTAGE 0.7
#define SECOND_THRESH_PERCENTAGE 0.6
#define THIRD_THRESH_PERCENTAGE 0.5
#define FOURTH_THRESH_PERCENTAGE 0.4
#define LOWEST_THRESH_PERCENTAGE 0.15
#include "General.h"
typedef enum { eRock, eRap, ePop, eNumOfGenres } eGenre;
static const char* SongGenreStr[eNumOfGenres]= { "Rock", "Rap", "Pop" };

typedef struct {
	char* name;
	eGenre genre;
	int playCount;
} Song;

int initSong(Song* pSong);
int saveSongToBinFile(const Song* pSong, FILE* f);
int writeSongToTextFile(const Song* pSong, FILE* f);
void printSong(const Song* pSong);
void freeSong(Song* pSong);
Song* readSongFromTextFile(FILE* f);
Song* loadSongFromBinFile(FILE* f);
Song* copySong(const Song* pSong);
double calculateSongIncomeForCompany(const Song* pSong);