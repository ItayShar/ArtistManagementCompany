#include "Concert.h"

int initConcert(Concert* pConcert, LIST * artistSongs, int amountOfArtistSongs, LIST * artistEmployees, int amountOfEmployees, const Merch ** merchArr, int merchAmount, const Date * maxDate)
{
	if (!getSongsFromArtist(pConcert, artistSongs, amountOfArtistSongs))
		return 0;
	if (!getStalls(pConcert, artistEmployees, amountOfEmployees, merchArr, merchAmount))
		return 0;
	getNumberOfTicketsAndPrice(pConcert);
	Date* concertDate = (Date*)malloc(sizeof(Date));
	if (!concertDate) return 0;
	while (1) {
		printf("Enter concert date:");
		initDate(concertDate);
		if (compareDates(concertDate, maxDate) == 1) // if concertDate > maxDate
		{
			printf("Please enter a date prior to ");
			printDate(maxDate);
			printf("\n");
			continue;
		}
		break;
	}
	pConcert->date = concertDate;
	return 1;
}

int getSongsFromArtist(Concert* pConcert, LIST * artistSongs, int amountOfArtistSongs) {
	int songChoice, numOfArtistSongs;
	printf("Artist has %d songs\n", amountOfArtistSongs);
	pConcert->songArrSize = getNumberInRange(1, amountOfArtistSongs, "Enter the amount of songs in the concert:");
	pConcert->songArr = (Song**)malloc(sizeof(Song*) * pConcert->songArrSize);
	if (!pConcert->songArr) return 0;
	for (int i = 0; i < pConcert->songArrSize; i++) {
		printf("Choose a song from the list to add:\n");
		while (1)
		{
			numOfArtistSongs = printLinkedListInRows(artistSongs, printSong);
			//print all songs from ArtistSongs. should be numberd 1 to n 
			scanf("%d", &songChoice);
			if (songChoice < 1 || songChoice > numOfArtistSongs) {
				printf("Invalid song selection, please try again:\n");
				continue;
			}
			break;
		}
		NODE* tmpNode = removeNodeFromListByIndex(artistSongs, songChoice);
		pConcert->songArr[i] = (Song*)tmpNode->key;
		free(tmpNode);
	}
	return 1;
}

int getStalls(Concert* pConcert, LIST * artistEmployees, int employeeAmount, const Merch** merchArr, int merchAmount)
{
	if (merchAmount == 0) {
		pConcert->stallArrSize = 0;
		pConcert->stallArr = (Stall**)malloc(sizeof(Stall*) * pConcert->stallArrSize); // in order to free later in freeStalls
		return 1; //If there's no merch to sell there's nothing to sell on the stalls, therefore no stalls should be created
	}
	Stall* pStall;
	printf("Artist has %d employees\n", employeeAmount);
	pConcert->stallArrSize = getNumberInRange(0, employeeAmount, "Enter amount of stalls in the concert:");
	pConcert->stallArr = (Stall**)malloc(sizeof(Stall*) * pConcert->stallArrSize);
	if (!pConcert->stallArr) return 0;
	for (int i = 0; i < pConcert->stallArrSize; i++) {
		pStall = (Stall*)malloc(sizeof(Stall));
		if (!pStall) return freeStalls(pConcert, i);
		if (!initStall(pStall, artistEmployees, merchArr, merchAmount)) {
			free(pStall);
			return freeStalls(pConcert, i);
		}
		pConcert->stallArr[i] = pStall;
	}
	return 1;
}

void getNumberOfTicketsAndPrice(Concert* pConcert)
{
	pConcert->amountOfSoldTickets = getNonNegativeNumber("Enter number of sold tickets for the concert:");
	pConcert->ticketPrice = getNumberGreaterThan(0, "Enter price per ticket:");
}

void printConcert(const void* v)
{
	const Concert* pConcert = *(const Concert**) v;
	printDate(pConcert->date);
	printf(" Playlist:\n");
	for (int i = 0; i < pConcert->songArrSize; i++) {
		printSong(pConcert->songArr[i]);
		printf("\n");
	}
	if(pConcert->stallArrSize > 0){
		printf("Concert stalls:\n");
		generalArrayFunction(pConcert->stallArr, pConcert->stallArrSize, sizeof(Stall*), printStall);
		printf("\n");
	}
	printf("Tickets sold: %d \t Price per Ticket: %d\n\n", pConcert->amountOfSoldTickets, pConcert->ticketPrice);
}
void freeConcert(Concert* pConcert)
{
	freeSongs(pConcert, pConcert->songArrSize);
	freeStalls(pConcert, pConcert->stallArrSize); //if there are no stalls there's nothing to free
	free(pConcert->date);
	free(pConcert);
}

int freeStalls(Concert* pConcert, int numOfStalls)
{
	for (int i = 0; i < numOfStalls; i++)
		freeStall(pConcert->stallArr[i]);
	free(pConcert->stallArr);
	return 0;
}

void freeSongs(Concert* pConcert, int numOfSongs)
{
	for (int i = 0; i < numOfSongs; i++)
		freeSong(pConcert->songArr[i]);
	free(pConcert->songArr);
}
int	 calcConcertRevenue(const Concert* pConcert)
{
	int revenue = 0;
	for (int i = 0; i < pConcert->stallArrSize; i++)
		revenue += calcStallRevenue(pConcert->stallArr[i]);
	revenue += pConcert->amountOfSoldTickets * pConcert->ticketPrice;
	return revenue;
}

int writeConcertToTextFile(const Concert* pConcert, FILE* f)
{
	if (!f) return 0;
	if (!writeSongArrToTextFile(pConcert, f)) return 0;
	if (!writeStallArrToTextFile(pConcert, f)) return 0;
	fprintf(f, "%d %d\n", pConcert->amountOfSoldTickets, pConcert->ticketPrice);
	if (!writeDateToTextFile(pConcert->date, f)) return 0;
	return 1;
}

int writeSongArrToTextFile(const Concert* pConcert, FILE* f)
{
	if (!f) return 0;
	fprintf(f, "%d\n", pConcert->songArrSize);
	for (int i = 0; i < pConcert->songArrSize; i++)
		writeSongToTextFile(pConcert->songArr[i], f);
	return 1;
}

int writeStallArrToTextFile(const Concert* pConcert, FILE* f)
{
	if (!f) return 0;
	fprintf(f, "%d\n", pConcert->stallArrSize);
	for (int i = 0; i < pConcert->stallArrSize; i++)
		writeStallToTextFile(pConcert->stallArr[i], f);
	return 1;
}

int readSongArrFromTextFile(Concert * pConcert, FILE* f)
{
	if (fscanf(f, "%d\n", &pConcert->songArrSize) != 1) return 0;
	Song** songArr = (Song**)malloc(sizeof(Song*) * pConcert->songArrSize);
	if (!songArr) return 0;
	pConcert->songArr = songArr;
	for (int i = 0; i < pConcert->songArrSize; i++) {
		pConcert->songArr[i] = readSongFromTextFile(f);
		if (!pConcert->songArr[i]) {
			freeSongs(pConcert, i);
			return 0;
		}
	}
	return 1;
}

int readStallArrFromTextFile(Concert* pConcert, FILE* f)
{
	if (fscanf(f, "%d\n", &pConcert->stallArrSize) != 1) return 0;
	Stall** stallArr = (Stall**)malloc(sizeof(Stall*) * pConcert->stallArrSize);
	if (!stallArr) return 0;
	pConcert->stallArr = stallArr;
	for (int i = 0; i < pConcert->stallArrSize; i++) {
		pConcert->stallArr[i] = readStallFromTextFile(f);
		if (!pConcert->stallArr[i]) {
			freeStalls(pConcert, i);
			return 0;
		}
	}
	return 1;
}

Concert* readConcertFromTextFile(FILE* f)
{
	if (!f) return NULL;
	Concert* newConcert = (Concert*)malloc(sizeof(Concert));
	if (!newConcert) return NULL;

	if (!readSongArrFromTextFile(newConcert, f)) {
		freeConcert(newConcert);
		free(newConcert);
		return 0;
	}
	if (!readStallArrFromTextFile(newConcert, f)) {
		freeConcert(newConcert);
		free(newConcert);
		return 0;
	}
	if (fscanf(f, "%d %d\n", &newConcert->amountOfSoldTickets, &newConcert->ticketPrice) != 2) {
		freeConcert(newConcert);
		free(newConcert);
		return NULL;
	}
	newConcert->date = readDateFromTextFile(f);
	if (!newConcert->date) {
		freeConcert(newConcert);
		free(newConcert);
		return NULL;
	}
	return newConcert;
}

int saveConcertToBinFile(const Concert* pConcert, FILE* f)
{
	CHECK_VALIDITY(f, 0);
	CHECK_VALIDITY(pConcert, 0);
	CHECK_VALIDITY(fwrite(&pConcert->songArrSize, sizeof(int), 1, f) == 1, 0); // writes songArrSize
	CHECK_VALIDITY(fwrite(&pConcert->amountOfSoldTickets, sizeof(int), 1, f) == 1, 0); // writes amountOfSoldTickets
	CHECK_VALIDITY(fwrite(&pConcert->ticketPrice, sizeof(int), 1, f) == 1, 0); // writes ticketPrice
	CHECK_VALIDITY(fwrite(pConcert->date, sizeof(Date), 1, f) == 1, 0); // writes date
	CHECK_VALIDITY(fwrite(&pConcert->stallArrSize, sizeof(int), 1, f) == 1, 0); // writes stallArrSize
	for (int i = 0; i < pConcert->songArrSize; i++) // writes songArr
		CHECK_VALIDITY(saveSongToBinFile(pConcert->songArr[i], f), 0);
	for (int i = 0; i < pConcert->stallArrSize; i++) // writes stallArr
		CHECK_VALIDITY(saveStallToBinFile(pConcert->stallArr[i], f), 0);
	return 1;

}
Concert* loadConcertFromBinFile(FILE* f)
{
	CHECK_VALIDITY(f, NULL);
	Concert* newConcert = (Concert*)malloc(sizeof(Concert));
	CHECK_VALIDITY(newConcert, NULL);
	newConcert->date = (Date*)malloc(sizeof(Date));
	EXEC_IF_FAILED(newConcert->date, free(newConcert), NULL);
	EXEC_IF_FAILED(fread(&newConcert->songArrSize, sizeof(int), 1, f) == 1, free(newConcert->date);free(newConcert), NULL); //reads songArrSize
	EXEC_IF_FAILED(fread(&newConcert->amountOfSoldTickets, sizeof(int), 1, f) == 1, free(newConcert->date); free(newConcert), NULL); //reads amountOfSoldTickets
	EXEC_IF_FAILED(fread(&newConcert->ticketPrice, sizeof(int), 1, f) == 1, free(newConcert->date); free(newConcert), NULL); //reads ticketPrice
	EXEC_IF_FAILED(fread(newConcert->date, sizeof(Date), 1, f) == 1, free(newConcert->date); free(newConcert), NULL); //reads date
	EXEC_IF_FAILED(fread(&newConcert->stallArrSize, sizeof(int), 1, f) == 1, free(newConcert->date); free(newConcert), NULL); //reads stallArrSize
	newConcert->songArr = (Song**)malloc(newConcert->songArrSize * sizeof(Song*));
	if (!newConcert->songArr || !loadSongArrFromBinFile(newConcert, f)) { //reads songArr
		free(newConcert->date);
		free(newConcert);
		return NULL;
	}
	newConcert->stallArr = (Stall**)malloc(newConcert->stallArrSize * sizeof(Stall*));
	if (!newConcert->stallArr || !loadStallArrFromBinFile(newConcert, f)) { //reads stallArr
		free(newConcert->date);
		freeSongs(newConcert, newConcert->songArrSize);
		free(newConcert);
		return NULL;
	}
	return newConcert;
}


int loadSongArrFromBinFile(Concert* newConcert, FILE* f)
{
	CHECK_VALIDITY(f, 0);
	CHECK_VALIDITY(newConcert, 0);
	Song* newSong;
	for (int i = 0; i < newConcert->songArrSize; i++) {
		newSong = loadSongFromBinFile(f);
		if (!newSong) {
			freeSongs(newConcert, i);
			return 0;
		}
		newConcert->songArr[i] = newSong;
	}
	return 1;
}

int loadStallArrFromBinFile(Concert* newConcert, FILE* f)
{
	Stall* newStall;
	CHECK_VALIDITY(f, 0);
	CHECK_VALIDITY(newConcert, 0);
	for (int i = 0; i < newConcert->stallArrSize; i++) {
		newStall = loadStallFromBinFile(f);
		if (!newStall) {
			freeStalls(newConcert, i);
			return 0;
		}
		newConcert->stallArr[i] = newStall;
	}
	return 1;
}