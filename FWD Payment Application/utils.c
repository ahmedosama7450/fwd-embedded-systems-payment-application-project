
#include "utils.h"


//int inputInt(char msg[]) {
//	printf(msg);
//
//	int data;
//	scanf_s("%d", &data);
//	fgetc(stdout);
//	
//	return data;
//}
//
//unsigned int inputUInt(char msg[]) {
//	char* data = inputString(msg);
//
//	int i = atoi(data);
//
//	return i >= 0 ? i : i * -1;
//}

//int inputDate(char msg[], time_t* in) {
//	char* data = inputString(msg);
//	
//	time_t t = time(NULL);
//	struct tm tm;
//	localtime_s(&tm, &t);
//	tm.tm_hour = 0;
//	tm.tm_min = 0;
//	tm.tm_sec = 0;
//
//	char* token= strtok(data, "-");
//	if (token != NULL) tm.tm_mday = atoi(token);
//	else return -1;
//	
//	token = strtok(NULL, "-");
//	if (token != NULL) tm.tm_mon = atoi(token) + 1;
//	else return -1;
//	
//	token = strtok(NULL, "-");
//	if (token != NULL) tm.tm_year = atoi(token) + 1900;
//	else return -1;
//
//	*in = mktime(&tm);
//
//	return 0;
//}
