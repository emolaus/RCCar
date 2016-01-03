/*
This example is working perfectly except for turn control,
which competes with forward/reverse thrust for current.
Serial interface:
send integer between 0 and 2512 that enables left, right and reverse or forward.
0 means standstill.
speed: command % 1000 dictates forward or reverse, where
  0-255 is forward speed
  256 - 511 is reverse speed
left / right turn: (command - speed)/1000.
  0 is go straight
  1 is turn left
  2 is turn right
*/
const int forwardPin = 3;
const int reversePin = 5;
const int leftPin = 6;
const int rightPin = 9;

const byte speedLimit = 255;

int rawCommand = 0;
int steering = 0; // 0: no turn, 1: left, 2: right
byte forwardSpeed = 0;
byte reverseSpeed = 0;

String commandString = "Command: ";
String errorString = "Command parse error: ";
class MotorOutput {
  //private
};
void setup() {
  // initialize the serial communication:
  Serial.begin(115200);
  // initialize the ledPin as an output:
  pinMode(forwardPin, OUTPUT);
  pinMode(reversePin, OUTPUT);
  pinMode(leftPin, OUTPUT);
  pinMode(rightPin, OUTPUT);
}

void loop() {
  int result = checkAndParseCommand();
  if (result < 0) {
    String report = errorString + result;
    Serial.println(report);
  }
  if (result == 0) {
    String report = commandString + rawCommand;
    Serial.println(report);
    applyMotorControl();
  }
  //delay(25);
}

int checkAndParseCommand() {
  if (Serial.available()) {
    rawCommand = Serial.parseInt();
    // Ignore all other things
    Serial.readStringUntil(10);
    
    // Verify input
    if (rawCommand < 0) {
      //Serial.println("Input wrong, ignoring command. Error -1.");
      return -1;
    }
    if (rawCommand > 2511) {
      //Serial.println("Input wrong, ignoring command. Error -2.");
      return -2;
    }
    int speedValue = rawCommand % 1000;
    // Verify input
    if (speedValue > 511) {
      //Serial.println("Input wrong, ignoring command. Error 3.");
      return -3;
    }
    int turnCommand = (rawCommand - speedValue) / 1000;
    //Serial.print("Turn command: ");
    //Serial.println(turnCommand);
    if (turnCommand < 0 || turnCommand > 2) {
      //Serial.println("Input wrong, ignoring command. Error 4.");
      return -4;
    }
    steering = turnCommand;
    // Input ok. Parse forward/reverse speed.
    //Serial.print("Speed command: ");
    //Serial.println(speedValue);
    if (speedValue <= 255) {
      // Forward.
      forwardSpeed = min(speedLimit, speedValue);
      reverseSpeed = 0;
    } else {
      reverseSpeed = min(speedLimit, speedValue - 256);
      forwardSpeed = 0;
    }
    return 0;
  }
  return 1;
}
 
void applyMotorControl() {
  analogWrite(forwardPin, forwardSpeed);
  analogWrite(reversePin, reverseSpeed);
  if (steering == 0) applyNoTurn();
  if (steering == 1) applyFullLeft();
  if (steering == 2) applyFullRight();
}
void applyNoTurn() {
  digitalWrite(leftPin, LOW);
  digitalWrite(rightPin, LOW);
}
void applyFullLeft() {
  digitalWrite(leftPin, HIGH);
  digitalWrite(rightPin, LOW);
}
void applyFullRight() {
  digitalWrite(leftPin, LOW);
  digitalWrite(rightPin, HIGH);
}

