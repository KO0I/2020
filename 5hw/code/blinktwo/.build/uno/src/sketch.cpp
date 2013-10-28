#include <Arduino.h>

void setup();
void loop();
void led13Switch();
void led12Switch();
#line 1 "src/sketch.ino"
//BLINKS TWO LEDS AT DIFFERENT RATES
//USED SOME EXAMPLE CODE FROM http://www.gammon.com.au/forum/?id=11411
int freq1 = 0;
int freq2 = 0;
boolean freq1Entered = false;
boolean freq2Entered = false;

unsigned long led13delay, led12delay;
unsigned long led13timer, led12timer;

void setup(){
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  char readChar = (char)Serial.read();
  while(Serial.available()){
  
  if(!freq1Entered){
  Serial.println("Enter the delay you wish the LED at pin 13 to
  blink at");i
  while(Serial.available()){
    if(readChar=='\r'){
     freq1 = Serial.parseInt();
     Serial.println(freq1);
     led13delay = freq1;
     freq1Entered = true;
     Serial.flush();
     continue;
     }
  }
}
 
  if(freq1Entered && !freq2Entered){
  Serial.println("Enter the delay you wish the LED at pin 12 to blink at");  
  while(Serial.available()){
    if(readChar=='\r'){
     freq2 = Serial.parseInt();
     Serial.println(freq2);
     led12delay = freq2;
     freq2Entered = true;
     continue;
     }
     }
  }

  if(freq1Entered && freq2Entered){
    break;
    Serial.println("Thank you!");
    }
  }
  led13timer = millis();//SET THE CURRENT TIME
  led12timer = millis();
}
  
void loop(){
//CHECKS IF THE APPROPRIATE AMOUNT OF TIME HAS ELAPSED
//FOR A STATE CHANGE IN EITHER LED
  if( (millis() - led13timer) >= led13delay) led13Switch();
  if( (millis() - led12timer) >= led12delay) led12Switch();
}



//TOGGLES THE LEDS ON OR OFF
void led13Switch(){
if (digitalRead(13) == LOW) digitalWrite(13, HIGH);
if (digitalRead(13) == HIGH) digitalWrite(13,LOW);
led13timer = millis();
}
void led12Switch(){
if (digitalRead(12) == LOW) digitalWrite(12, HIGH);
if (digitalRead(12) == HIGH) digitalWrite(12,LOW);
led12timer = millis();
}
