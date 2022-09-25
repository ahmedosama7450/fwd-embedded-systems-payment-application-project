#include <stdio.h>
#include <string.h>

#include "server.h"

#define DB_SIZE 255

// ST_accountsDB_t accounts[DB_SIZE] = { {500, "12345"}, {750, "67892"}, {2200, "54321"} };
// ST_transaction_t transactions[DB_SIZE];

ST_accountsDB_t activeAccount;
int nextSequenceNumber = 0;

int deserializeAccounts(ST_accountsDB_t* accounts, int* count) {
	FILE* file;
	fopen_s(&file, "accounts.dat", "r");
	if (file == NULL) {
		fclose(file);
		return 0;
	}

	char buffer[100];
	int i = 0;
	
	fseek(file, 0, SEEK_SET);
	fgets(buffer, sizeof(buffer), file);

	while (!feof(file)) {
		ST_accountsDB_t* a = accounts + i;
		sscanf_s(buffer, "%f, %20s", &a->balance, a->primaryAccountNumber, 20);
		fgets(buffer, sizeof(buffer), file);
		i++;
	}

	*count = i;

	fclose(file);
	return 1;
	
	//float balance;
	//char pan[21];
	//if (fscanf_s(file, "%f, %20s\n", &balance, pan, 21) < 2) {
	//	fclose(file);
	//	return ACCOUNT_NOT_FOUND;
	//}

	//pan[strlen(pan)] = 0;

}
	
int serializeAccounts(ST_accountsDB_t* accounts, int count) {
	FILE* file;
	fopen_s(&file, "accounts.dat", "w");
	if (file == NULL) {
		fclose(file);
		return 0;
	}

	fseek(file, 0, SEEK_SET);
	for (int i = 0; i < count; i++) {
		ST_accountsDB_t* a = accounts + i;
		fprintf_s(file, "%.2f, %s\n", a->balance, a->primaryAccountNumber);
	}

	fclose(file);
	return 1;
}

EN_transState_t recieveTransactionData(ST_transaction_t* transData) {
	if (isValidAccount(&transData->cardHolderData) != OK_S) {
		puts("Declined Invalid Account");
		transData->transState = DECLINED_STOLEN_CARD;
	} else if (isAmountAvailable(&transData->terminalData) != OK_S) {
		puts("Declined Insuffecient Funds");
		transData->transState = DECLINED_INSUFFECIENT_FUND;
	} else if (saveTransaction(transData) != OK_S) {
		puts("Internal Server Error");
		transData->transState = INTERNAL_SERVER_ERROR;
	} else {
		activeAccount.balance -= transData->terminalData.transAmount;
		
		// Update accounts.dat with new account balance
		ST_accountsDB_t accounts[20];
		int count;

		if (!deserializeAccounts(accounts, &count)) {
			return ACCOUNT_NOT_FOUND;
		}

		for (int i = 0; i < count; i++) {
			ST_accountsDB_t account = accounts[i];
			if (strcmp(account.primaryAccountNumber, activeAccount.primaryAccountNumber) == 0) {
				accounts[i] = activeAccount;
			}
		}
		
		serializeAccounts(accounts, count);

		transData->transState = APPROVED;
	}

	// Update transactions.dat
	FILE* file;
	fopen_s(&file, "transactions.dat", "a");
	if (file == NULL) {
		fclose(file);
		return SAVING_FAILED;
	}

	fprintf_s(file, "Transaction number %d at %s with state code: %d\nCard holder name: %s, Amount: %.2f\n\n",
		nextSequenceNumber,
		transData->terminalData.transactionDate,
		(int)transData->transState,
		transData->cardHolderData.cardHolderName,
		transData->terminalData.transAmount);

	fclose(file);

	return transData->transState;
}

EN_serverError_t isValidAccount(ST_cardData_t* cardData) {
	ST_accountsDB_t accounts[20];
	int count;
	
	if (!deserializeAccounts(accounts, &count)) {
		return ACCOUNT_NOT_FOUND;
	}
	
	for (int i = 0; i < count; i++) {	
		ST_accountsDB_t account = accounts[i];

		if (strcmp(account.primaryAccountNumber, cardData->primaryAccountNumber) == 0) {
			activeAccount.balance = account.balance;
			strcpy_s(activeAccount.primaryAccountNumber, sizeof(activeAccount.primaryAccountNumber), account.primaryAccountNumber);

			return OK_S;
		}
	}

	return ACCOUNT_NOT_FOUND;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData) {
	if (termData->transAmount > activeAccount.balance) {
		return LOW_BALANCE;
	}

	return OK_S;
}

EN_serverError_t saveTransaction(ST_transaction_t* transData) {
	if (nextSequenceNumber > DB_SIZE - 1) {
		return SAVING_FAILED;
	}

	transData->transactionSequenceNumber = nextSequenceNumber;
	nextSequenceNumber++;

	return OK_S;
}

//EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t* transData) {
//	for (int i = 0; i < DB_SIZE; i++) {
//		if (transactions[i].transactionSequenceNumber = transactionSequenceNumber) {
//			return OK_S;
//		}
//	}
//
//	return TRANSACTION_NOT_FOUND;
//}