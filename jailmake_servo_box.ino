/*
Open 3 boxes with 3 servos 
unsigned long startLoop = millis();
unsigned long openDuration = 2000;
unsigned long closedDuration = 2000;
*/
#include <Servo.h>

unsigned long openDuration = 2000;
unsigned long pauseWhileOpen = 4000;
unsigned long closeDuration = 2000;
unsigned long pauseBeforeNext = 2000;

unsigned long now = 0;
unsigned long startCycle = 0;
unsigned long elapsedTime = 0;
unsigned long lastLogTime = 0;

int loopDelay = 20; // Change me for smother moves. Less than 50 seems to cause the servo some confusion.
int openPos = 70;
int closedPos = 150;
int currentServo = 0;

Servo s1;
Servo s2;
Servo s3;
Servo servos[] = {s1, s2, s3};

int numberOfServos = 2;

int pins[] = {9,10,11};
int positions[] = {closedPos,closedPos,closedPos};

void setup() { 
  Serial.begin(9600);
  for (int i=0; i < numberOfServos; i++) {
    servos[i].attach(pins[i]);
  }
  // pause for 1s before we begin.
  delay(1000);
  for (int j=0; j < numberOfServos; j++) {
      // set to initial positions.
    servos[j].write(positions[j]);
  }
  delay(2000);
}

void loopx() {
  for (int i=0; i < numberOfServos; i++) {
     servos[i].write(closedPos);
  }
}

void loop() {

  now = millis();
  
  if (startCycle == 0){
    startCycle = now;
  }
  
  elapsedTime = now - startCycle;
  
  // write the current position to each servo.
  for (int i=0; i < numberOfServos; i++) {
    servos[i].write(constrain(positions[i], openPos, closedPos));
  }
 
  // unpdate the postions for next pass
  if (elapsedTime <= openDuration){
    // we opening
    positions[currentServo] = map(elapsedTime, 0, openDuration, closedPos, openPos);
    
  } else if (elapsedTime <= (openDuration + pauseWhileOpen)){
    // holding open
    
  } else if (elapsedTime <= (openDuration + pauseWhileOpen + closeDuration)){
    // closing
    unsigned long t = (openDuration + pauseWhileOpen + closeDuration) - elapsedTime;
    positions[currentServo] = map(t, 0, closeDuration, closedPos, openPos);
  
  } else if (elapsedTime <= (openDuration + pauseWhileOpen + closeDuration + pauseBeforeNext)){
    // holding open
    
  } else {
    // we've done a loop, move to next servo, reset the startCycle
    currentServo = (currentServo + 1) % numberOfServos;
    startCycle = 0;
  }
  
  delay(loopDelay);
  logState();
}

void logState(){
  
  if (millis() - lastLogTime < 500){
    return;
  }
  lastLogTime = millis();
  
  for (int i=0; i < numberOfServos; i++) {
    Serial.print("pin ");
    Serial.print(pins[i]);
    Serial.print(": ");
    Serial.print(positions[i]);
    Serial.print(" | ");
  }
  Serial.print(elapsedTime);
  Serial.println("");
}

