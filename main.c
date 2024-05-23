#include "Company.h"
enum { eExit, ePrintCompany, eAddnewArtist, eAddnewStudio, eSortArtists, eSearchArtist, eChooseArtist, eArtistAwards, eCalcRevenue, eCreateRecording};

void printCompanyMenu();

int main() {
	int res;
	Company company;
	Artist* artist;
	EXEC_IF_FAILED(initCompanyFromFile(&company), printf("Company creation failed!\n"), 0);
	int option;
	do {
		printCompanyMenu();
		scanf("%d", &option);
		switch (option)
		{
		case ePrintCompany:
			printCompany(&company);
			break;
		case eAddnewArtist:
			if(!addNewArtist(&company)) printf("Adding artist failed!\n");
			break;
		case eAddnewStudio:
			if(!addNewStudio(&company)) printf("Adding studio failed!\n");
			break;
		case eSortArtists:
			sortArtistArr(&company);
			break;
		case eSearchArtist:
			searchArtist(&company);
			break;
		case eChooseArtist:
			artist = chooseArtist(&company);
			if(artist != NULL) doActionOnArtist(artist);
			break;
		case eArtistAwards:
			artistAwards(&company);
			break;
		case eCalcRevenue:
			printf("Company revenue: %.2lf\n\n",calcCompanyRevenue(&company));
			break;
		case eCreateRecording:
			createRecording(&company);
			break;
		case eExit:
			break;
		default:
			printf("Invalid input, please try again:\n");
			break;
		}
	} while (option != eExit);

	//saving data to files
	res = saveCompanyToBinFile(&company);
	if (!res) printf("Saving to binary file failed!\n");
	res = writeCompanyToTextFile(&company);
	if (!res) printf("Saving to text file failed!\n");
	freeCompany(&company);
}

void printCompanyMenu() {
	printf("Please choose one of the following:\n");
	printf("1. Print company general info\n");
	printf("2. Add a new artist\n");
	printf("3. Add a new studio\n");
	printf("4. Sort artist array\n");
	printf("5. Search an artist\n");
	printf("6. Present artist menu\n");
	printf("7. Show artist awards\n");
	printf("8. Calculate company's revenue\n");
	printf("9. Create new recording\n");
	printf("0. Exit the program\n");

}
