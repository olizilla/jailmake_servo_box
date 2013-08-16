/*
Open 3 boxes with 3 servos 
*/

#include <Servo.h>

Servo s1;
Servo s2;
Servo s3;

Servo servos[] = {s1, s2, s3};
int pins[] = {9,10,11};

int closedPos = 100;
int openPos = 180;
int pauseWhileOpen = 2000;
int pauseBeforeNext = 1000;

void setup() {
  Serial.begin(9600);
  
  for (int i=0; i<3; i++) {
    servos[i].attach(pins[i]);

    // reset to closed positions.
    servos[i].write(closedPos);
  }
  // pause for 1s before we begin.
  delay(1000);
}
void loop() {
  for (int i=0; i<3; i++) {
    // move from closed to open.
    Serial.print("Running servo ");
    Serial.print(i);    
    Serial.print(" on pin ");
    Serial.print(pins[i]);
    Serial.println("");
    
    openAndClose(i);
    delay(pauseBeforeNext);
  }  
}

void openAndClose(int i){
  int pos = closedPos;
  
  for(pos = closedPos; pos <= openPos; pos += 1){
    servos[i].write(pos);
    delay(20);
  }
  
  delay(pauseWhileOpen);
  
  for(pos = openPos; pos >= closedPos; pos -= 1){
    servos[i].write(pos);
    delay(20);
  }
}
