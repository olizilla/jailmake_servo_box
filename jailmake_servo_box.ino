/*
Open 3 boxes with 3 servos
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

int numberOfServos = 1;

int pins[] = {9,10,11};
int positions[] = {closedPos,closedPos,closedPos};

void setup() { 
  Serial.begin(9600);

  delay(1000);
  
  // Initialise the servos. Write the closed pos to the servos and attach.
  for (int j=0; j < numberOfServos; j++) {
      // set to initial positions.
    servos[j].write(positions[j]);
    servos[j].attach(pins[j]);
  }
  
  delay(1000);

  // Detach the servos. They make a noise whilst attached, even when we're not writing.
  for (int h=0; h < numberOfServos; h++) {
    servos[h].detach();
  }
}

void loop() {

  now = millis();
  
  if (startCycle == 0){
    startCycle = now;
    servos[currentServo].attach(pins[currentServo]);
  }
  
  elapsedTime = now - startCycle;
  
  // write the current position to each servo.
  for (int i=0; i < numberOfServos; i++) {
    int pos = constrain(positions[i], openPos, closedPos);
    // avoid sending a signal to a servo in the closed position to reduce the noise when servos aren't doing useful work.
    if (pos != closedPos) {
       servos[i].write(pos);
    }
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
    // holding closed
    servos[currentServo].detach();
    
  } else {
    // we've done a loop, move to next servo, reset the startCycle
    currentServo = (currentServo + 1) % numberOfServos;
    startCycle = 0;
    
    servos[currentServo].attach(pins[currentServo]);
  }
  
  delay(loopDelay);
  logState();
}

// Dump useful state nuggets to the Serial monitor
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

// not called, use me to test out servo positions.
void testLoop() {
  for (int i=0; i < numberOfServos; i++) {
     servos[i].write(closedPos);
  }
  
  delay(2000);
  
  for (int j=0; j < numberOfServos; j++) {
      // set to initial positions.
    servos[j].detach();
  }
}
