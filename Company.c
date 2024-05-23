#include "Company.h"
int initCompany(Company* pComp)
{
	pComp->name = getName("Enter company name:");
	pComp->artistAmount = 0;
	pComp->studioAmount = 0;
	//Allocation for enabling reallocating in addNewArtist and addNewStudio
	pComp->artistArr = (Artist**)malloc(0);
	pComp->studioArr = (Studio**)malloc(0);
	pComp->artistArrSortType = eUnsorted;
	return 1;
}

void freeCompany(Company* pComp)
{
	free(pComp->name);
	freeArtistArr(pComp, pComp->artistAmount);
	freeStudioArr(pComp, pComp->studioAmount);
}
int freeArtistArr(Company* pComp, int numOfArtists)
{
	for (int i = 0; i < numOfArtists; i++) {
		freeArtist(pComp->artistArr[i]);
		free(pComp->artistArr[i]);
	}
	free(pComp->artistArr);
	return 0;
}
int freeStudioArr(Company* pComp, int numOfStudios)
{
	for (int i = 0; i < numOfStudios; i++)
		freeStudio(pComp->studioArr[i], pComp->studioArr[i]->gearArrSize);
	free(pComp->studioArr);
	return 0;
}

int addNewArtist(Company* pComp)
{
	Artist* newArtist = (Artist *)malloc(sizeof(Artist));
	if (!newArtist) return 0;
	if (!initArtist(newArtist)) {
		free(newArtist);
		return 0;
	}
	Artist** tmpPtr = (Artist**)realloc(pComp->artistArr, sizeof(Artist*) * (pComp->artistAmount + 1));
	if (!tmpPtr) {
		free(newArtist);
		return 0;
	}
	pComp->artistArr = tmpPtr;
	pComp->artistArr[pComp->artistAmount] = newArtist;
	pComp->artistAmount++;
	pComp->artistArrSortType = eUnsorted;
	return 1;
}

int addNewStudio(Company* pComp)
{
	Studio* newStudio = (Studio*)malloc(sizeof(Studio));
	CHECK_VALIDITY(newStudio, 0);
	if (!initStudio(newStudio, pComp->studioArr	, pComp->studioAmount)) {
		free(newStudio);
		return 0;
	}
	Studio** tmpPtr = (Studio**)realloc(pComp->studioArr, sizeof(Studio*) * (pComp->studioAmount + 1));
	if (!tmpPtr) {
		free(newStudio);
		return 0;
	}
	pComp->studioArr = tmpPtr;
	pComp->studioArr[pComp->studioAmount] = newStudio;
	pComp->studioAmount++;
	return 1;
}


int writeCompanyToTextFile(const Company* pComp)
{
	FILE* f = fopen(TEXT_FILE_NAME, "w");
	if (!f) return 0;
	fprintf(f, "%s\n", pComp->name);
	//writing artistArr
	fprintf(f, "%d\n", pComp->artistAmount);
	for (int i = 0; i < pComp->artistAmount; i++)
		if (!writeArtistToTextFile(pComp->artistArr[i], f)) return 0;
	//wrtiting studioArr
	fprintf(f, "%d\n", pComp->studioAmount);
	for (int i = 0; i < pComp->studioAmount; i++)
		if (!writeStudioToTextFile(pComp->studioArr[i], f)) return 0;
	fprintf(f, "%d\n", pComp->artistArrSortType);
	fclose(f);
	return 1;
}

int readCompanyFromTextFile(FILE* f, Company* newCompany)
{
	CHECK_VALIDITY(f, 0);
	CHECK_VALIDITY(newCompany, 0);
	if (!readCompanyNameFromTextFile(f, newCompany)) {
		return 0;
	}
	if (!readArtistArrFromTextFile(f, newCompany)) { //reads artistArr
		failedReadingCompanyFromTextFile(newCompany, 1); //frees name
		return 0;
	}
	if (!readStudioArrFromTextFile(f, newCompany) ){
		failedReadingCompanyFromTextFile(newCompany, 2); //fress name, artistArr
		return 0;
	}
	if (fscanf(f, "%d\n", &newCompany->artistArrSortType) != 1) {
		failedReadingCompanyFromTextFile(newCompany, 3); //frees name, artistArr, studioArr
		return 0;
	}
	fclose(f);
	return 1;
}

int readCompanyNameFromTextFile(FILE* f, Company* newCompany)
{
	CHECK_VALIDITY(f, 0);
	CHECK_VALIDITY(newCompany, 0);
	char buffer[MAX_STR_LEN];
	myGets(buffer, sizeof(buffer), f); //reads name
	newCompany->name = _strdup(buffer);
	return 1;
}

int readArtistArrFromTextFile(FILE* f, Company* newCompany)
{
	CHECK_VALIDITY(f, 0);
	Artist* newArtist;
	if (fscanf(f, "%d\n", &newCompany->artistAmount) != 1) return 0;
	newCompany->artistArr = (Artist**)malloc(sizeof(Artist*) * newCompany->artistAmount);
	if (!newCompany->artistArr) return 0;
	for (int i = 0; i < newCompany->artistAmount; i++) {
		newArtist = readArtistFromTextFile(f);
		if (!newArtist) return freeArtistArr(newCompany, i); //frees all artists that were allocated till failure
		newCompany->artistArr[i] = newArtist;
	}
	return 1;
}

int readStudioArrFromTextFile(FILE* f, Company* newCompany)
{
	if (!f) return 0;
	Studio* newStudio;
	if (fscanf(f, "%d\n", &newCompany->studioAmount) != 1) return 0;
	newCompany->studioArr = (Studio**)malloc(sizeof(Studio*) * newCompany->studioAmount);
	if (!newCompany->studioArr) return 0;
	for (int i = 0; i < newCompany->studioAmount; i++) {
		newStudio = readStudioFromTextFile(f);
		if (!newStudio) return freeStudioArr(newCompany, i); //frees all studios that were allocated till failure
		newCompany->studioArr[i] = newStudio;
	}
	return 1;
}

void printCompany(const Company* pComp)
{
	if (!pComp) return;
	printf("company: %s\n", pComp->name);
	if (pComp->artistAmount > 0) {
		printf("Artists:\n"); printf("--------\n");
	}
	generalArrayFunction(pComp->artistArr, pComp->artistAmount, sizeof(Artist*), printGeneralInfo); //prints artistArr
	if (pComp->studioAmount > 0) {
		printf("Studios:\n"); printf("--------\n");
	}
	generalArrayFunction(pComp->studioArr, pComp->studioAmount, sizeof(Studio*), printStudio); //prints studioArr
	printf("\n");
}

void failedReadingCompanyFromTextFile(Company* pComp, int step)
{
	if (step >= 1) //frees name
		free(pComp->name);
	if (step >= 2) //frees artistArr
		freeArtistArr(pComp, pComp->artistAmount);
	if (step >= 3) //fress studioArr
		freeStudioArr(pComp, pComp->studioAmount);
}

void sortArtistArr(Company* pComp)
{
	if (pComp->artistAmount == 0) {
		printf("Company does not have any artists. Add an artist to use this feature\n");
		return;
	}
	sortType sortType = getSelectionFromEnumList(sortTypeStr, eNumOfSorts, "Choose field to sort artists by:", "Invalid input was entered, please try again:");
	if (sortType == eNameSorted)
		qsort(pComp->artistArr, pComp->artistAmount, sizeof(Artist*), compareArtistsByName);
	if (sortType == eRevenueSorted)
		qsort(pComp->artistArr, pComp->artistAmount, sizeof(Artist*), compareArtistsByRevenue);
	if (sortType == eDateSorted)
		qsort(pComp->artistArr, pComp->artistAmount, sizeof(Artist*), compareArtistsByContractEndDate);
	pComp->artistArrSortType = sortType;
}

void searchArtist(const Company* pComp) {
	if (pComp->artistAmount == 0) {
		printf("Company does not have any artists. Add an artist to use this feature\n");
		return;
	}
	Artist** searchRes = NULL;
	if (pComp->artistArrSortType == eUnsorted) {
		printf("Artists array is not sorted, please sort it first.\n");
		return;
	}
	if (pComp->artistArrSortType == eNameSorted)
		searchRes = searchByName(pComp);
	if (pComp->artistArrSortType == eRevenueSorted)
		searchRes = searchByRevenue(pComp);
	if (pComp->artistArrSortType == eDateSorted)
		searchRes = searchByDate(pComp);
	if (!searchRes)
		printf("Artist not found\n\n");
	else
	{
		printf("\nArtist found:\n\n");
		printArtist(*searchRes);
	}
}

Artist** searchByName(const Company* pComp)
{
	Artist search;
	char* name = getName("Enter artist name to search:");
	if (!name) return NULL;
	search.name = name;
	Artist* toSearch = &search;
	Artist ** returnVal =  (Artist**)bsearch(&toSearch, pComp->artistArr, pComp->artistAmount, sizeof(Artist*), compareArtistsByName);
	free(name);
	return returnVal;
}


Artist** searchByRevenue(const Company* pComp)
{
	Artist search;
	printf("Enter artist revenue to search:\n");
	scanf("%lf", &search.revenue);
	Artist* toSearch = &search;
	return (Artist**)bsearch(&toSearch, pComp->artistArr, pComp->artistAmount, sizeof(Artist*), compareArtistsByRevenue);
}
Artist** searchByDate(const Company* pComp)
{
	Artist search;
	Date d;
	printf("Enter artist contract end date to search:\n");
	initDate(&d);
	search.contractExpireDate = &d;
	Artist* toSearch = &search;
	return (Artist**)bsearch(&toSearch, pComp->artistArr, pComp->artistAmount, sizeof(Artist*), compareArtistsByContractEndDate);
}

double calcCompanyRevenue(const Company* pComp)
{
	printf("\nThe company revenue is calculated as follows:\n");
	printf("Firstly, we sum up the revenue of all the artists in the company.\n");
	printf("The revenue of an artist for the company consists of the share the company takes from each song income (based on it's playcount),\n");
	printf("the income from concert ticket sales and income from stalls purchases (after substracting employee expenses).\n");
	printf("Afterwards, we substract the cost of all the studios the company owns. A studio cost consists of the rent and the cost of all the gear in the studio.\n");
	printf("Finally, we add to the revenue the management fee for each of the artists, which is a set amount.\n");
	double revenue = 0;
	for (int i = 0; i < pComp->artistAmount; i++)
		revenue += calcArtistRevenueForCompany(pComp->artistArr[i]) + MANAGEMENT_FEE;
	for (int i = 0; i < pComp->studioAmount; i++)
		revenue -= calcStudioCost(pComp->studioArr[i]);
	return revenue;
}


int saveCompanyToBinFile(const Company* pComp)
{
	FILE* f = fopen(BIN_FILE_NAME, "wb");
	CHECK_VALIDITY(f, 0);
	CHECK_VALIDITY(pComp, 0);
	CHECK_VALIDITY(fwrite(&pComp->artistArrSortType, sizeof(int), 1, f) == 1, 0); //writes current sort type of ArtistArr
	CHECK_VALIDITY(fwrite(&pComp->artistAmount, sizeof(int), 1, f) == 1, 0); //writes artistAmount
	CHECK_VALIDITY(fwrite(&pComp->studioAmount, sizeof(int), 1, f) == 1, 0); //writes studioAmount
	CHECK_VALIDITY(saveStringToBinFile(pComp->name, f), 0); //writes name
	for (int i = 0; i < pComp->artistAmount; i++) //writes artists
		CHECK_VALIDITY(saveArtistToBinFile(pComp->artistArr[i], f), 0);
	for (int i = 0; i < pComp->studioAmount; i++) //writes studios
		CHECK_VALIDITY(saveStudioToBinFile(pComp->studioArr[i], f), 0);
	fclose(f);
	return 1;
}
int loadCompanyFromBinFile(FILE* f, Company * newCompany)
{
	CHECK_VALIDITY(f, 0);
	CHECK_VALIDITY(newCompany, 0);
	CHECK_VALIDITY(fread(&newCompany->artistArrSortType, sizeof(int), 1, f) == 1, 0); //reads sort type
	CHECK_VALIDITY(fread(&newCompany->artistAmount, sizeof(int), 1, f) == 1, 0); //reads artistAmount
	CHECK_VALIDITY(fread(&newCompany->studioAmount, sizeof(int), 1, f) == 1, 0); //reads studioAmount
	newCompany->name = loadStringFromBinFile(f); //reads name
	CHECK_VALIDITY(newCompany->name, 0);
	newCompany->artistArr = (Artist**)malloc(sizeof(Artist*) * newCompany->artistAmount);
	EXEC_IF_FAILED(newCompany->artistArr, free(newCompany->name), 0);
	EXEC_IF_FAILED(loadArtistArrFromBinFile(newCompany, f), free(newCompany->name), 0); //reads artistArr
	newCompany->studioArr = (Studio**)malloc(sizeof(Studio*) * newCompany->studioAmount);
	EXEC_IF_FAILED(newCompany->studioArr, freeArtistArr(newCompany, newCompany->artistAmount); free(newCompany->name), 0);
	EXEC_IF_FAILED(loadstudioArrFromBinFile(newCompany, f), freeArtistArr(newCompany, newCompany->artistAmount); free(newCompany->name), 0); //reads studioArr
	fclose(f);
	return 1;
}

int loadArtistArrFromBinFile(Company* newCompany, FILE* f)
{
	Artist* newArtist;
	CHECK_VALIDITY(f, 0);
	CHECK_VALIDITY(newCompany, 0);
	for (int i = 0; i < newCompany->artistAmount; i++) {
		newArtist = loadArtistFromBinFile(f);
		EXEC_IF_FAILED(newArtist, freeArtistArr(newCompany, i), 0);
		newCompany->artistArr[i] = newArtist;
	}
	return 1;
}

int loadstudioArrFromBinFile(Company* newCompany, FILE* f)
{
	Studio* newStudio;
	CHECK_VALIDITY(f, 0);
	CHECK_VALIDITY(newCompany, 0);
	for (int i = 0; i < newCompany->studioAmount; i++) {
		newStudio = loadStudioFromBinFile(f);
		EXEC_IF_FAILED(newStudio, freeStudioArr(newCompany, i), 0);
		newCompany->studioArr[i] = newStudio;
	}
	return 1;
}

int initCompanyFromFile(Company* newCompany)
{
	CHECK_VALIDITY(newCompany, 0);
	int option, res;
	printf("Choose one of the following:\n");
	printf("1. Load company from binary file\n");
	printf("2. Load company from text file\n");
	while (1) {
		scanf("%d", &option);
		if (option <= 0 || option > 2) {
			printf("Invalid input, please try again\n");
			continue;
		}
		break;
	}
	if (option == 1) { //binary
		FILE* f = fopen(BIN_FILE_NAME, "rb");
		res = loadCompanyFromBinFile(f, newCompany);
		if (res == 0) {
			printf("Loading company from binary file failed. Creating empty Company:\n");
			res = initCompany(newCompany);
			if (res == 0) return 0;
		}
	}
		else { //text
			FILE* f = fopen(TEXT_FILE_NAME, "r");
			res = readCompanyFromTextFile(f, newCompany);
			if (res == 0) {
				printf("Loading company from text file failed. Creating empty Company:\n");
				res = initCompany(newCompany);
				if (res == 0) return 0;
			}
		}
	return 1;
}

Artist* chooseArtist(const Company* pComp) {
	CHECK_VALIDITY(pComp, NULL);
	EXEC_IF_FAILED(pComp->artistAmount > 0, printf("There are no artists in the company\n\n"), NULL);
	int option;
	printf("Choose an artist:\n");
	while (1) {
		for (int i = 0; i < pComp->artistAmount; i++)
			printf("%d. %s\n", (i + 1), pComp->artistArr[i]->name);
		scanf("%d", &option);
		if (option <= 0 || option > pComp->artistAmount) {
			printf("Invalid selection. Please try again\n");
			continue;
		}
		break;
	}
	return pComp->artistArr[option - 1];
}

Studio* chooseStudio(const Company* pComp) {
	CHECK_VALIDITY(pComp, NULL);
	EXEC_IF_FAILED(pComp->studioAmount > 0, printf("There are no studios in the company\n"), NULL);
	int option;
	printf("Choose a studio:\n");
	while (1) {
		for (int i = 0; i < pComp->studioAmount; i++) {
			printf("%d. ", i + 1);
			printRegStudio(pComp->studioArr[i]);
		}
		scanf("%d", &option);
		if (option <= 0 || option > pComp->studioAmount) {
			printf("Invalid selection. Please try again\n");
			continue;
		}
		break;
	}
	return pComp->studioArr[option - 1];
}




void artistAwards(const Company* pComp)
{
	if (!pComp) return;
	if (pComp->artistAmount < 3) {
		printf("To use this option, please have a minimum of 3 artists\n");
		return;
	}
	Artist* artist;
	int mostSongs, mostTicketsSold;
	double mostRevenue;
	artist = getMostTrending(pComp, &mostSongs);
	printf("The most trending artist is %s with %d overall plays!\n", artist->name, mostSongs);
	artist = getMostTicketsSold(pComp, &mostTicketsSold);
	printf("The artist who sold the most tickets is %s with %d tickets sold!\n", artist->name, mostTicketsSold);
	artist = getArtistOfTheYear(pComp, &mostRevenue);
	printf("Artist of the year is %s! Revenue made for company: %.2lf\n", artist->name, mostRevenue);
	printf("\n");
}

Artist * getMostTrending(const Company* pComp, int * mostSongs) {
	//pcomp has at least 3 artists
	Artist* mostTrending = pComp->artistArr[0];
	*mostSongs = calcOverallPlaycount(pComp->artistArr[0]);
	int currentPlayCount;
	for (int i = 1; i < pComp->artistAmount; i++) {
		currentPlayCount = calcOverallPlaycount(pComp->artistArr[i]);
		if (currentPlayCount > *mostSongs) {
			mostTrending = pComp->artistArr[i];
			*mostSongs = currentPlayCount;
		}
	}
	return mostTrending;
}

Artist* getMostTicketsSold(const Company* pComp, int* mostTicketsSold) {
	//pcomp has at least 3 artists
	Artist * mostTickets = pComp->artistArr[0];
	*mostTicketsSold = calcOverallTicketsSold(pComp->artistArr[0]);
	int currentTicketsSold;
	for (int i = 1; i < pComp->artistAmount; i++) {
		currentTicketsSold = calcOverallTicketsSold(pComp->artistArr[i]);
		if (currentTicketsSold > *mostTicketsSold) {
			mostTickets = pComp->artistArr[i];
			*mostTicketsSold = currentTicketsSold;
		}
	}
	return mostTickets;
}

Artist* getArtistOfTheYear(const Company* pComp, double* mostRevenue)
{
	Artist* mostRev = pComp->artistArr[0];
	*mostRevenue = calcArtistRevenueForCompany(pComp->artistArr[0]);
	double CurrentRevenue;
	for (int i = 1; i < pComp->artistAmount; i++) {
		CurrentRevenue = calcArtistRevenueForCompany(pComp->artistArr[i]);
		if (CurrentRevenue > *mostRevenue) {
			mostRev = pComp->artistArr[i];
			*mostRevenue = CurrentRevenue;
		}
	}
	return mostRev;
}

int createRecording(Company* pComp) {
	CHECK_VALIDITY(pComp, 0);
	if (pComp->artistAmount == 0 || pComp->studioAmount == 0) {
		printf("Please have a minimum of 1 artist and 1 studio before using this feature\n");
		return 1;
	}
	Artist* artist = chooseArtist(pComp);
	CHECK_VALIDITY(artist, 0);
	Studio* studio = chooseStudio(pComp);
	CHECK_VALIDITY(studio, 0);
	CHECK_VALIDITY(addRecordingToArtist(artist, studio), 0);
	return 1;
}

int addRecordingToArtist(Artist* artist, const Studio* studio) {
	Event* recording = createRecordingEvent(studio->code, artist->contractExpireDate);
	CHECK_VALIDITY(recording, 0);
	NODE* position = findEventInsertPosition(&artist->schedule, recording);
	EXEC_IF_FAILED(position, freeEvent(recording), 0);
	EXEC_IF_FAILED(L_insert(position, recording), freeEvent(recording), 0);
	artist->eventAmount++;
	return 1;
}
