#include <Arduino.h>
#include <avr/io.h> 
int main (void);
#line 1 "src/sketch.ino"
//#include <avr/io.h> 

int main (void) 
{ 

  // Port D5 as output 
  DDRD   |= (1 << 5); 


  // Set on match, clear on TOP 
  TCCR1A  = ((1 << COM1A1) | (1 << COM1A0)); 

  // Phase + Frequency Correct PWM, Fcpu speed 
  TCCR1B  = ((1 << CS10) | (1 << WGM13)); 

  for (;;) 

    //where 0xa is the desired brightness 
    OCR1A=0Xa; 


}
