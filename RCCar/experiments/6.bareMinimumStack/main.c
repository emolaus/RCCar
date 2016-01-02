#include <stdio.h>
#include <string.h>
#include "wiringSerial.h"

#include "client.h"

int intCommand;
int serialFD;

int fetchCommand();
void relayCommand();
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
  int serialFD = serialOpen("/dev/ttyACM0", 9600);
  // TODO while-loop 
  // - check for commands
  // - pass command on serial bus 
  
  int commandResult;
  while(1) {
    commandResult = fetchCommand();
    if (commandResult == -1 || commandResult == -2) break;
    if (commandResult >= 0) {
      printf("Command: %d\n", intCommand);
      relayCommand();
    }
    // TODO read data from Arduino
  }
  closeSocket();
  return 0;
}
int fetchCommand() {
  int result = checkCommand();
  if (intCommand == -1) {
    printf("Read error. Finishing.");
  }
  if (intCommand == -2) {
    printf("Socket closed. Finishing.");
    // TODO set speed and turn to 0;
  }
  if (result >= 0) intCommand = result;
  return result;
  
}
void relayCommand() {
  serialPrintf(serialFD, "%d\n", intCommand);
}
