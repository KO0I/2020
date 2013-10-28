#include <Arduino.h>

void setup();
void loop();
#line 1 "src/sketch.ino"
int led = 13;
boolean timeEntered = false;
String timeChar = "";
unsigned long period;
void setup() {                
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  Serial.println("ENTER A PERIOD FOR THE LIGHT TO BLINK AT");
}

// the loop routine runs over and over again forever:
void loop() {
  if(!timeEntered){ //CHECKING TO SEE IF A TIME HAS BEEN ENTERED
    while(Serial.available()){ 
    char input1 = (char)Serial.read(); //INPUT A NUMBER IN THE THE TERMINAL
      if(input1 == '\r'){ //PRESS ENTER, YOU KNOW YOU LOVE IT
        timeEntered = true; //SETS THE TIME ENTERED TO TRUE
        Serial.flush();
      }
    timeChar += input1; //SETS THE STRING TIMECHAR TO WHATEVER WAS INPUT FROM SERIAL
    period = timeChar.toInt(); //CONVERTS THE STRING TO AN INTEGER WHICH WE STORE AS PERIOD
    }
  }
  if(timeEntered){ //YOUR TIME HAS BEEN ENTERED YOU CAN NOW BEHOLD THE GLORY OF THE BLINKY
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(period);               // wait for a PERIOD
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(period);               // wait for a PERIOD
  }
}
