#include <Arduino.h>
void setup();
void loop();
#line 1 "src/sketch.ino"
  const boolean bPenguin    = true;
  const boolean bFrog       = false;
  const int iTurtle         = 0x19;
  const int iRabbit         = B00001111;
  const int iHampster       = 0;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  Serial.println(iTurtle & iRabbit);
  Serial.println(iTurtle && iRabbit);
  Serial.println(bPenguin && iRabbit);
  Serial.println(iHampster & iTurtle);
  Serial.println(iTurtle  || iRabbit);
  Serial.println(iTurtle  | iRabbit);
  Serial.println(iTurtle  | bFrog);
  Serial.println(iHampster  ||  bPenguin);
  delay(1000);
  delay(1000);
  delay(1000);
  delay(1000);
  delay(1000);
  delay(1000);
  delay(1000);
  delay(1000);

}
