#include "General.h"

char* myGets(char* buffer, int size, FILE* f)
{
	if (!f)
		return NULL;
	char* ok;
	if (buffer != NULL && size > 0)
	{
		do { //skip only '\n' strings
			ok = fgets(buffer, size, f);
		} while (ok && ((strlen(buffer) <= 1) && (isspace(buffer[0]))));
		if (ok)
		{
			char* back = buffer + strlen(buffer);
			//trim end spaces
			while ((buffer < back) && (isspace(*--back)));
			*(back + 1) = '\0';
			return buffer;
		}
		buffer[0] = '\0';
	}
	return NULL;
}

char* getName(const char * msg)
{
	printf("%s\n", msg);
	char buffer[MAX_STR_LEN];
	myGets(buffer, sizeof(buffer), stdin);
	char* name = (char*)malloc((sizeof(char) * strlen(buffer)) + 1);
	if (!name)
		return NULL;
	strcpy(name, buffer);
	return name;
}

int getNumberGreaterThan(int min, const char* msg)
{
	int number;
	printf("%s\n", msg);
	while (1) {
		scanf("%d", &number);
		if (number <= min) {
			printf("Number is not greater than %d, please try again:\n", min);
			continue;
		}
		return number;
	}
}

int getNonNegativeNumber(const char* msg)
{
	int number;
	printf("%s\n", msg);
	while (1) {
		scanf("%d", &number);
		if (number < 0) {
			printf("Number is not non-negative, please try again:\n");
			continue;
		}
		return number;
	}
}

int getNumberInRange(int min, int max, const char* msg)
{
	int number;
	printf("%s\n", msg);
	while (1) {
		scanf("%d", &number);
		if (number < min || number > max) {
			printf("Please enter a number between %d and %d:\n", min, max);
			continue;
		}
		return number;
	}
}

int getSelectionFromEnumList(const char** enumStrArr, int arrSize, const char * msg, const char * invalidMsg)
{
	int option;
	printf("%s\n", msg); //prints msg based on usecase
	while (1) {
		for (int i = 0; i < arrSize; i++)
			printf("Type %d for %s\n", (i + 1), enumStrArr[i]);//prints all the enums to the user
		scanf("%d", &option);
		if (option < 1 || option > arrSize) {
			printf("%s\n", invalidMsg); //prints invalid selection msg based on usecase
			continue;
		}
		return option - 1; // -1 since we're presenting the user a menu starting at 1 but arr starts from 0
	}
}

int printLinkedListInRows(const LIST * pList, void (*print)(const void*))
{
	CHECK_VALIDITY(pList, 0);
	NODE* tmp;
	int	c = 0;
	if (!pList)
		return False;

	printf("\n");
	tmp = pList->head.next;
	while (tmp != NULL)
	{
		printf("%d. ", c + 1);
		print(tmp->key);
		printf("\n");
		c++;
		tmp = tmp->next;
	}
	printf("\n");
	return c;
}

NODE* removeNodeFromListByIndex(LIST* pList, int index)
{
	if (!pList || !&pList->head)
		return NULL;
	NODE* seletedNode = pList->head.next, *prev = &pList->head;
	for (int i = 0; i < index - 1; i++) {
		if (!seletedNode)
			return NULL;
		prev = seletedNode;
		seletedNode = seletedNode->next;
	}
	prev->next = seletedNode->next;
	seletedNode->next = NULL;
	return seletedNode;
}

void generalArrayFunction(void* arr, int numOfElements, size_t sizeofElement, void f(void* element))
{
	if (!arr || !f) return;
	for (int i = 0; i < numOfElements; i++) {
		f((char*)arr + (i * sizeofElement));
	}
}


int saveStringToBinFile(const char* str, FILE* f)
{
	CHECK_VALIDITY(f, 0);
	CHECK_VALIDITY(str, 0);
	size_t len = strlen(str) + 1;
	CHECK_VALIDITY(fwrite(&len, sizeof(int), 1, f) == 1, 0);
	CHECK_VALIDITY(fwrite(str, sizeof(char), len, f) == len, 0);
	return 1;
}

char* loadStringFromBinFile(FILE* f)
{
	CHECK_VALIDITY(f, NULL);
	int len;
	IS_READ_CORRECTLY(fread(&len, sizeof(int), 1, f), 1, NULL); //reads string len
	char* str = (char*)malloc(sizeof(char) * len);
	CHECK_VALIDITY(str, NULL);
	if (fread(str, sizeof(char), len, f) != len) //reads string
	{
		free(str);
		return NULL;
	}
	return str;
}

void uppercaseStr(char* str) { 
	//used only on strings the user entered, cannot be NULL
	for (int i = 0; str[i] != '\0'; i++)
		str[i] = toupper(str[i]);
}

char* dupStrUppercase(const char* str) {
	CHECK_VALIDITY(str, NULL);
	size_t len = strlen(str) + 1;
	char* upperStr = (char*)malloc(len);
	CHECK_VALIDITY(upperStr, NULL);
	for (int i = 0; i < len; i++) {
		upperStr[i] = toupper(str[i]);
	}
	return upperStr;
}