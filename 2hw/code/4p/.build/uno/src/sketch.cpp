#include <Arduino.h>
void setup();
void loop();
#line 1 "src/sketch.ino"
void setup()
{
  Serial.begin(9600);
}

void loop()
{
  Serial.println("AM I BEING ANNOYING?");
  delay(1000);
}
