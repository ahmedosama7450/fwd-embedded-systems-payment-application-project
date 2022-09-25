#include <stdio.h>
#include <stdlib.h>

#include "app.h"
#include "card.h"
#include "terminal.h"
#include "server.h"

void start();

void appStart(void) {
	while (1) {
		printf("====================================\n");
		printf("Welcome to the payment application\n");
		printf("====================================\n");

		start();

		printf("\n\nPress enter to start the application again\n");
		fgetc(stdin);
		system("cls");
	}
}

void start() {
	// Card
	ST_cardData_t cardData;
	if (getCardHolderName(&cardData) != OK_C) {
		puts("Holder name doesn't seem right!");
		return;
	}
	if (getCardExpiryDate(&cardData) != OK_C) {
		puts("Expiry date doesn't seem right!");
		return;
	}
	if (getCardPAN(&cardData) != OK_C) {
		puts("Primary account number doesn't seem right!");
		return;
	}

	puts("\nData received:");
	printf("Holder Name: %s\nExpiry date: %s\nPrimary Account Number: %s\n\n",
		cardData.cardHolderName,
		cardData.cardExpirationDate,
		cardData.primaryAccountNumber);

	// Terminal
	if (isValidCardPAN(&cardData) != OK_T) {
		puts("Primary account number is not a correct Luhn Number");
		return;
	}

	ST_terminalData_t terminalData;
	if (getTransactionDate(&terminalData) != OK_T) {
		puts("Transaction date doesn't seem right!");
		return;
	}

	printf("Transaction Date from system: %s\n\n", terminalData.transactionDate);

	if (isCardExpired(cardData, terminalData) != OK_T) {
		puts("Card is Expired");
		return;
	}
	if (setMaxAmount(&terminalData, 500) != OK_T) {
		puts("Invalid Max Amount");
		return;
	}
	if (getTransactionAmount(&terminalData) != OK_T) {
		puts("Invalid Amount");
		return;
	}
	if (isBelowMaxAmount(&terminalData) != OK_T) {
		puts("Max Amount Exceeded");
		return;
	}

	// Server
	ST_transaction_t transaction;
	transaction.cardHolderData = cardData;
	transaction.terminalData = terminalData;
	if (recieveTransactionData(&transaction) == APPROVED) {
		printf("Transaction has been successfully processed");
	}
}
