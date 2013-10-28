#include <Arduino.h>

void timer1_init(float freq,float duty);
void setup();
void loop();
int get_line();
#line 1 "src/sketch.ino"
/*********
 * pwm 
 **********/
//  pin 9 is to be used instead of pin 6, because a 16-bit counter's resolution
//  is preferable.
#define CLOCK_FREQ 16000000
#define PRESCALE 8
#define MAX 65535

//  paramater variables
volatile int freq=20;
volatile int duty=70;

void timer1_init(float freq,float duty){
  //  disable interrupts
  SREG &= ~(1 << 7);

  //  clear all registers
  TCCR1A  = 0;  //  Sets comparisons
  TCCR1B  = 0;  //  Clock prescaling, waveform select
  ICR1    = 0;  //  TOP value - used to set frequency
  OCR1A   = 0;  //  compare value, used to set PWM


//---------------------------------------------------------------------
//    Setting Frequency, Duty Cycle
//---------------------------------------------------------------------
  //  Setting ICR1 (TOP value)
  //  equation is freq=(CLOCK_FREQ)/(2*N*TOP)
  //  Below, a temporary variable is used to calculate the TOP time
  //  using floats and typecast to an integer.

  int TOP = CLOCK_FREQ/(2*PRESCALE*freq);

  //  ICR1 MUST be given an integer
  ICR1 = TOP;
//  ICR1 = 0xCFFF;
  //  Setting OCR1A for duty cycle. 
  //  The function receives a float value, and then 

  float percent = ((float)duty)/100; //take input and convert it to decimal
  int ton = (int) percent*MAX;
  OCR1A = ton;
//  OCR1A = 0xB3AC;

//---------------------------------------------------------------------
//    Setting Mode
//---------------------------------------------------------------------
  //  waveform generation mode set to phase-correct PWM
  //  from Datasheet, this is Mode 8 from Table 15-4, p. 136
  //TCCR1A |= (1 << WGM10);
  TCCR1B |= (1 << WGM13);

  //  non-invert mode. Page 135, Table 15-3
  //  set to (1,0) for phase and frequency correct PWM mode.
  TCCR1A |= ((1 << COM1A1) | (0 << COM1A0));
  
  // Enable the Timer/Counter1 Interrupt Mask Register p.139
  TIMSK1 |= (1<<OCIE1A);

  //  Set clock to divide signal by 8. See Table 15-5 on p. 137
  //TCCR1B |= (1 << CS11);
  TCCR1B |= ((0 << CS12)|(1 << CS11)|(1<<CS10));


  //  enable interrupts
  SREG |= (1 << 7);
}

void setup(){
  Serial.begin(9600);
  //  set LED to output
  DDRB |= (1 << 5);
  timer1_init(freq,duty); //set to 20Hz, 50% duty cycle
}

ISR(TIMER1_COMPA_vect){
  PORTB ^= (1 << 5);
  //digitalWrite(LED, digitalRead(LED) ^ 1);
}

void loop(){
  delay(1500);
  Serial.println("Enter Duty Cycle (0-100%)");
  delay(10);
  duty = get_line();
  Serial.println(duty);
  timer1_init(20,duty);
  for(;;){}
  }


int get_line() {
  char number[20];
  int index = 0;
  Serial.flush();
  char readChar = (char)Serial.read();

  while(Serial.available() <= 0);
  delay(100);
  while(Serial.available() > 0) {
    if(readChar=='\r'){
      number[index]=0;
      continue;
    }
    number[index] = Serial.read();
    index++;
  }
  number[index] = 0;
  return atoi(number);
}

