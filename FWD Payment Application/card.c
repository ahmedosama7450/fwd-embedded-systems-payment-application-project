#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "card.h" 
#include "utils.h"

int isPosDigit(int c) {
	return isdigit(c) && c >= 0;
}

EN_cardError_t getCardHolderName(ST_cardData_t* cardData) {
	char name[26];
	printf("Enter card holder name: ");
	fgets(name, sizeof(name), stdin);
	name[strlen(name) - 1] = '\0';

	size_t len = strlen(name);
	if (name == NULL || len < 5 || len > 10) { // TODO
		return WRONG_NAME;
	}
	
	strcpy_s(cardData->cardHolderName, sizeof(cardData->cardHolderName), name);
	return OK_C;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData) {
	char date[7];
	printf("Enter card expiry date: ");
	fgets(date, sizeof(date), stdin);
	date[strlen(date) - 1] = '\0';

	size_t len = strlen(date);
	if (date == NULL || len != 5 || date[2] != '/'
		|| !isPosDigit(date[0]) || !isPosDigit(date[1]) 
		|| !isPosDigit(date[3]) || !isPosDigit(date[4])
		|| (date[0] != '0' && date[0] != '1')
		|| date[1] == '0') {
		return WRONG_EXP_DATE;
	}

	strcpy_s(cardData->cardExpirationDate, sizeof(cardData->cardExpirationDate), date);
	return OK_C;
}

EN_cardError_t getCardPAN(ST_cardData_t* cardData){
	char pan[21];
	printf("Enter primary account number: ");
	fgets(pan, sizeof(pan), stdin);
	pan[strlen(pan) - 1] = '\0';

	size_t len = strlen(pan);
	if (pan == NULL || len < 5 || len > 19) { // TODO
		return WRONG_PAN;
	}

	strcpy_s(cardData->primaryAccountNumber, sizeof(cardData->primaryAccountNumber), pan);
	return OK_C;
}