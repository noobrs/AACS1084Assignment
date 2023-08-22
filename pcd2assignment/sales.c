#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include "Declarations.h"
#pragma warning(disable:4996)
typedef int Menu;

void salesmain() {
	call(MAIN_MENU);
}

void salesLogo() {
	ENTER;
	makeMiddle("      .^^.    .^^.      ", PAGE_WIDTH); ENTER;
	makeMiddle("     :#@@#G55G#@@#:     ", PAGE_WIDTH); ENTER;
	makeMiddle("     ^@@@@@@@@@@@@^     ", PAGE_WIDTH); ENTER;
	makeMiddle("      !&@@@@@@@@&!      ", PAGE_WIDTH); ENTER;
	makeMiddle("     ^Y&@@@@@@@@&Y^     ", PAGE_WIDTH); ENTER;
	makeMiddle("    ?&@@@@&  @@@@@&?    ", PAGE_WIDTH); ENTER;
	makeMiddle("   5@@@@@@5  JP#@@@@5   ", PAGE_WIDTH); ENTER;
	makeMiddle("  5@@@@@G^ :.. J@@@@@5  ", PAGE_WIDTH); ENTER;
	makeMiddle(" 7@@@@@@! :B&@@@@@@@@@7 ", PAGE_WIDTH); ENTER;
	makeMiddle(".#@@@@@@G^ .:~Y&@@@@@@#.", PAGE_WIDTH); ENTER;
	makeMiddle("~@@@@@@@@@#G5. 7@@@@@@@~", PAGE_WIDTH); ENTER;
	makeMiddle("~@@@@@@@P??J?  J@@@@@@@~", PAGE_WIDTH); ENTER;
	makeMiddle("^@@@@@@@5..  7B@@@@@@@@^", PAGE_WIDTH); ENTER;
	makeMiddle(" G@@@@@@@@#  #@@@@@@@@G ", PAGE_WIDTH); ENTER;
	makeMiddle(" :P@@@@@@@@@@@@@@@@@@P: ", PAGE_WIDTH); ENTER;
	makeMiddle("   ~JG#&@@@@@@@@&#GJ~   ", PAGE_WIDTH); ENTER;
	makeMiddle("      .::^^~~^^::.      ", PAGE_WIDTH); ENTER; ENTER;
}

void displayRecords(SalesRecord* salesRecords, int numberOfRecordsInFile) {
	int option;
	void (*functions[])() = { sortRecordsByName, sortRecordsByDate, sortBySalesID };
	while ((option = obtainOptionFor(2, 1, 4)) != 4) {
		functions[option - 1](salesRecords, numberOfRecordsInFile);
	}
}

void searchRecords(SalesRecord* salesRecords, int numberOfRecordsInFile, int* numberOfSearchedRecords, int indexOfSearchedRecords[]) {
	int option;
	int (*setConditionFunction[])() = { searchForProduct, searchForDate, searchByID, searchByMemberID };
	void (*obtainDataFunction[])() = { promptName, obtainDate, trigger, promptMemberID };
	option = obtainOptionFor(3, 1, 5);
	if (--option != 4) //If the --Option is 4 the User Exits the Search Menu
		searchByCondition(salesRecords, numberOfRecordsInFile, numberOfSearchedRecords, indexOfSearchedRecords, option, obtainDataFunction[option], setConditionFunction[option]);
	if (*numberOfSearchedRecords == 0 && option != 4) //If the Function Returns 0 Number of Searched Records a Message is Displayed
		printf("\nNo Such Record!\n");
}


void modifyARecord(SalesRecord* salesRecords, int numberOfRecordsInFile) {
	int numberOfSearchedRecords = 0;
	int indexOfSearchedRecords[10];
	searchRecords(salesRecords, numberOfRecordsInFile, &numberOfSearchedRecords, indexOfSearchedRecords); //The Search Record Function is Called to Retrieve Records to Modify
	if (numberOfSearchedRecords == 0)
		return 0;
	//The Search Selection Function is for Selecting The Record You want to Modify Out of all the Listed Records. Becuz the Search Function might Return more than One Record
	searchSelection(salesRecords, indexOfSearchedRecords, numberOfSearchedRecords, dataModifyMenu, 0, numberOfRecordsInFile);
	writeRecordsIntoFile(salesRecords, numberOfRecordsInFile);
}

void addARecord() {
	SalesRecord newRecord;
	//Obtains All the Details Required for the New Record
	obtainDetailsOfNewRecord(&newRecord);
	writeNewRecordIntoFile(&newRecord);
}

void deleteARecord(SalesRecord* salesRecords, int numberOfRecordsInFile) {
	int numberOfSearchedRecords = 0;
	int indexOfSearchedRecords[10];
	searchRecords(salesRecords, numberOfRecordsInFile, &numberOfSearchedRecords, indexOfSearchedRecords);
	if (!numberOfSearchedRecords)
		return 0;
	searchSelection(salesRecords, indexOfSearchedRecords, numberOfSearchedRecords, deleteSearchedRecords, 1, numberOfRecordsInFile);
}

void mainMenuDesign() {
	title("SALES INFORMATION"); ENTER;
	salesLogo(); ENTER;
	line(); ENTER;
	makeMiddle("You would like to :", PAGE_WIDTH); ENTER; ENTER;
	line();
	printf(
		"1. Display Records\n"
		"2. Search Sales Records\n"
		"3. Modify Sales Records\n"
		"4. Add A Sales Record\n"
		"5. Delete Sales Records\n"
		"6. Display Sales Report\n"
		"7. BACK\n\n"
	);
}

void displayMenu() {
	printf(
		"\n-----------------\n"
		" DISPLAY RECORDS\n"
		"-----------------\n"
		"1. Sort By Name\n"
		"2. Sort By Date\n"
		"3. Sort By Sales ID\n"
		"4. BACK\n\n"
	);
}

void searchMenu() {
	printf(
		"\n-----------------\n"
		" SEARCH RECORDS\n"
		"-----------------\n"
		"1. Search By Product Name\n"
		"2. Search By Date\n"
		"3. Search By Sales ID\n"
		"4. Search By Member ID\n"
		"5. BACK\n\n"
	);
}

void dataModificationMenu() {
	printf(
		"\n----------------\n"
		" MODIFY RECORDS\n"
		"----------------\n"
		"1. Modify Product Name\n"
		"2. Modify Quantity\n"
		"3. Modify Date\n"
		"4. Modify Member ID\n"
		"5. BACK\n\n"
	);
}


void tableHeaderForDisplayingSalesRecords() {
	printf("\n======================================================================================================================\n");
	printf("%s%66s%51s\n", "|", "SALES RECORDS", "|");
	printf("======================================================================================================================\n");
	printf("| %-4s | %17s %14s %-2s | %12s %3s %11s %7s %7s %4s %7s |\n", "Sales ID", "Product", "|", "Quantity", "Tax (6.6%)", "|", "Total", "|", "Date", "|", "Member ID");
	printf("+==========+================================+==========+================+===================+============+===========+\n");
}


void nothingMenu() {
	printf("");
}

void searchSelectionMenu(SalesRecord* salesRecords, int indexOfSearchedRecords[], int numberOfSearchedRecords, int whichFunction) {
	//Depends on the Value of whichFunction, the Title of the Menu is Altered
	char modifyOrDelete[][40] = { {"Modify?"}, {"Delete?"} };
	if (numberOfSearchedRecords == 0)
		return 0;
	printf("=====================");
	printf("\n%14s%7s\n", modifyOrDelete[whichFunction], "|");
	printf("=====================\n");
	printf("| Choice | Sales ID |\n");
	printf("=====================\n");
	for (int i = 0; i < numberOfSearchedRecords; i++) {
		printf("| %3d %4s %6s   |\n", i + 1, "|", salesRecords[indexOfSearchedRecords[i]].salesID);
		printf("=====================\n");
	}
	//If whichFunction is 1, Which is the Calling of the Delete Records Function, And the Records Returned is > 1, A Different Menu is Displayed
	if (whichFunction && numberOfSearchedRecords > 1) {
		printf("| %3d %4s %6s   |\n", numberOfSearchedRecords + 1, "|", "All");
		printf("=====================\n");
		printf("| %3d %4s %6s   |\n", numberOfSearchedRecords + 2, "|", "Exit");
		printf("=====================\n\n");
		return 0;
	}
	//Else the Menu Format Will be As Shown Below
	printf("| %3d %4s %6s   |\n", numberOfSearchedRecords + 1, "|", "Exit");
	printf("=====================");
	printf("\n\n");
}


void salesReportHeader() {
	printf("\n+============================================================+\n");
	printf("|%36s%25s\n", "Sales Ranking", "|");
	printf("+============================================================+\n");
	printf("| %5s | %15s %11s %s | %10s %5s\n", "Rank", "Product", "|", "Sales", "Profit", "|");
	printf("+============================================================+\n");
}


void call(int whichMenu) {
	//Contains the Main Menu Functions
	void (*functions[])() = { displayRecords, searchRecords, modifyARecord, addARecord, deleteARecord, salesPerformanceReport };
	menuOption(functions, whichMenu);
}

void menuOption(void (*functions[])(), int whichMenu) {
	SalesRecord salesRecords[MAX_SALES];
	Menu option;
	//Obtains Option for Main Menu
	while ((option = obtainOptionFor(whichMenu, 1, 7)) != 7) {
		loopMainFunctions(functions, option);
	}
}

char loopMainFunctions(void (*functions[])(), int option) {
	SalesRecord salesRecords[MAX_SALES];
	char proceed = 'N';
	//Depends on Which Option is Chosen, A Message is Displayed Accordingly
	char loop[][40] = { {"Search Another Record"}, {"Modify Another Record"}, {"Add Another Record"}, {"Delete Another Record"} };
	do {
		int numberOfSearchedRecords = 0;
		int indexOfSearchedRecords[MAX_SALES];
		functions[option - 1](salesRecords, obtainSalesRecordsFromFile(salesRecords), &numberOfSearchedRecords, indexOfSearchedRecords);
		if (option != 1 && option != 6) {
			proceed = yesOrNoFunction(loop[option - 2]);
		}
	} while (proceed != 'N');
}

//Used For Displaying Appropriate Menu's According to whichMenu and Obtaining Input for the Menu's
int obtainOptionFor(int whichMenu, int lowerLimit, int upperLimit) {
	Menu option;
	void (*menuFunctions[])() = { mainMenuDesign, dataModificationMenu, displayMenu, searchMenu, nothingMenu };
	menuFunctions[whichMenu]();
	do {
		printf("Enter your selection (%d-%d) > ", lowerLimit, upperLimit);
	} while (!scanf("%d", &option) || (option < lowerLimit || option > upperLimit));
	rewind(stdin);
	return option;
}

//Used for All Yes Or No Situations
char yesOrNoFunction(const char enquiry[]) {
	char yesNo;
	rewind(stdin);
	printf("\n%s (Y-Yes N-No)? > ", enquiry);
	while ((yesNo = toupper(getchar())) != 'N' && yesNo != 'Y') {
		rewind(stdin);
		printf("\nInvalid Input! Enter Y Or N Only > ");
	};
	rewind(stdin);
	return yesNo;
}

int obtainSalesRecordsFromFile(SalesRecord* salesRecords) {
	FILE* salesFile = openFileFor(READING);
	return numberOf(salesRecords, FROM_SALES_FILE);
}

FILE* openFileFor(const char* accessMode) {
	FILE* salesFile = fopen("sales.bin", accessMode);
	if (salesFile == NULL) {
		printf("Error Opening File");
		exit(-1);
	}
	return salesFile;
}

int numberOf(SalesRecord* row, FILE* salesFile) {
	int i = 0;
	while (fread(&row[i], sizeof(SalesRecord), 1, salesFile)) {
		i++;
	}
	FILE_CLOSE;
	return i;
}

void displayRecordsOrRecord(SalesRecord salesRecords[], int loopCount) {
	tableHeaderForDisplayingSalesRecords();
	for (int i = 0; i < loopCount; i++) {
		displayRecord(&salesRecords[i]);
	}
	printf("\n");
}

//Displays Single Record Without Displaying Menu
void displayRecord(SalesRecord* salesRecord) {
	StockInfo stock[MAX_STOCK_SIZE];
	int size = 0;
	readStockFile(stock, &size);
	printf("| %6s %3s %s%-29s | %5d %4s %4s%-8.2f %3s %6s%-11.2f %s %02d/%02d/%-4d | %7s %3s\n", salesRecord->salesID, "|", (checkExistedName(salesRecord->itemName, stock, &size) < 0) ? ("*") : (" "), salesRecord->itemName, salesRecord->itemQuantity, "|", "$", salesRecord->salesTax, "|", "$", salesRecord->salesTotal, "|", salesRecord->salesDate.day, salesRecord->salesDate.month, salesRecord->salesDate.year, salesRecord->memberId, "|");
	printf("+==========+================================+==========+================+===================+============+===========+\n");
}

void sortBySalesID(SalesRecord* salesRecords, int numberOfRecordsInFile) {
	system("cls");
	displayRecordsOrRecord(salesRecords, numberOfRecordsInFile);
}

void sortRecordsByName(SalesRecord* line, int numberOfRecordsInFile) {
	system("cls");
	tableHeaderForDisplayingSalesRecords();
	for (int i = 65; i < 90; i++) {
		for (int j = 0; j < numberOfRecordsInFile; j++) {
			if ((int)(line[j].itemName[0]) == i) {
				displayRecord(&line[j]);
			}
		}
	}
	system("pause");
}

void sortRecordsByDate(SalesRecord* line, int numberOfRecordsInFile) {
	system("cls");
	tableHeaderForDisplayingSalesRecords();
	for (int i = 2022; i > 2000; i--) {
		for (int j = 12; j > 0; j--) {
			for (int k = 31; k > 0; k--) {
				for (int z = 0; z < numberOfRecordsInFile; z++) {
					if (line[z].salesDate.year == i && line[z].salesDate.month == j && line[z].salesDate.day == k) {
						displayRecord(&line[z]);
					}
				}
			}
		}
	}
	system("pause");
}

void searchByMemberID(char inputMID[], char mIDInFile[]) {
	return strcmp(inputMID, mIDInFile) == 0 ? 1 : 0;
}

int searchForProduct(char input[], char itemName[]) {
	return strcmp(input, itemName) == 0 ? 1 : 0;
}

int searchForDate(SalesRecord* inputDate, SalesRecord* salesRecord) {
	return (inputDate->salesDate.day == salesRecord->salesDate.day) && (inputDate->salesDate.month == salesRecord->salesDate.month) && (inputDate->salesDate.year == salesRecord->salesDate.year) == 1 ? 1 : 0;
}

int searchByID(char input[], char salesID[]) {
	return strcmp(input, salesID) == 0 ? 1 : 0;
}

//Function That Returns the Number of Searched Records 
//The Function Pointers Are Used for Specifying Conditions and Prompts for the Searches
void searchByCondition(SalesRecord* salesRecords, int numberOfRecordsInFile, int* numberOfSearchedRecords, int indexOfSearchedRecords[], int whichSet, void(*obtainDataFunction)(), int (*setConditionFunction)()) {
	SalesRecord  salesRecord;
	SalesRecord recordsForDisplay[MAX_SALES];
	(whichSet == 0) ? (obtainDataFunction(salesRecord.itemName, "\nEnter the Product Name > ")) : (whichSet == 1) ? (obtainDataFunction(&salesRecord)) : (whichSet == 2) ? (obtainDataFunction(promptForSalesID, salesRecord.salesID)) : (obtainDataFunction(salesRecord.memberId));
	for (int i = 0; i < numberOfRecordsInFile; i++) {
		if ((whichSet == 0) ? (setConditionFunction(salesRecord.itemName, salesRecords[i].itemName)) : (whichSet == 1) ? (setConditionFunction(&salesRecord, &salesRecords[i])) : (whichSet == 2) ? (setConditionFunction(salesRecord.salesID, salesRecords[i].salesID)) : (setConditionFunction(salesRecord.memberId, salesRecords[i].memberId))) {
			recordsForDisplay[*numberOfSearchedRecords] = salesRecords[i];
			indexOfSearchedRecords[(*numberOfSearchedRecords)++] = i;
		}

	}
	if (*numberOfSearchedRecords > 0)
		displayRecordsOrRecord(recordsForDisplay, *numberOfSearchedRecords);
}

//This Function Allows the User to Select which Record to Modify or Delete
//The indexOfSearchedRecords Stores the Indexes of the Searched Records in the Structure Array
void searchSelection(SalesRecord* salesRecords, int indexOfSearchedRecords[], int numberOfSearchedRecords, void (*modifyOrDelete)(), int whichMenu, int numberOfRecordsInFile) {
	int option;
	char deleteAll;
	searchSelectionMenu(salesRecords, indexOfSearchedRecords, numberOfSearchedRecords, whichMenu);
	if (whichMenu && numberOfSearchedRecords > 1)
		option = obtainOptionFor(4, 1, numberOfSearchedRecords + 2);
	else
		option = obtainOptionFor(4, 1, numberOfSearchedRecords + 1);
	rewind(stdin);
	if (whichMenu && numberOfSearchedRecords > 1 && (option == numberOfSearchedRecords + 1)) {
		if (yesOrNoFunction("Confirm Deletion Of All Listed Records") == 'Y') {
			modifyOrDelete(salesRecords, numberOfRecordsInFile, indexOfSearchedRecords, numberOfSearchedRecords);
			printf("\nRecords Successfully Deleted\n");
		}
		return;
	}
	if (!whichMenu && (option != (numberOfSearchedRecords + 1))) {
		modifyOrDelete(salesRecords, numberOfRecordsInFile, &indexOfSearchedRecords[--option], 1);
		printf("\nRecord Successfully Modified\n");

	}
	if (whichMenu && (option != numberOfSearchedRecords + 2) && numberOfSearchedRecords > 1) {
		modifyOrDelete(salesRecords, numberOfRecordsInFile, &indexOfSearchedRecords[--option], 1);

	}
	if (whichMenu && (option != numberOfSearchedRecords + 1) && numberOfSearchedRecords == 1) {
		modifyOrDelete(salesRecords, numberOfRecordsInFile, &indexOfSearchedRecords[--option], 1);
	}
}

void dataModifyMenu(SalesRecord* salesRecord) {
	StockInfo stock[MAX_STOCK_SIZE];
	Members members[MAX_MEMBER];
	Members singleMember;
	strcpy(singleMember.mID, salesRecord->memberId);
	//Used to Compare the Old and Updated Member Index
	int oldMemberIndex;
	if ((oldMemberIndex = mChkRedundancy(&singleMember)) == 0) { // If the Member Can't Be Found in the Member File
		printf("Member has been deleted or modified from Member Module! Record cannot be modified!\n");
		PAUSE;
		return;
	}
	int newMemberIndex;
	--oldMemberIndex;//Old member index = 1
	int numberOfMemberRecords = rData(members);
	int size = 0;
	char storeOldStatus[50]; //Stores the Member Status Before The Update
	newMemberIndex = 0;
	strcpy(storeOldStatus, members[oldMemberIndex].mStatus);
	char storeAnotherStatus[50];
	readStockFile(stock, &size);
	int index;
	//Obtain Index of Product In Stock File
	if ((index = checkExistedName(salesRecord->itemName, stock, &size)) == -1) {
		printf("Product has been deleted from Stock Module! Record cannot be modified!\n");
		PAUSE;
		return;
	}
	int newIndex = index;//Used to Compare Old and Updated Stock Indexes 
	Menu option;
	int (*modifyFunctions[5])(SalesRecord*) = { obtainProductName, obtainProductQuantity, obtainDate, promptMemberIDForDataModify };
	while ((option = obtainOptionFor(1, 1, 5)) != 5) {
		(--option) == 0 ? (newIndex = modifyFunctions[option](salesRecord, stock, size)) : (option == 3) ? modifyFunctions[option](salesRecord, &newMemberIndex) : (modifyFunctions[option](salesRecord, stock, index, size));//Update New Index Every Iteration
		if (newIndex != index) {//If the New Name is not Equal to the Old Name 
			stock[newIndex].qtyInStock -= salesRecord->itemQuantity;// Minus the Quantity of the New Name In Stock
			stock[index].qtyInStock += salesRecord->itemQuantity;// Plus the Quantity of the Old Name In Stock
			index = newIndex;//Assign the New Index as the Old Index
			if (!compareStockBetween(stock[index].minLvl, (stock[index].qtyInStock))) {
				modifyFunctions[1](salesRecord, stock, index, size);//Enter the New Quantity
			};
		}
		if (option == 1 || option == 0) {
			members[oldMemberIndex].mTotalSales -= salesRecord->salesTotal;
			salesRecord->salesTax = stock[index].sellPrice * salesRecord->itemQuantity * 0.066;
			salesRecord->salesTotal = stock[index].sellPrice * salesRecord->itemQuantity + salesRecord->salesTax;
			members[oldMemberIndex].mTotalSales += salesRecord->salesTotal;
			if (!compareStockBetween(stock[index].minLvl, (stock[index].qtyInStock))) {// If the Quantity in Stock is Below Min Lvl
				modifyFunctions[1](salesRecord, stock, index, size);//Enter the New Quantity
			};
		}
		if ((newMemberIndex - 1) != oldMemberIndex && option == 3) {
			strcpy(storeAnotherStatus, members[--newMemberIndex].mStatus);
			members[newMemberIndex].mTotalSales += salesRecord->salesTotal;
			members[oldMemberIndex].mTotalSales -= salesRecord->salesTotal;
			updateMemberStatus(&members[oldMemberIndex]);
			updateMemberStatus(&members[newMemberIndex]);
			if (strcmp(storeOldStatus, members[oldMemberIndex].mStatus)) {
				strcpy(storeOldStatus, members[oldMemberIndex].mStatus);
				printf("\nOops! %s %s Has Been Demoted To A %s Member!\n", members[oldMemberIndex].mName, members[oldMemberIndex].mID, members[oldMemberIndex].mStatus);
			}
			if (strcmp(storeAnotherStatus, members[newMemberIndex].mStatus)) {
				printf("\nCongratulations! %s %s Is Now A %s Member!\n", members[newMemberIndex].mName, members[newMemberIndex].mID, members[newMemberIndex].mStatus);
			}

			oldMemberIndex = newMemberIndex;
		}
		writeStockFile(stock, &size);
		wData(members, numberOfMemberRecords);
		writeStockFile(stock, &size);
	}
}

int promptMemberIDForDataModify(SalesRecord* salesRecord, int* newIndex) {
	Members singleM;
	char inputMID;
	do {
		printf("\nEnter The Member's ID > ");
		scanf("%s", salesRecord->memberId);
		rewind(stdin);
		strcpy(singleM.mID, salesRecord->memberId);
	} while ((*newIndex = mChkRedundancy(&singleM)) == 0);
}

void updateMemberStatus(Members* member) {
	if (member->mTotalSales < 5000)
		strcpy(member->mStatus, "IRON");
	else if (member->mTotalSales < 10000)
		strcpy(member->mStatus, "BRONZE");
	else if (member->mTotalSales < 20000)
		strcpy(member->mStatus, "SILVER");
	else if (member->mTotalSales < 50000)
		strcpy(member->mStatus, "GOLD");
	else if (member->mTotalSales < 100000)
		strcpy(member->mStatus, "PLATINUM");
	else
		strcpy(member->mStatus, "DIAMOND");
}

int obtainProductQuantity(SalesRecord* salesRecord, StockInfo* stock, int index, int size) {
	int tempQty;
	do {
		tempQty = promptQty("\nEnter Product Quantity  > ");
		if (stock[index].minLvl > stock[index].qtyInStock - (tempQty - salesRecord->itemQuantity))
			printf("\nProduct %s Only %d Left In Stock!\n", stock[index].prodName, stock[index].qtyInStock - stock[index].minLvl);
	} while (!compareStockBetween(stock[index].minLvl, (stock[index].qtyInStock - (tempQty - salesRecord->itemQuantity))));
	stock[index].qtyInStock -= (tempQty - salesRecord->itemQuantity);//110
	salesRecord->itemQuantity = tempQty;//10
}

int obtainProductName(SalesRecord* salesRecord, StockInfo* stock, int size) {
	char buffer[50];
	int index;
	do {
		promptName(buffer, "\nEnter Product Name > ");
	} while ((index = checkExistedName(buffer, stock, &size)) < 0);
	strcpy(salesRecord->itemName, stock[index].prodName);
	return index;
}

void writeRecordsIntoFile(SalesRecord* salesRecords, int numberOfRecordsInFile) {
	FILE* salesFile = openFileFor(WRITING);
	fwrite(salesRecords, sizeof(SalesRecord), numberOfRecordsInFile, salesFile);
	FILE_CLOSE;
}

//Auto Generates the Sales ID for a New Record
void generateSalesId(SalesRecord* salesRecord) {
	SalesRecord recordsInFile[MAX_SALES];
	int newSequenceNumber = 0;
	for (int i = 0, highestSequenceNumber = 0; i < obtainSalesRecordsFromFile(recordsInFile); i++) {
		sscanf(recordsInFile[i].salesID, "S%d", &highestSequenceNumber);
		if (highestSequenceNumber > newSequenceNumber)
			newSequenceNumber = highestSequenceNumber;
	}
	sprintf(salesRecord->salesID, "S%03d", ++newSequenceNumber);
}

int validateIfIsValidDate(Date* dateOfSale) {

	if (dateOfSale->day < 32 || dateOfSale->day > 0 || dateOfSale->month < 13 || dateOfSale->month > 0 || dateOfSale->year < 2023 || dateOfSale->year > 0) {
		if (dateOfSale->month != 1 && dateOfSale->month != 3 && dateOfSale->month != 5 && dateOfSale->month != 7 && dateOfSale->month != 8 && dateOfSale->month != 11 && dateOfSale->month != 12 && dateOfSale->day > 30)
			return 1;
		if (dateOfSale->month == 2 && dateOfSale->day > 28) {
			if ((dateOfSale->year % 4) != 0)
				return 1;
			else if (dateOfSale->day > 29)
				return 1;
		}
		return 0;
	}
	else
		return 1;
}

void trigger(void (*solicit)(), char* solicitedData) {
	solicit();
	scanf("%[^\n]", solicitedData);
	rewind(stdin);
}

void promptForSalesID() {
	printf("\nEnter The Sales ID > ");
}

//Counts the Loop Count of Loops and Displays An Error Message if it is Looped A Second time or More
void promptsAndErrorMessages(int* loopCount, int errorMessage) {
	char dataRequest[][100] = { { "\nEnter Date by (DD/MM/YYYY) > " }, { "\nInvalid Date. Re-enter Date (DD/MM/YYYY) > " } };
	printf(dataRequest[*loopCount = (*loopCount >= (errorMessage) ? errorMessage : *loopCount)]);
	(*loopCount)++;
	rewind(stdin);
}

void recordNotFoundMessage() {
	printf("\nNo Such Record Was Found!\n");
}

void writeNewRecordIntoFile(SalesRecord* newRecord) {
	if (yesOrNoFunction("Confirm Addition") == 'N')
		return 0;
	printf("\nRecord Successfully Added\n");
	FILE* salesFile = openFileFor(APPENDING);
	fwrite(newRecord, sizeof(SalesRecord), 1, salesFile);
	FILE_CLOSE;
}

void obtainDate(SalesRecord* salesRecord) {
	int loopCount = 0;
	do {
		promptsAndErrorMessages(&loopCount, 1);
	} while (scanf("%d/%d/%d", &salesRecord->salesDate.day, &salesRecord->salesDate.month, &salesRecord->salesDate.year) != 3 || validateIfIsValidDate(&salesRecord->salesDate));
	rewind(stdin);
}

void obtainDetailsOfNewRecord(SalesRecord* newRecord) {
	Members members[MAX_MEMBER];
	Members singleMember;
	StockInfo stock[MAX_STOCK_SIZE];
	char inputID[MAX_CHAR_MEMBER_ID];
	int size = 0;
	int memberIndex;
	int numberOfRecordsInMemberFile = rData(members);
	readStockFile(stock, &size);
	int stockIndex = obtainProductName(newRecord, stock, size);
	char storeOldStatus[20];
	newRecord->itemQuantity = NULL;
	generateSalesId(newRecord);
	if (stock[stockIndex].minLvl == stock[stockIndex].qtyInStock) {
		printf("\nStock Has Reached its Min Lvl! Enter Another Product!\n");
		stockIndex = obtainProductName(newRecord, stock, size);
	}
	do {
		obtainProductQuantity(newRecord, stock, stockIndex, size);
	} while (!compareStockBetween(stock[stockIndex].minLvl, (stock[stockIndex].qtyInStock)));
	promptMemberIDForDataModify(newRecord, &memberIndex);
	strcpy(storeOldStatus, members[--memberIndex].mStatus);
	newRecord->salesTax = stock[stockIndex].sellPrice * newRecord->itemQuantity * 0.066;
	newRecord->salesTotal = stock[stockIndex].sellPrice * newRecord->itemQuantity + newRecord->salesTax;
	members[memberIndex].mTotalSales += newRecord->salesTotal;
	updateMemberStatus(&members[memberIndex]);
	if (strcmp(storeOldStatus, members[memberIndex].mStatus))
		printf("\nCongratulations! %s %s Is Now A %s Member!\n", members[memberIndex].mName, members[memberIndex].mID, members[memberIndex].mStatus);
	obtainDate(newRecord);
	writeStockFile(stock, &size);
	wData(members, numberOfRecordsInMemberFile);
}

void promptMemberID(char inputMID[]) {
	printf("\nEnter The Member's ID > ");
	scanf("%s", inputMID);
	rewind(stdin);
}

void deleteSearchedRecords(SalesRecord* salesRecords, int numberOfRecordsInFile, int indexOfSearchedRecords[], int numberOfSearchedRecords) {
	if (numberOfSearchedRecords == 1) {
		if (yesOrNoFunction("Confirm Deletion") == 'N')
			return 0;
	}
	int num, size;
	num = size = 0;
	SalesRecord updatedRecords[MAX_SALES];
	StockInfo stock[MAX_STOCK_SIZE];
	Members members[MAX_MEMBER];
	Members singleMember;
	readStockFile(stock, &size);
	int stockIndex;
	int numberOfMemberRecords = rData(members);
	for (int i = 0; i < numberOfRecordsInFile; i++) {
		int skipIndex = 0;
		for (int j = 0; j < numberOfSearchedRecords; j++) {
			if (indexOfSearchedRecords[j] == i) {
				strcpy(singleMember.mID, salesRecords[i].memberId);
				char storeOldStatus[50];
				if ((stockIndex = checkExistedName(salesRecords[i].itemName, stock, &size)) != -1)
					stock[stockIndex].qtyInStock += salesRecords[i].itemQuantity;
				int memberIndex = mChkRedundancy(&singleMember);
				--memberIndex;
				strcpy(storeOldStatus, members[memberIndex].mStatus);
				members[memberIndex].mTotalSales -= salesRecords[i].salesTotal;
				updateMemberStatus(&members[memberIndex]);
				if (strcmp(storeOldStatus, members[memberIndex].mStatus))
					printf("\nOops! %s %s Has Been Demoted To A %s Member!\n", members[memberIndex].mName, members[memberIndex].mID, members[memberIndex].mStatus);
				skipIndex = 1;
			}
		}
		if (!skipIndex)
			updatedRecords[num++] = salesRecords[i];
	}
	writeRecordsIntoFile(updatedRecords, num);
	writeStockFile(stock, &size);
	wData(members, numberOfMemberRecords);
	if (numberOfSearchedRecords == 1)
		printf("\nRecord Successfully Deleted\n");
}

void salesPerformanceReport(SalesRecord* salesRecords, int numberOfSalesRecords) {
	StockInfo stock[MAX_STOCK_SIZE];
	int size = 0;
	readStockFile(stock, &size);
	int quantitySold[MAX_STOCK_SIZE] = { NULL };
	salesReportHeader();
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < numberOfSalesRecords; j++) {
			if (!strcmp(stock[i].prodName, salesRecords[j].itemName))
				quantitySold[i] += salesRecords[j].itemQuantity;
		}
	}
	for (int i = 0; i < size; i++) {
		int highestInIteration = NULL;
		int number = NULL;
		for (int j = 0; j < size; j++) {
			if (quantitySold[j] > number) {
				highestInIteration = j;
				number = quantitySold[j];
			}
		}
		printf("| %3d %3s %-25s | %3d %3s %5s%-9.2f |\n", i + 1, "|", stock[highestInIteration].prodName, quantitySold[highestInIteration], "|", "$", quantitySold[highestInIteration] * (stock[highestInIteration].sellPrice - stock[highestInIteration].costPrice));;
		printf("+============================================================+\n");
		quantitySold[highestInIteration] = 0;
	}
	system("pause");
}