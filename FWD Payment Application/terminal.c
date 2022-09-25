#include <stdio.h>
#include <time.h>
#include <string.h>

#include "terminal.h"

EN_terminalError_t getTransactionDate(ST_terminalData_t* termData) {
	// Get current date
	time_t t = time(NULL);
	struct tm tm;
	localtime_s(&tm, &t);

	// Format string DD/MM/YYYY
	char date[11];
	snprintf(date, sizeof(date), "%02d/%02d/%02d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);

	strcpy_s(termData->transactionDate, sizeof(termData->transactionDate), date);
	return OK_T;
}

EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData) {
	int expiryMonth, expiryYear;
	sscanf_s(cardData.cardExpirationDate, "%d/%d", &expiryMonth, &expiryYear);
	expiryYear += 2000;
	
	int transDay, transMonth, transYear;
	sscanf_s(termData.transactionDate, "%d/%d/%d", &transDay, &transMonth, &transYear);

	if (expiryYear < transYear ||
		(expiryYear == transYear && expiryMonth < transMonth))
		return EXPIRED_CARD;
	
	return OK_T;
}

EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData) {
	char* pan = cardData->primaryAccountNumber;
	int nDigits = strlen(pan);

	int sum = 0, isSecond = 0;
	for (int i = nDigits - 1; i >= 0; i--) {

		int d = pan[i] - '0';

		if (isSecond == 1) d = d * 2;

		// We add two digits to handle
		// cases that make two digits after
		// doubling
		sum += d / 10;
		sum += d % 10;

		isSecond = !isSecond;
	}

	return (sum % 10 == 0) ? OK_T : INVALID_CARD;
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData) {
	printf("Enter transaction amount: ");

	float amount;
	scanf_s("%f", &amount);
	fgetc(stdin);

	if (amount <= 0) {
		return INVALID_AMOUNT;
	}

	termData->transAmount = amount;
	return OK_T;
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData) {
	if (termData->transAmount > termData->maxTransAmount) {
		return EXCEED_MAX_AMOUNT;
	}

	return OK_T;
}

EN_terminalError_t setMaxAmount(ST_terminalData_t* termData, float max) {
	if (max <= 0) {
		return INVALID_MAX_AMOUNT;
	}

	termData->maxTransAmount = max;
	return OK_T;
}