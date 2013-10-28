//Written for ECEN 2020 Fall 2010
//University of Colorado Boulder
//R. Pomeroy robert.pomeroy@colorado.edu

//Program:
//Developed with the Arduino Uno Board and Arduino IDE
//Blinks the LED on the Arduino Uno board (attached to digital pin 13)


volatile unsigned char * myPORTB  = (volatile unsigned char *) 0x25;
volatile unsigned char * myDDRB = (volatile unsigned char *) 0x24;


void setup()   {                
  *myDDRB = 0x20;
}


void loop()                    
{  
  const unsigned int MAX_COUNT = 65535; //10000 -> 44Hz, 65535 -> 6.7Hz (2^16-1 for the largest 16 bit unsigned int)
  volatile unsigned int i;
  
  
  *myPORTB = 0x20; //on
  for (i = 0; i < MAX_COUNT; i++);
  
  *myPORTB = 0x00;       // off
  for (i = 0; i < MAX_COUNT; i++);
}