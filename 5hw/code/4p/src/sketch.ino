/******************************************************************
*  Runtime
******************************************************************/

volatile unsigned long time = 0;
volatile unsigned long check = 50;

void setup()
{
  Serial.begin(9600);
  Serial.println("Program Begin");
}

void loop()
{
  time = millis();
  //  at start of each loop, we check the time
  //  every 100ms print stuff
  Serial.print(((float)time)/1000,1);
  Serial.println("s");
  delay(100);
}
