#include "Song.h"

int initSong(Song* pSong)
{
	pSong->name = getName("Enter song name:");
	if (!pSong->name) return 0;
	pSong->genre = getSelectionFromEnumList(SongGenreStr, eNumOfGenres, "Choose the song genre:", "Invalid genre selected, please try again:");
	pSong->playCount = getNonNegativeNumber("Enter the song's play count:");
	return 1;
}


void printSong(const Song* pSong)
{
	printf("Song name: %s \t Genre: %s \t PlayCount: %d", 
		pSong->name, SongGenreStr[pSong->genre], pSong->playCount);
}

void freeSong(Song* pSong)
{
	free(pSong->name);
	free(pSong);
}

double calculateSongIncomeForCompany(const Song* pSong)
{
	int income = pSong->playCount * INCOME_PER_PLAY;
	if (pSong->playCount < FIRST_THRESH)  //less than 1st threshold
		return income * FIRST_THRESH_PERCENTAGE;
	if (pSong->playCount < SECOND_THRESH) // between 1st and 2nd threshold
		return income * SECOND_THRESH_PERCENTAGE;
	if (pSong->playCount < THIRD_THRESH) // between 2nd and 3rd threshold
		return income * THIRD_THRESH_PERCENTAGE;
	if (pSong->playCount < FOURTH_THRESH) // between 3rd and 4th threshold
		return income * FOURTH_THRESH_PERCENTAGE;
	return income * LOWEST_THRESH_PERCENTAGE; //greater than 4th threshold
}

int writeSongToTextFile(const Song* pSong, FILE* f) {
	if (!f)
		return 0;
	fprintf(f, "%s\n", pSong->name);
	fprintf(f, "%d %d\n", pSong->genre, pSong->playCount);
	return 1;
}

Song* readSongFromTextFile(FILE* f)
{
	if (!f) return NULL;
	Song* newSong = (Song*)malloc(sizeof(Song));
	if (!newSong) return NULL;
	char buffer[MAX_STR_LEN];
	myGets(buffer, sizeof(buffer), f);
	newSong->name = _strdup(buffer);
	if (fscanf(f, "%d\n", &newSong->genre) != 1) {
		free(newSong->name);
		return NULL;
	}
	if (fscanf(f, "%d\n", &newSong->playCount) != 1) {
		free(newSong->name);
		return NULL;
	}
	return newSong;
}

int saveSongToBinFile(const Song* pSong, FILE* f)
{
	CHECK_VALIDITY(f, 0);
	CHECK_VALIDITY(pSong, 0);
	CHECK_VALIDITY(saveStringToBinFile(pSong->name, f), 0); //writes name
	CHECK_VALIDITY(fwrite(&pSong->genre, sizeof(int), 1, f) == 1, 0); //writes genre
	CHECK_VALIDITY(fwrite(&pSong->playCount, sizeof(int), 1, f) == 1, 0); //writes playCount
	return 1;
}
Song* loadSongFromBinFile(FILE* f)
{
	CHECK_VALIDITY(f, NULL);
	char* name = loadStringFromBinFile(f); //reads name
	CHECK_VALIDITY(name, NULL);
	Song* newSong = (Song*)malloc(sizeof(Song));
	FREE_IF_FAILED(newSong, name, NULL);
	newSong->name = name;
	if (fread(&newSong->genre, sizeof(int), 1, f) != 1 //reads genre
		|| fread(&newSong->playCount, sizeof(int), 1, f) != 1) //reads playCount
	{
		freeSong(newSong);
		free(newSong);
		return NULL;
	}
	return newSong;

}

Song* copySong(const Song* pSong) {
	Song* newSong = (Song*)malloc(sizeof(Song));
	CHECK_VALIDITY(newSong, NULL);
	newSong->name = _strdup(pSong->name);
	newSong->genre = pSong->genre;
	newSong->playCount = pSong->playCount;
	return newSong;
}
