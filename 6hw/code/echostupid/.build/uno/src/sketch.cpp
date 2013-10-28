#include <Arduino.h>

void setup();
void loop();
#line 1 "src/sketch.ino"
byte byteRead;

void setup(){
Serial.begin(9600);
}

void loop(){
  if (Serial.available()){
  byteRead = Serial.read();
  Serial.write(byteRead);
  }
}
