#include <Arduino.h>
void setup();
void loop();
#line 1 "src/sketch.ino"
//define ports for memory-mapped I/O:
#define PORTB ((unsigned char*) 0x25) // char is one byte
#define DDRB  ((unsigned char*) 0x24) // data direction reg
//Pin 13 is on B set of registers

//unsigned int t=100;
//unsigned int timeNow, timePrev=0;

void setup()
{
  //setting PORTB as output
  *DDRB = 0xFF; //B11111111
}

void loop()
{

  //  wait a bit - namely, the max representation of
  //  an unsigned int
  for(unsigned int i=0; i<65534; i++){
  // turn LED on
  *PORTB |= 0x20; //B00100000
  }

  for(unsigned int j=0; j<65534; j++){
  // turn LED off
  *PORTB &= 0x00; //B00000000
  }
}
