#include <Arduino.h>

void setup();
void loop();
int get_line();
#line 1 "src/sketch.ino"
void setup() {                
  Serial.begin(9600);
  pinMode(13, OUTPUT);     
}

void loop() {
  unsigned int ms_delay = 0;
  
  Serial.print("Enter an number of milliseconds: ");
  ms_delay = get_line();
  Serial.print(ms_delay);
  for(;;) {
    digitalWrite(13, HIGH);   // set the LED on
    delay(ms_delay);              // wait for a second
    digitalWrite(13, LOW);    // set the LED off
    delay(ms_delay);              // wait for a second
  }
}

int get_line() { //Function to grab the input from the user.
  char number[20];
  int index = 0;
  
  while(Serial.available() <= 0);
  delay(50);
  while(Serial.available() > 0) {
     number[index] = Serial.read();
     index++;
  }
  number[index] = 0;
  
  return atoi(number); 
}
