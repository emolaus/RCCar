#include <stdio.h>
#include <string.h>
#include "wiringSerial.h"

#include "client.h"

int intCommand;
/*
 * gcc -o main -I/home/pi/wiringpi/wiringPi/ -L/home/pi/wiringPi/devLib/ main.c client.c -lwiringPi
 * 
 * */
int main() {
  intCommand = 0;
  // TODO socket to node server
  if (openSocket() == 0) printf("Opened socket\n");
  else {
    printf("Failed opening socket.");
    exit(-1);
  }
  // Init serial bus to Arduino
  //int res = serialOpen("/dev/ttyACM0", 9600);
  // TODO while-loop 
  // - check for commands
  // - pass command on serial bus 
  
  while(1) {
    intCommand = checkCommand();
    if (intCommand == -1) {
      printf("Read error. Finishing.");
      break;
    }
    if (intCommand == -2) {
      printf("Socket closed. Finishing.");
      // TODO set speed and turn to 0;
      break;
    }
    if (intCommand < 0) continue;
    printf("Command: %d\n", intCommand);
  }
  closeSocket();
  return 0;
}
