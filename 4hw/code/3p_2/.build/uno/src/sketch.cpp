#include <Arduino.h>
#include <avr/io.h>
void timer1_init(float freq,float duty);
void setup();
void loop();
#line 1 "src/sketch.ino"
/*********
* pwm 
**********/
//#include <avr/io.h>
//  pin 9 is to be used instead of pin 6, because a 16-bit counter's resolution
//  is preferable.
#define LED 13
#define CLOCK_FREQ 16000000
#define PRESCALE 8
#define MAX 65535

//  paramater variables
//volatile float freq;
//volatile float duty;

void timer1_init(float freq,float duty){
//  disable interrupts
SREG &= ~(1 << 7);

//  clear all registers
TCCR1A  = 0;  //  Sets comparisons
TCCR1B  = 0;  //  Clock prescaling, waveform select
ICR1    = 0;  //  TOP value - used to set frequency
OCR1A   = 0;  //  compare value, used to set PWM

DDRB |= (1<<DDB1)|(1<<DDB2);

//  Setting ICR1 (TOP value)
//  equation is freq=(CLOCK_FREQ)/(2*N*TOP)
//  Below, a temporary variable is used to calculate the TOP time
//  using floats and typecast to an integer.

//int TOP = (int) CLOCK_FREQ/(2*PRESCALE*freq);

//  ICR1 MUST be given an integer
//ICR1 = TOP;
ICR1 = 0xFFFF;

//  Setting OCR1A for duty cycle. 
//  The function receives a float value, and then 

//float percent = duty/100; //take input and convert it to decimal
//int ton = (int) percent*MAX;
//OCR1A = ton;
OCR1A = 0xFFFF;

//  non-invert mode. Page 135, Table 15-3
//  set to (1,0) for phase and frequency correct PWM mode.
TCCR1A |= ((1 << COM1A1) | (1 << COM1A1));

//  Set clock to divide signal by 8. See Table 15-5 on p. 137
//TCCR1B |= (1 << CS11);
TCCR1B |= ((0 << CS12)|(1 << CS11)|(1<<CS10));

//  waveform generation mode set to phase-correct PWM
//  from Datasheet, this is Mode 8 from Table 15-4, p. 136
TCCR1B |= (1 << WGM13);

//  enable interrupts
SREG |= (1 << 7);
}

void setup()
{
Serial.begin(9600);
//  set LED to output
DDRB |= (1 << 5);
timer1_init(20,50); //set to 20Hz, 50% duty cycle
}

ISR(TIMER1_COMPA_vect){
//  PORTB ^= (1 << 5);
digitalWrite(LED, digitalRead(LED) ^ 1);
}
void loop()
{
//  PORTB ^= (1 << 5);
}
