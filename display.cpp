
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include "QuadDisplay.h"

#define DISPLAY_PIN 30

int main(int argc, char **argv) {

    if (argc == 0) {
	printf("Usage\n");
	return -1;
    }
    printf("Write: %s", argv[1]);

    wiringPiSetup();

    //displayInt(uint8_t pin, int val, bool padZeros, uint8_t dots)


    if (strcmp(argv[1],"-c") == 0) {
//	printf("Cleared\n");
	displayClear(DISPLAY_PIN);
    } else if (strcmp(argv[1],"-i") == 0) {
//	printf("Int\n");
	displayInt(DISPLAY_PIN, atoi(argv[2]), false, 0);
    } else if (strcmp(argv[1],"-t") == 0) {
//	printf("Int\n");
	displayTemperature(DISPLAY_PIN, atoi(argv[2]), false);
    } else if (strcmp(argv[1],"-e") == 0) {
	char ecode[5] = "0000";
	for (int i=0;i<strlen(argv[2]);i++) {
	    ecode[i] = argv[2][i];
	    if (ecode[i] == '_') ecode[i] = '9' + 1;
	}
	displayDigits(DISPLAY_PIN, QD_E,numerals[ecode[0] - '0'],numerals[ecode[1] - '0'], numerals[ecode[2] - '0']);
    }






    return 0;
}
