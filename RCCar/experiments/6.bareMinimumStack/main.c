#include <stdio.h>
#include <string.h>
#include "wiringSerial.h"

#include "client.h"

int intCommand;
int serialFD;

int fetchCommand();
void relayCommand();
int checkAndPrintArduinoInfo();
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
  serialFD = serialOpen("/dev/ttyACM0", 115200);
  printf("Opened serial device %d\n", serialFD);
  // TODO while-loop 
  // - check for commands
  // - pass command on serial bus 
  
  int commandResult;
  while(1) {
    commandResult = fetchCommand();
    if (commandResult == -1 || commandResult == -2) {
      printf("Socket closed. Exit.");
      serialClose(serialFD);
      break;
    }
    if (commandResult >= 0) {
      printf("Command: %d\n", intCommand);
      relayCommand();
    }
    // TODO read data from Arduino
    if (checkAndPrintArduinoInfo() < 0) {
      printf("Arduino disconnected. Exit.");
      break;
    }
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
  printf("Relaying %d\n", intCommand);
  serialPrintf(serialFD, "%i\n", intCommand);
}
int checkAndPrintArduinoInfo() {
  int charCount = serialDataAvail(serialFD);
  if (charCount < 0) return -1;
  
  if (charCount == 0) return 0;
  int i;
  printf("Arduino reports: ");
  for (i = 0; i < charCount; i++) {
    printf("%c", serialGetchar(serialFD));
  } 
  return 0;
}
