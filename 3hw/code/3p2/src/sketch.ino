//define ports for memory-mapped I/O:
#define PORTB ((unsigned char*) 0x25) // char is one byte
#define DDRB  ((unsigned char*) 0x24) // data direction reg
//Pin 13 is on B set of registers

unsigned long t=100;
unsigned long timeNow, timePrev=0;

void setup()
{
  //setting PORTB as output
  *DDRB = 0xFF; //B11111111
}

void loop()
{
unsigned long timeNow = millis();

if(timeNow-timePrev>t){
  timePrev=timeNow;
  // turn LED on
  *PORTB |= 0x20; //B00100000
  }
if(timeNow-timePrev>t){
  timePrev=timeNow;
  // turn LED off
  *PORTB &= 0x00; //B00000000
  }
}
