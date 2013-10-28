#include <Arduino.h>

void setup();
void loop();
#line 1 "src/sketch.ino"
// Sends back two digital lines for errors, and needs to detect the stop
// command. If either go high, then error has occurred. If S has been sent, an
// error has occurred. For any error that has occured, an LED will light up

//  Objectives:
//  a. digital pins 2 and 3 both watch for an error
//  b. error from user detected by Serial.read()
//  c. if error is true, print message, light up LED and halt the program
//  d. LIBRARIES ALLOWED, BUT DO NOT USE ATTACHINTERRUPT()

//  As libraries are permitted, we will use them for the error LED
//  error condition

volatile int error0 = 0;
volatile int error1 = 0;

void setup()
{
  //  registers to be set
  SREG &= ~(1<<7);
  DDRB |= (1<<5);
  EIMSK |= ((1<<0)|(1<<1));
  EICRA |= ((1<<ISC00)|(1<<ISC01)|(1<<ISC10)|(1<<ISC11));
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  Serial.begin(9600);
  SREG |= (1<<7);
}

ISR(INT0_vect){
//  data from Pin 2
error0 =1;
}

ISR(INT1_vect){
//  data from Pin 3
error1 =1;
}

void loop()
{   
  // the worst has happened!
  if(Serial.available()){
    if(Serial.read()=='S'){
    Serial.println("Error! User is touching the computer!");
    //  light up an LED
    PORTB ^= (1<<5);
    for(;;){}
    }
  }
  if(error0){
    Serial.println("Error has occurred on Pin2");
    //  light up an LED
    PORTB ^= (1<<5);
    //  halt the program
    for(;;){};
    }
  if(error1){
    Serial.println("Error has occurred on Pin3");
    //  light up an LED
    PORTB ^= (1<<5);
    //  halt the program
    for(;;){};
    }
}
