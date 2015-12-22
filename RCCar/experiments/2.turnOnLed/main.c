#include <stdio.h>
#include <string.h>
#include "wiringSerial.h"

int main() {
	 int res = serialOpen("/dev/ttyACM0", 9600);
printf("You opened %d\n", res);
	printf("TODO: figure out how to read a string correctly\n");
	while(1) {
		printf("Type 1 (on), 0(off) or q (quit): ");
		int c = fgetc(stdin);
		printf("\n");
		if (c == '1') {
			printf("TODO: turn on LED\n");
			serialPrintf(res, "on\n");
		}
		if (c == '0') printf("TODO: turn off LED\n");
		if (c == 'q') {
			printf("Quitting.\n");
			break;
		}
		/* FUNKAR EJ
		char command[5]; 
		printf("Type on, off or quit: ");
		fgets(command, 5, stdin);
	
		if (command[1] == '\0') printf("Last element of command is null term");
		if (strcmp(command, cmdON) == 0) printf("\n You typed on\n");
		if (strcmp(command, "off") == 0) printf("\n You typed off\n");
		if (strcmp(command, "quit") == 0) {
			printf("\n You typed quit\n");
			break;
		}*/	
	}	
	serialClose(res);
	return 0;
}
