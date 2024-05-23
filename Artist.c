#include "Artist.h"

int initArtist(Artist* pArtist)
{
	int initRes;
	pArtist->name = getName("Enter artist name:"); //gets artist name from user
	if (!pArtist->name) return 0;
	if (!generateSongList(pArtist)) return failedCreation(pArtist, 1);//gets song list from user
	if (!generateEmployeeList(pArtist)) return failedCreation(pArtist, 2); //gets employee list from user
	if (!generateMerchList(pArtist)) return failedCreation(pArtist, 3); //gets merch list from user
	if (!generateSchedule(pArtist)) return failedCreation(pArtist, 4); //creates empty schedule
	if (!getContractDate(pArtist)) return failedCreation(pArtist, 5); //gets contract end-date from user
	if (!generateConcertList(pArtist)) return failedCreation(pArtist, 6); //gets concert list from user
	int type = getArtistType(); //selects artist type: solo/band and using the corresponding init function 
	if (type == SOLO) initRes = initSolo(pArtist);
	else initRes = initBand(pArtist);
	if (initRes == 0) failedCreation(pArtist, 7);
	pArtist->revenue = calcArtistRevenueForCompany(pArtist);
	return 1;
}

int failedCreation(Artist* pArtist, int step)
{
	free(pArtist->name);
	if (step >= 2)
		L_free(&pArtist->songList, freeSong);
	if (step >= 3)
		freeEmployeeList(pArtist, pArtist->employeeAmount);
	if (step >= 4) {
		for (int i = 0; i < pArtist->merchAmount; i++)
			free(pArtist->merchArr[i]);
		free(pArtist->merchArr);
	}
	if (step >= 5)
		L_free(&pArtist->schedule, freeEvent);
	if (step >= 6)
		free(pArtist->contractExpireDate);
	if (step >= 7)
		freeConcertList(pArtist, pArtist->concertAmount);
	return 0;
}

int generateSongList(Artist* pArtist)
{
	LIST list;
	if (L_init(&list) == False) return 0;
	pArtist->songList = list;
	pArtist->songAmount = getNumberGreaterThan(0, "Enter number of songs:");
	Song* pSong;
	for (int i = 0; i < pArtist->songAmount; i++) 
	{
		pSong = (Song*)malloc(sizeof(Song));
		if (!pSong) {
			L_free(&pArtist->songList, freeSong); //freeing all songs that were created untill failure
			return 0;
		}
		initSong(pSong);
		if (!L_insert(&pArtist->songList.head, pSong)) {
			L_free(&pArtist->songList, freeSong); //freeing all songs that were created untill failure
			return 0;
		}
	}
	return 1;
}

int generateEmployeeList(Artist* pArtist) {
	Employee tempEmployee;
	pArtist->employeeAmount = getNumberGreaterThan(0, "Enter number of artist Employees:");
	pArtist->employeeList = (Employee*)malloc(sizeof(Employee) * pArtist->employeeAmount);
	if (!pArtist->employeeList) return 0;
	for (int i = 0; i < pArtist->employeeAmount; i++) {
		if (!initEmployee(&tempEmployee)) {
			return freeEmployeeList(pArtist, i);
		}
		pArtist->employeeList[i] = tempEmployee;
	}
	return 1;
}

int generateMerchList(Artist* pArtist) {
	Merch* tempMerch;
	pArtist->merchAmount = getNonNegativeNumber("Enter amount of merch items:");
	pArtist->merchArr = (Merch**)malloc((sizeof(Merch*) * pArtist->merchAmount));
	if (!pArtist->merchArr) return 0;
	for (int i = 0; i < pArtist->merchAmount; i++)
	{
		tempMerch = (Merch*)malloc(sizeof(Merch));
		if (!tempMerch) {
			free(pArtist->merchArr); //frees allocated space for all merch items
			return 0;
		}
		initMerch(tempMerch, pArtist->merchArr, i);
		pArtist->merchArr[i] = tempMerch;
	}
	return 1;
}

int generateConcertList(Artist* pArtist)
{
	LIST* employeeList, *songList;
	Concert* pConcert;
	pArtist->concertAmount = getNonNegativeNumber("Enter amount of concerts:");
	pArtist->concertArr = (Concert**)malloc(sizeof(Concert*) * pArtist->concertAmount);
	if (!pArtist->concertArr) return 0;
	for (int i = 0; i < pArtist->concertAmount; i++) {
		pConcert = (Concert*)malloc(sizeof(Concert));
		if (!pConcert) return freeConcertList(pArtist, i);
		employeeList = generateListFromArr(pArtist->employeeList, pArtist->employeeAmount);
		songList = duplicateList(&pArtist->songList);
		if (!initConcert(pConcert, songList, pArtist->songAmount, employeeList, pArtist->employeeAmount,pArtist->merchArr, pArtist->merchAmount, pArtist->contractExpireDate)) {
			free(pConcert);
			return freeConcertList(pArtist, i);
		}
		pArtist->concertArr[i] = pConcert;
		addConcertToEventList(pArtist, pConcert, i);
		L_free(employeeList, freeEmployeePtr); //frees ramaining employees in duplicate employee list
		free(employeeList);
		L_free(songList, freeSong); //frees remaining songs in duplicate song list
		free(songList);
	}
	return 1;
}

int addConcertToEventList(Artist * pArtist, Concert * pConcert, int CurrentNumOfConcerts) {
	Event* concertEvent = createConcertEvent(pConcert->date);
	if (!concertEvent) {
		free(pConcert);
		return freeConcertList(pArtist, CurrentNumOfConcerts);
	}
	NODE * position = findEventInsertPosition(&pArtist->schedule, concertEvent);
	if(!L_insert(position, concertEvent)) return 0;
	pArtist->eventAmount++;
	return 1;
}

int generateSchedule(Artist* pArtist)
{
	LIST tmpList;
	if (!L_init(&tmpList)) return 0;
	pArtist->schedule = tmpList;
	pArtist->eventAmount = 0;
	return 1;
}

int getContractDate(Artist* pArtist)
{
	Date* tmpDate = (Date*)malloc(sizeof(Date));
	if (!tmpDate) return 0;
	printf("Enter contract end date:\n");
	initDate(tmpDate);
	pArtist->contractExpireDate = tmpDate;
	return 1;
}

int getArtistType() {
	int type;
	printf("Press 1 for solo artist\nPress 2 for band\n");
	while (1) {
		scanf("%d", &type);
		if (type != 1 && type != 2) {
			printf("Invalid input, please try again:\n");
			continue;
		}
	return type;
	}
}

int initSolo(Artist* pArtist)
{
	pArtist->printFunc = printSolo;
	pArtist->freeFunc = freeSolo;
	pArtist->numOfMembers = 1;
	Member member;
	if (!initSoloMember(&member)) {
		return 0;
	}
	pArtist->type.soloArtist.member = member;
	return 1;
}

int initBand(Artist* pArtist)
{
	pArtist->printFunc = printBand;
	pArtist->freeFunc = freeBand;
	Member* member;
	pArtist->numOfMembers = getNumberGreaterThan(1, "Enter amount of band members:");
	pArtist->type.band.bandMembers = (Member*)malloc(sizeof(Member) * pArtist->numOfMembers);
	if (!pArtist->type.band.bandMembers) return 0;
	for (int i = 0; i < pArtist->numOfMembers; i++) {
		member = (Member*)malloc(sizeof(Member));
		if (!member){
			return freeBandMembers(pArtist, i); //if allocation failed, frees all the members that were alocated so far
		}
		if (!initMember(member)) {
			freeMember(member);//frees current member that was allocated but not inserted yet into bandMembers array
			return freeBandMembers(pArtist, i);
		}
		pArtist->type.band.bandMembers[i] = *member;
		free(member);
	}
	return 1;
}

void printGeneralInfo(const void * v)
{
	const Artist* pArtist = *(const Artist**)v;
	printf("%s: %d songs, %d employees, %d merch items, %d events,", pArtist->name, pArtist->songAmount, pArtist->employeeAmount, pArtist->merchAmount, pArtist->eventAmount);
	printf(" Revenue: %.2lf, contract ends at: ", pArtist->revenue);
	printDate(pArtist->contractExpireDate);
	printf("\n\n");
}

void printArtist(const Artist* pArtist)
{
	printf("%s\n", pArtist->name);
	pArtist->printFunc(pArtist);
	printf("\nSongs:");
	printLinkedListInRows(&pArtist->songList, (void *)printSong); //prints songList
	printf("Employees:\n");
	printArtistEmployees(pArtist);
	if (pArtist->merchAmount > 0) {
		printf("\nMerch:\n");
		printArtistMerch(pArtist);
	}
	if (pArtist->concertAmount > 0) {
		printf("\nConcerts:\n");
		printArtistConcerts(pArtist);
	}
	if (pArtist->eventAmount > 0) {
		printf("\nEvents:\n");
		printLinkedListInRows(&pArtist->schedule, (void *)printEvent); //prints schedule
	}
	printf("Revenue for company - %.2lf\n", pArtist->revenue);
	printf("Contract ends at: ");
	printDate(pArtist->contractExpireDate);
	printf("\n\n");
}

void printSolo(const Artist* pArtist)
{
	printMember(&pArtist->type.soloArtist.member);
}

void printBand(const Artist* pArtist)
{
	generalArrayFunction(pArtist->type.band.bandMembers, pArtist->numOfMembers, sizeof(Member), printMember);
}

void printArtistEmployees(const Artist* pArtist)
{
	generalArrayFunction(pArtist->employeeList, pArtist->employeeAmount, sizeof(Employee), printEmployeeWithEnter);
}

void printArtistMerch(const Artist* pArtist)
{
	generalArrayFunction(pArtist->merchArr, pArtist->merchAmount, sizeof(Merch *), printMerchWithEnter);
}

void printArtistConcerts(const Artist* pArtist)
{
	generalArrayFunction(pArtist->concertArr, pArtist->concertAmount, sizeof(Concert*), printConcert);
}

void freeArtist(Artist* pArtist)
{
	free(pArtist->name);
	pArtist->freeFunc(pArtist); //frees members
	L_free(&pArtist->songList, freeSong); // frees songList
	freeEmployeeList(pArtist, pArtist->employeeAmount);
	for (int i = 0; i < pArtist->merchAmount; i++)
		free(pArtist->merchArr[i]);
	free(pArtist->merchArr);
	freeConcertList(pArtist, pArtist->concertAmount);
	L_free(&pArtist->schedule, freeEvent); // frees schedule
	free(pArtist->contractExpireDate);
}

void freeSolo(Artist* pArtist)
{
	freeMember(&pArtist->type.soloArtist.member);
}

void freeBand(Artist* pArtist)
{
	freeBandMembers(pArtist, pArtist->numOfMembers);
}

int freeBandMembers(Artist* pArtist, int numOfMembers)
{
	for (int i = 0; i < numOfMembers; i++) {
		freeMember(&pArtist->type.band.bandMembers[i]);//frees each member
	}
	free(pArtist->type.band.bandMembers);//frees allocated space for all members
	return 0;
}

int freeEmployeeList(Artist* pArtist, int numOfEmployees) {
	for (int i = 0; i < numOfEmployees; i++) {
		freeEmployee(&pArtist->employeeList[i]); //frees each employee
	}
	free(pArtist->employeeList);//frees allocated space for all employees
	return 0;
}

int freeConcertList(Artist* pArtist, int numOfConcerts)
{
	for (int i = 0; i < numOfConcerts; i++) {
		freeConcert(pArtist->concertArr[i]); //frees each concert
	}
	free(pArtist->concertArr); //frees allocated space for all concerts
	return 0;
}

LIST* generateListFromArr(const Employee* arr, int amount) {
	Employee* newEmployee;
	LIST* pList = (LIST*)malloc(sizeof(LIST));
	if (!pList) return NULL;
	if (L_init(pList) == False) {
		free(pList);
		return NULL;
	}
	for (int i = 0; i < amount; i++) {
		newEmployee = copyEmployee(&arr[i]);
		if(!L_insert(&pList->head, newEmployee)) return NULL;
	}
	return pList;
}

LIST* duplicateList(const LIST* source)
{
	Song* newSong;
	if (!source || !source->head.next) return NULL;
	LIST * dupList = (LIST*)malloc(sizeof(LIST));
	if (!dupList) return NULL;
	if (L_init(dupList) == False) {
		free(dupList);
		return NULL;
	}
	NODE* tmpSRC = source->head.next, *tmpDUP = &dupList->head;
	while (tmpSRC != NULL)
	{
		newSong = copySong((const Song*)tmpSRC->key);
		EXEC_IF_FAILED(newSong, L_free(dupList, freeSong), NULL);
		EXEC_IF_FAILED(L_insert(tmpDUP, newSong), L_free(dupList, freeSong),NULL);
		tmpDUP = tmpDUP->next;
		tmpSRC = tmpSRC->next;
	}
	return dupList;
}

int addNewSong(Artist* pArtist)
{
	Song* newSong = (Song*)malloc(sizeof(Song));
	if (!newSong) return 0;
	if (!initSong(newSong)) {
		free(newSong);
		return 0;
	}
	if (!L_insert(&pArtist->songList.head, newSong)) {
		free(newSong);
		return 0;
	}
	pArtist->songAmount++;
	pArtist->revenue += calculateSongIncomeForCompany(newSong); //adds song's revenue to artist's revenue for company
	return 1;
}

int addNewEmployee(Artist* pArtist)
{
	Employee* newEmployee = (Employee*)malloc(sizeof(Employee));
	if (!newEmployee) return 0;
	if (!initEmployee(newEmployee)) {
		free(newEmployee);
		return 0;
	}
	Employee* tmpListPtr = (Employee*)realloc(pArtist->employeeList, (pArtist->employeeAmount + 1) * sizeof(Employee));
	if (!tmpListPtr) {
		freeEmployee(newEmployee);
		free(newEmployee);
		return 0;
	}
	pArtist->employeeList = tmpListPtr;
	pArtist->employeeList[pArtist->employeeAmount] = *newEmployee;
	pArtist->employeeAmount++;
	free(newEmployee);
	return 1;
}

int addNewConcert(Artist* pArtist)
{
	Concert* newConcert = (Concert*)malloc(sizeof(Concert));
	CHECK_VALIDITY(newConcert, 0);
	LIST* employeeList = generateListFromArr(pArtist->employeeList, pArtist->employeeAmount);
	LIST* songList = duplicateList(&pArtist->songList);
	if (!initConcert(newConcert, songList, pArtist->songAmount, employeeList, pArtist->employeeAmount, pArtist->merchArr, pArtist->merchAmount, pArtist->contractExpireDate)) {
		free(newConcert);
		return 0;
	}
	Concert** tmpListptr = (Concert**)realloc(pArtist->concertArr, (pArtist->concertAmount + 1) * sizeof(Concert*));
	if (!tmpListptr) {
		freeConcert(newConcert);
		free(newConcert);
		return 0;
	}
	L_free(employeeList, freeEmployeePtr);
	free(employeeList);
	L_free(songList, freeSong);
	free(songList);
	pArtist->concertArr = tmpListptr;
	pArtist->concertArr[pArtist->concertAmount] = newConcert;
	pArtist->concertAmount++;
	Event* newEvent = createConcertEvent(newConcert->date); //adding new concert to schedule. Position will be based on date
	NODE* position = findEventInsertPosition(&pArtist->schedule, newEvent);
	if(!L_insert(position, newEvent)) return 0;
	pArtist->eventAmount++;
	pArtist->revenue += calcConcertRevenue(newConcert); //adding to artist's revenue the new concert revenue.
	return 1;
}

int addNewMerch(Artist* pArtist)
{
	Merch* newMerch = (Merch*)malloc(sizeof(Merch));
	if (!newMerch) return 0;
	initMerch(newMerch, pArtist->merchArr, pArtist->merchAmount);
	Merch** tmpListPtr = (Merch**)realloc(pArtist->merchArr, (pArtist->merchAmount + 1) * sizeof(Merch*));
	if (!tmpListPtr) {
		free(newMerch);
		return 0;
	}
	pArtist->merchArr = tmpListPtr;
	pArtist->merchArr[pArtist->merchAmount] = newMerch;
	pArtist->merchAmount++;
	return 1;
}

int addNewEvent(Artist* pArtist)
{
	Event* newEvent = (Event*)malloc(sizeof(Event));
	if (!newEvent) return 0;
	if (!initEvent(newEvent, pArtist->contractExpireDate)) {
		free(newEvent);
		return 0;
	}
	NODE* insertPos = findEventInsertPosition(&pArtist->schedule, newEvent);
	if (insertPos == NULL) {
		free(newEvent);
		return 0;
	}
	if(!L_insert(insertPos, newEvent)) return 0;
	pArtist->eventAmount++;
	return 1;
}

NODE* findEventInsertPosition(LIST* schedule, Event* newEvent)
{
	CHECK_VALIDITY(schedule, NULL);
	CHECK_VALIDITY(newEvent, NULL);
	Event* currEvent;
	NODE* afterPosition = schedule->head.next, *position = &schedule->head;
	while (afterPosition != NULL)
	{
		currEvent = (Event*)(afterPosition->key);
		if (compareDates(newEvent->date, currEvent->date) < 0)
			return position;
		position = afterPosition;
		afterPosition = afterPosition->next;
	}
	return position;
}

double calcArtistRevenueForCompany(const Artist * pArtist)
{
	double revenue = 0;
	revenue += calcSongsRevenue(pArtist);
	revenue += calcConcertsRevenue(pArtist);
	return revenue;
}

double calcSongsRevenue(const Artist* pArtist)
{
	double revenue = 0;
	NODE* songPtr = pArtist->songList.head.next;
	while (songPtr != NULL) {
		revenue += calculateSongIncomeForCompany((const Song*)songPtr->key);
		songPtr = songPtr->next;
	}
	return revenue;
}

int calcConcertsRevenue(const Artist* pArtist) {
	int revenue = 0;
	for (int i = 0; i < pArtist->concertAmount; i++)
		revenue += calcConcertRevenue(pArtist->concertArr[i]);
	return revenue;
}

int writeArtistToTextFile(const Artist* pArtist, FILE* f)
{
	if (!f) return 0;
	fprintf(f, "%s\n", pArtist->name);
	fprintf(f, "%d\n", pArtist->numOfMembers);
	if (pArtist->numOfMembers == 1)
		writeMemberToTextFile(&pArtist->type.soloArtist.member, f); //writing solo artist
	else
		for (int i = 0; i < pArtist->numOfMembers; i++)
			writeMemberToTextFile(&pArtist->type.band.bandMembers[i], f); //wrtiting all band members
	if (!writeSongListToTextFile(pArtist, f)) return 0;
	if (!writeEmployeeArrToTextFile(pArtist, f)) return 0;
	if (!writeMerchArrToTextFile(pArtist, f)) return 0;
	if (!writeConcertArrToTextFile(pArtist, f)) return 0;
	if (!writeScheduleToTextFile(pArtist, f)) return 0;
	if (!writeDateToTextFile(pArtist->contractExpireDate, f)) return 0;
	fprintf(f, "%lf\n", pArtist->revenue);
	return 1;
}

int writeSongListToTextFile(const Artist* pArtist, FILE* f)
{
	if (!f) return 0;
	fprintf(f, "%d\n", pArtist->songAmount);
	NODE* nodePtr = pArtist->songList.head.next;
	while (nodePtr != NULL) {
		writeSongToTextFile((const Song *)(nodePtr->key), f);
		nodePtr = nodePtr->next;
	}
	return 1;
}

int writeScheduleToTextFile(const Artist* pArtist, FILE* f)
{
	if (!f) return 0;
	fprintf(f, "%d\n", pArtist->eventAmount);
	NODE* nodePtr = pArtist->schedule.head.next;
	while (nodePtr != NULL) {
		writeEventToTextFile((const Event*)(nodePtr->key), f);
		nodePtr = nodePtr->next;
	}
	return 1;
}

int writeEmployeeArrToTextFile(const Artist* pArtist, FILE* f)
{
	if (!f) return 0;
	fprintf(f, "%d\n", pArtist->employeeAmount);
	for (int i = 0; i < pArtist->employeeAmount; i++)
		writeEmployeeToTextFile(&pArtist->employeeList[i], f);
	return 1;
}

int writeMerchArrToTextFile(const Artist* pArtist, FILE* f)
{
	if (!f) return 0;
	fprintf(f, "%d\n", pArtist->merchAmount);
	for (int i = 0; i < pArtist->merchAmount; i++)
		writeMerchToTextFile(pArtist->merchArr[i], f);
	return 1;
}

int writeConcertArrToTextFile(const Artist* pArtist, FILE* f)
{
	if (!f) return 0;
	fprintf(f, "%d\n", pArtist->concertAmount);
	for (int i = 0; i < pArtist->concertAmount; i++)
		writeConcertToTextFile(pArtist->concertArr[i], f);
	return 1;
}

Artist* readArtistFromTextFile(FILE* f)
{
	CHECK_VALIDITY(f, NULL);
	Artist* newArtist = (Artist*)malloc(sizeof(Artist));
	CHECK_VALIDITY(newArtist, NULL);
	CHECK_VALIDITY(readArtistNameFromTextFile(f, newArtist), failedToReadArtistFromFile(newArtist, 0)); //reads name
	CHECK_VALIDITY(readArtistMembersFromTextFile(f, newArtist), failedToReadArtistFromFile(newArtist, 1)); //reads members
	CHECK_VALIDITY(readSongListFromTextFile(f, newArtist), failedToReadArtistFromFile(newArtist, 2)); //reads songList
	CHECK_VALIDITY(readEmployeeArrFromTextFile(f, newArtist), failedToReadArtistFromFile(newArtist, 3)); //reads EmployeeArr
	CHECK_VALIDITY(readMerchArrFromTextFile(f, newArtist), failedToReadArtistFromFile(newArtist, 4)); //reads MerchArr
	CHECK_VALIDITY(readConcertArrFromTextFile(f, newArtist), failedToReadArtistFromFile(newArtist, 5)); //reads concertArr
	CHECK_VALIDITY(readScheduleFromTextFile(f, newArtist), failedToReadArtistFromFile(newArtist, 6)); //reads schedule
	Date* newDate = readDateFromTextFile(f);
	CHECK_VALIDITY(newDate, failedToReadArtistFromFile(newArtist, 7)); //reads contract end-date
	newArtist->contractExpireDate = newDate;
	IS_READ_CORRECTLY(fscanf(f, "%lf\n", &newArtist->revenue), 1, failedToReadArtistFromFile(newArtist, 8));
	return newArtist;
}

Artist* failedToReadArtistFromFile(Artist* pArtist, int step)
{
	if (step >= 1) //frees Name
		free(pArtist->name);
	if (step >= 2) { //frees Members
		if (pArtist->numOfMembers == 1) //solo artist
			freeMember(&pArtist->type.soloArtist.member);
		else // band
				freeBandMembers(pArtist, pArtist->numOfMembers);
	}
	if (step >= 3) //frees songList
		L_free(&pArtist->songList, freeSong);
	if (step >= 4) //frees employees
		freeEmployeeList(pArtist, pArtist->employeeAmount);
	if (step >= 5) { //frees merch
		for (int i = 0; i < pArtist->merchAmount; i++)
			free(pArtist->merchArr[i]);
		free(pArtist->merchArr);
	}
	if (step >= 6) //frees concerts
		freeConcertList(pArtist, pArtist->concertAmount);
	if (step >= 7) //frees schedule
		L_free(&pArtist->schedule, freeEvent);
	if (step >= 8)
		free(pArtist->contractExpireDate);
	free(pArtist);
	return NULL;
}

int readArtistNameFromTextFile(FILE* f, Artist * newArtist)
{
	if (!f) return 0;
	char buffer[MAX_STR_LEN];
	myGets(buffer, sizeof(buffer), f);
	newArtist->name =_strdup(buffer);
	return 1;
}

int readArtistMembersFromTextFile(FILE* f, Artist* newArtist)
{
	if (!f) return 0;
	if (fscanf(f, "%d\n", &newArtist->numOfMembers) != 1) return 0;
	Member* newMember;
	if (newArtist->numOfMembers == 1) {
		newArtist->printFunc = printSolo;
		newArtist->freeFunc = freeSolo;
		newMember = readMemberFromTextFile(f);
		if (!newMember) return 0;
		newArtist->type.soloArtist.member = *newMember;
		free(newMember);
	}
	else
	{
		newArtist->printFunc = printBand;
		newArtist->freeFunc = freeBand;
		newArtist->type.band.bandMembers = (Member*)malloc(sizeof(Member) * newArtist->numOfMembers);
		if (!newArtist->type.band.bandMembers) return 0;
		for (int i = 0; i < newArtist->numOfMembers; i++) {
			newMember = readMemberFromTextFile(f);
			if (!newMember) {
				freeBandMembers(newArtist, i); //frees all members that were allocated so far
				return 0;
			}
			newArtist->type.band.bandMembers[i] = *newMember;
			free(newMember);
		}
	}
	return 1;
}

int readSongListFromTextFile(FILE* f, Artist* newArtist)
{
	if (!f) return 0;
	Song* newSong;
	if (fscanf(f, "%d\n", &newArtist->songAmount) != 1) return 0;
	LIST * tmpList = (LIST*)malloc(sizeof(LIST));
	if (!tmpList) return 0;
	newArtist->songList = *tmpList;
	if (L_init(&newArtist->songList) == False) return 0;
	NODE* nodePtr = &newArtist->songList.head;
	for (int i = 0; i < newArtist->songAmount; i++) {
		newSong = readSongFromTextFile(f);
		if (!newSong) {
			L_free(&newArtist->songList, freeSong); //frees all the songs that were allocated till failure
			return 0;
		}
		nodePtr = L_insert(nodePtr, newSong);
	}
	free(tmpList);
	return 1;
}

int readEmployeeArrFromTextFile(FILE* f, Artist* newArtist)
{
	Employee* newEmployee;
	if (!f) return 0;
	if (fscanf(f, "%d\n", &newArtist->employeeAmount) != 1) return 0;
	newArtist->employeeList = (Employee*)malloc(sizeof(Employee) * newArtist->employeeAmount);
	if (!newArtist->employeeList) return 0;
	for (int i = 0; i < newArtist->employeeAmount; i++) {
		newEmployee = readEmployeeFromTextFile(f);
		if (!newEmployee) {
			freeEmployeeList(newArtist, i); //frees all the employees that were created till failure
			return 0;
		}
		newArtist->employeeList[i] = *newEmployee;
		free(newEmployee);
	}
	return 1;
}

int readMerchArrFromTextFile(FILE* f, Artist* newArtist)
{
	Merch* newMerch;
	if (!f) return 0;
	if (fscanf(f, "%d\n", &newArtist->merchAmount) != 1) return 0;
	newArtist->merchArr = (Merch**)malloc(sizeof(Merch*) * newArtist->merchAmount);
	if (!newArtist->merchArr) return 0;
	for (int i = 0; i < newArtist->merchAmount; i++) {
		newMerch = readMerchFromTextFile(f);
		if (!newMerch) {
			free(newArtist->merchArr); //frees allocated memory for merch array
			return 0;
		}
		newArtist->merchArr[i] = newMerch;
	}
	return 1;

}

int readConcertArrFromTextFile(FILE* f, Artist* newArtist)
{
	Concert* newConcert;
	if (!f) return 0;
	if (fscanf(f, "%d\n", &newArtist->concertAmount) != 1) return 0;
	newArtist->concertArr = (Concert**)malloc(sizeof(Concert*) * newArtist->concertAmount);
	if (!newArtist->concertArr) return 0;
	for (int i = 0; i < newArtist->concertAmount; i++) {
		newConcert = readConcertFromTextFile(f);
		if (!newConcert) {
			freeConcertList(newArtist, i); //frees all the concerts that were created till failure
			return 0;
		}
		newArtist->concertArr[i] = newConcert;
	}
	return 1;
}

int readScheduleFromTextFile(FILE* f, Artist* newArtist)
{
	if (!f) return 0;
	Event* newEvent;
	if (fscanf(f, "%d\n", &newArtist->eventAmount) != 1) return 0;
	LIST* tmpList = (LIST*)malloc(sizeof(LIST));
	if (!tmpList) return 0;
	newArtist->schedule = *tmpList;
	if (L_init(&newArtist->schedule) == False) return 0;
	NODE* nodePtr = &newArtist->schedule.head;
	for (int i = 0; i < newArtist->eventAmount; i++) {
		newEvent = readEventFromTextFile(f);
		if (!newEvent) {
			L_free(&newArtist->schedule, freeEvent); //frees all the events that were allocated till failure
			return 0;
		}
		nodePtr = L_insert(nodePtr, newEvent);
	}
	free(tmpList);
	return 1;

}

int compareArtistsByName(const void* v1, const void* v2)
{
	//returns 1 if name2 > name1
	//returns -1 if name1 > name2
	//returns 0 if name1 = name2
	const Artist* pArtist1 = *(const Artist**) v1;
	const Artist* pArtist2 = *(const Artist**) v2;
	return strcmp(pArtist1->name, pArtist2->name) ;
}

int compareArtistsByRevenue(const void* v1, const void* v2)
{
	//returns 1 if rev2 > rev1
	//returns -1 if rev1 > rev2
	//returns 0 if rev1 = rev2
	const Artist* pArtist1 = *(const Artist**)v1;
	const Artist* pArtist2 = *(const Artist**)v2;
	if (pArtist1->revenue < pArtist2->revenue) return 1;
	if (pArtist1->revenue > pArtist2->revenue) return -1;
	return 0; //same revenue

}

int compareArtistsByContractEndDate(const void* v1, const void* v2)
{
	const Artist* pArtist1 = *(const Artist**)v1;
	const Artist* pArtist2 = *(const Artist**)v2;
	return compareDates(pArtist1->contractExpireDate, pArtist2->contractExpireDate);
}

void renewContract(Artist* pArtist)
{
	Date newDate;
	printf("Contract end date: ");
	printDate(pArtist->contractExpireDate);
	printf("\n");
	while (1) {
		printf("Enter new contract end date:\n");
		initDate(&newDate);
		if (compareDates(&newDate, pArtist->contractExpireDate) <= 0) {
			printf("Please enter a date after ");
			printDate(pArtist->contractExpireDate);
			printf("\n");
			continue;
		}
		break;
	}
	//copying values
	pArtist->contractExpireDate->day = newDate.day;
	pArtist->contractExpireDate->month = newDate.month;
	pArtist->contractExpireDate->year = newDate.year;
}

int saveArtistToBinFile(const Artist* pArtist, FILE* f)
{
	CHECK_VALIDITY(f, 0);
	CHECK_VALIDITY(pArtist, 0);
	CHECK_VALIDITY(saveStringToBinFile(pArtist->name, f), 0); //writes name
	CHECK_VALIDITY(saveMembersToBinFile(pArtist, f), 0); //writes numOfMembers + members
	CHECK_VALIDITY(saveSongListToBinFile(pArtist, f), 0); //writes songAmount + songList
	CHECK_VALIDITY(saveEmployeeListToBinFile(pArtist, f), 0); //write empAmount + empList
	CHECK_VALIDITY(saveMerchArrToBinFile(pArtist, f), 0);//write merchAmount + merchArr
	CHECK_VALIDITY(saveConcertArrToBinFile(pArtist, f), 0);//write concertAmount + concertArr
	CHECK_VALIDITY(saveScheduleToBinFile(pArtist, f), 0);//write eventAmount + schedule
	
	CHECK_VALIDITY(fwrite(pArtist->contractExpireDate, sizeof(Date), 1, f) == 1, 0);//write contract end date
	CHECK_VALIDITY(fwrite(&pArtist->revenue, sizeof(double), 1, f) == 1, 0); //writes revenue
	return 1;
}

int saveMembersToBinFile(const Artist* pArtist, FILE* f)
{
	CHECK_VALIDITY(f, 0);
	CHECK_VALIDITY(pArtist, 0);
	CHECK_VALIDITY(fwrite(&pArtist->numOfMembers, sizeof(int), 1, f) == 1, 0); //writes numOfMembers
	if (pArtist->numOfMembers == 1) {
		CHECK_VALIDITY(saveMemberToBinFile(&pArtist->type.soloArtist.member, f), 0);
	}
	else {
		for (int i = 0; i < pArtist->numOfMembers; i++)
			CHECK_VALIDITY(saveMemberToBinFile(&pArtist->type.band.bandMembers[i], f),0);
	}
	return 1;
}

int saveSongListToBinFile(const Artist* pArtist, FILE* f)
{
	CHECK_VALIDITY(f, 0);
	CHECK_VALIDITY(pArtist, 0);
	CHECK_VALIDITY(&pArtist->songList, 0); //checks if songlist is null
	CHECK_VALIDITY(fwrite(&pArtist->songAmount, sizeof(int), 1, f) == 1, 0); //writes songAmount
	NODE* ptr = pArtist->songList.head.next;
	while (ptr != NULL) { //writes songs
		CHECK_VALIDITY(saveSongToBinFile((const Song*)(ptr->key), f), 0);
		ptr = ptr->next;
	}
	return 1;
}

int saveEmployeeListToBinFile(const Artist* pArtist, FILE* f)
{
	CHECK_VALIDITY(f, 0);
	CHECK_VALIDITY(pArtist, 0);
	CHECK_VALIDITY(fwrite(&pArtist->employeeAmount, sizeof(int), 1, f) == 1, 0); //writes EmployeeAmount
	for (int i = 0; i < pArtist->employeeAmount; i++) //writes employeeList
		CHECK_VALIDITY(saveEmployeeToBinFile(&pArtist->employeeList[i], f), 0);
	return 1;
}

int saveMerchArrToBinFile(const Artist* pArtist, FILE* f)
{
	CHECK_VALIDITY(f, 0);
	CHECK_VALIDITY(pArtist, 0);
	CHECK_VALIDITY(fwrite(&pArtist->merchAmount, sizeof(int), 1, f) == 1, 0); //writes merchAmount
	for (int i = 0; i < pArtist->merchAmount; i++) //writes merchArr
		CHECK_VALIDITY(saveMerchToBinFile(pArtist->merchArr[i], f), 0);
	return 1;
}

int saveConcertArrToBinFile(const Artist* pArtist, FILE* f)
{
	CHECK_VALIDITY(f, 0);
	CHECK_VALIDITY(pArtist, 0);
	CHECK_VALIDITY(fwrite(&pArtist->concertAmount, sizeof(int), 1, f) == 1, 0); //writes concertAmount
	for (int i = 0; i < pArtist->concertAmount; i++) //writes concertArr
		CHECK_VALIDITY(saveConcertToBinFile(pArtist->concertArr[i], f), 0);
	return 1;
}

int saveScheduleToBinFile(const Artist* pArtist, FILE* f)
{
	CHECK_VALIDITY(f, 0);
	CHECK_VALIDITY(pArtist, 0);
	CHECK_VALIDITY(&pArtist->schedule, 0); //checks if schedule is null
	CHECK_VALIDITY(fwrite(&pArtist->eventAmount, sizeof(int), 1, f) == 1, 0); //writes eventAmount
	NODE* ptr = pArtist->schedule.head.next;
	while (ptr != NULL) { //writes schedule
		CHECK_VALIDITY(saveEventToBinFile((const Event*)(ptr->key), f), 0);
		ptr = ptr->next;
	}
	return 1;
}

Artist* loadArtistFromBinFile(FILE* f)
{
	CHECK_VALIDITY(f, NULL);
	Artist* newArtist = (Artist*)malloc(sizeof(Artist));
	CHECK_VALIDITY(newArtist, NULL);
	Date* date = (Date*)malloc(sizeof(Date));
	CHECK_VALIDITY(date, NULL);
	newArtist->contractExpireDate = date;
	newArtist->name = loadStringFromBinFile(f); //reading name
	FREE_IF_FAILED(newArtist->name, newArtist, NULL);
	CHECK_VALIDITY(loadMembersFromBinFile(newArtist, f), failedToReadArtistFromFile(newArtist, 1)); //reading members
	CHECK_VALIDITY(loadSongListFromBinFile(newArtist, f), failedToReadArtistFromFile(newArtist, 2)); //reading songs
	CHECK_VALIDITY(loadEmployeeListFromBinFile(newArtist, f), failedToReadArtistFromFile(newArtist, 3)); //reading employees
	CHECK_VALIDITY(loadMerchArrFromBinFile(newArtist, f), failedToReadArtistFromFile(newArtist, 4)); //reading merchArr
	CHECK_VALIDITY(loadConcertArrFromBinFile(newArtist, f), failedToReadArtistFromFile(newArtist, 5)); //reading concertArr
	CHECK_VALIDITY(loadScheduleFromBinFile(newArtist, f), failedToReadArtistFromFile(newArtist, 6)); //reading schedule
	IS_READ_CORRECTLY(fread(newArtist->contractExpireDate, sizeof(Date), 1, f), 1, failedToReadArtistFromFile(newArtist, 7)); //reading contract end date
	IS_READ_CORRECTLY(fread(&newArtist->revenue, sizeof(double), 1, f), 1, failedToReadArtistFromFile(newArtist, 8)); //reading revenue
	return newArtist;
}

int loadMembersFromBinFile(Artist* newArtist, FILE* f)
{
	Member * member;
	CHECK_VALIDITY(f, 0);
	CHECK_VALIDITY(newArtist, 0);
	IS_READ_CORRECTLY(fread(&newArtist->numOfMembers, sizeof(int), 1, f), 1, 0); //reading numOfMembers
	if (newArtist->numOfMembers == 1) { //solo artist
		newArtist->printFunc = printSolo;
		newArtist->freeFunc = freeSolo;
		member = loadMemberFromBinFile(f);
		CHECK_VALIDITY(member, 0);
		newArtist->type.soloArtist.member = *member;
		free(member);
	}
	else { //band
		newArtist->printFunc = printBand;
		newArtist->freeFunc = freeBand;
		newArtist->type.band.bandMembers = (Member*)malloc(sizeof(Member) * newArtist->numOfMembers);
		CHECK_VALIDITY(newArtist->type.band.bandMembers, 0);
		for (int i = 0; i < newArtist->numOfMembers; i++) {
			member = loadMemberFromBinFile(f);
			EXEC_IF_FAILED(member, freeBandMembers(newArtist, i), 0);//frees all members allocated till failure
			newArtist->type.band.bandMembers[i] = *member;
			free(member);
		}
	}
	return 1;
}

int loadSongListFromBinFile(Artist* newArtist, FILE* f)
{
	CHECK_VALIDITY(f, 0);
	CHECK_VALIDITY(newArtist, 0);
	Song* newSong;
	LIST* list = (LIST*)malloc(sizeof(LIST));
	CHECK_VALIDITY(list, 0);
	if(L_init(list) == False) return 0;
	newArtist->songList = *list;
	IS_READ_CORRECTLY(fread(&newArtist->songAmount, sizeof(int), 1, f), 1, 0); //reading songAmount
	NODE* ptr = &newArtist->songList.head;
	for (int i = 0; i < newArtist->songAmount; i++) {
		newSong = loadSongFromBinFile(f);
		EXEC_IF_FAILED(newSong, L_free(&newArtist->songList, freeSong), 0); //frees all songs allocated till failure
		ptr = L_insert(ptr, newSong);
	}
	free(list);
	return 1;
}

int loadEmployeeListFromBinFile(Artist* newArtist, FILE* f)
{
	CHECK_VALIDITY(f, 0);
	CHECK_VALIDITY(newArtist, 0);
	Employee* newEmployee;
	IS_READ_CORRECTLY(fread(&newArtist->employeeAmount, sizeof(int), 1, f), 1, 0); //reading employeeAmount
	newArtist->employeeList = (Employee*)malloc(sizeof(Employee) * newArtist->employeeAmount);
	CHECK_VALIDITY(newArtist->employeeList, 0);
	for (int i = 0; i < newArtist->employeeAmount; i++) {
		newEmployee = loadEmployeeFromBinFile(f);
		EXEC_IF_FAILED(newEmployee, freeEmployeeList(newArtist, i), 0); // frees all employees allocated till failure
		newArtist->employeeList[i] = *newEmployee;
		free(newEmployee);
	}
	return 1;
}

int loadMerchArrFromBinFile(Artist* newArtist, FILE* f)
{
	CHECK_VALIDITY(f, 0);
	CHECK_VALIDITY(newArtist, 0);
	Merch * newMerch;
	IS_READ_CORRECTLY(fread(&newArtist->merchAmount, sizeof(int), 1, f), 1, 0); //reading merchAmount
	newArtist->merchArr = (Merch**)malloc(sizeof(Merch*) * newArtist->merchAmount);
	CHECK_VALIDITY(newArtist->merchArr, 0);
	for (int i = 0; i < newArtist->merchAmount; i++) {
		newMerch = loadMerchFromBinFile(f);
		EXEC_IF_FAILED(newMerch, free(newArtist->merchArr), 0);
		newArtist->merchArr[i] = newMerch;
	}
	return 1;
}

int loadConcertArrFromBinFile(Artist* newArtist, FILE* f)
{
	CHECK_VALIDITY(f, 0);
	CHECK_VALIDITY(newArtist, 0);
	Concert* newConcert;
	IS_READ_CORRECTLY(fread(&newArtist->concertAmount, sizeof(int), 1, f), 1, 0); //reading concertAmount
	newArtist->concertArr = (Concert**)malloc(sizeof(Concert*) * newArtist->concertAmount);
	CHECK_VALIDITY(newArtist->concertArr, 0);
	for (int i = 0; i < newArtist->concertAmount; i++) {
		newConcert = loadConcertFromBinFile(f);
		EXEC_IF_FAILED(newConcert, freeConcertList(newArtist, i), 0);
		newArtist->concertArr[i] = newConcert;
	}
	return 1;
}

int loadScheduleFromBinFile(Artist* newArtist, FILE* f)
{
	CHECK_VALIDITY(f, 0);
	CHECK_VALIDITY(newArtist, 0);
	Event* newEvent;
	LIST* list = (LIST*)malloc(sizeof(LIST));
	CHECK_VALIDITY(list, 0);
	if (L_init(list) == False) return 0;
	newArtist->schedule = *list;
	IS_READ_CORRECTLY(fread(&newArtist->eventAmount, sizeof(int), 1, f), 1, 0); //reading eventAmount
	NODE* ptr = &newArtist->schedule.head;
	for (int i = 0; i < newArtist->eventAmount; i++) {
		newEvent = loadEventFromBinFile(f);
		EXEC_IF_FAILED(newEvent, L_free(&newArtist->schedule, freeEvent), 0); //frees all songs allocated till failure
		ptr = L_insert(ptr, newEvent);
	}
	free(list);
	return 1;
}

void doActionOnArtist(Artist* pArtist) {
	int option;
	do {
		printArtistMenu();
		scanf("%d", &option);
		switch (option)
		{
		case ePrintArtist:
			printArtist(pArtist);
			break;
		case eAddSong:
			if(!addNewSong(pArtist)) printf("adding song failed\n");
			break;
		case eAddEmp:
			if(!addNewEmployee(pArtist)) printf("adding employee failed\n");
			break;
		case eAddMerch:
			if(!addNewMerch(pArtist)) printf("adding merch failed\n");
			break;
		case eAddConcert:
			if(!addNewConcert(pArtist)) printf("adding concert failed\n");
			break;
		case eAddEvent:
			if(!addNewEvent(pArtist)) printf("adding event failed\n");
			break;
		case eRenew:
			renewContract(pArtist);
			break;
		case eReturnToMenu:
			break;
		default:
			printf("Invalid input, please try again:\n");
			break;
		}
	} while (option != eReturnToMenu);
}

void printArtistMenu() {
	printf("Choose one of the following:\n");
	printf("1. Print artist\n");
	printf("2. Add a new song\n");
	printf("3. Add a new employee\n");
	printf("4. Add a new merch item\n");
	printf("5. Add a new concert\n");
	printf("6. Add a new event\n");
	printf("7. Renew artist contract\n");
	printf("0. Return to previous menu\n");
}

int calcOverallPlaycount(const Artist* pArtist) {
	CHECK_VALIDITY(pArtist, 0);
	int playCount = 0;
	const Song* song;
	NODE* ptr = pArtist->songList.head.next;
	while (ptr != NULL) {
		song = (const Song*)ptr->key;
		playCount += song->playCount;
		ptr = ptr->next;
	}
	return playCount;
}

int calcOverallTicketsSold(const Artist* pArtist)
{
	CHECK_VALIDITY(pArtist, 0);
	int ticketCount = 0;
	for (int i = 0; i < pArtist->concertAmount; i++)
		ticketCount += pArtist->concertArr[i]->amountOfSoldTickets;
	return ticketCount;
}