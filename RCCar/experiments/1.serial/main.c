#include <stdio.h>
#include "wiringSerial.h"

int main ()
{
	int res = serialOpen("/dev/ttyACM0", 9600);
	printf("Result from serialOpen: %d\n", res);
	
	int counter = 0;
	while (counter++ < 1000) {
		char c = serialGetchar(res);
		if (c == '\n') printf("Newline\n");
	}

	serialClose(res);
	return 0;
}
