//Currently an interrupt is flagged at 40 khz where the ports
//will be written to inside the isr.
//Inside the main loop function the math for the waveform will be calculated
//And placed into an array, acting as a buffer.
//When nobs are toggled the array will be updated with the newer waveform a few
//Samples in the future to act as a buffer for the calculations.
//Code in active development for the next hour or two.
#include "math.h"

#define CPUfreq  16000000 //16 mHz
#define SAMPLEfreq 40000  //40 kHz to get nyquist frequency of 20 khz max
#define NUMSAMPLES 10000

volatile unsigned long sampletime = 0;
static unsigned char samples[9999];

void setup()
{
  Serial.begin(9600);
  cli(); //Disable interrupts
  TCCR1A = 0;   //T1 Control Register A: Set all to zero
  TCCR1B = 0;   //T1 Control Register B: Set all to zero
  TCNT1 = 0;    //Timer 1: Initialize counter to zero.
  OCR1A = 399;  //Compare Register A:CPUfreq/(1*SAMPLEFreq)) - 1. Flag every 40 kHz
  TCCR1B |= (1 << WGM12); //Set counter to CTC mode
  TCCR1B |= (1 << CS10); //Set the prescaler to 1 so counter is incrementing at 16 mHz
  TIMSK1 |= (1 << OCIE1A); //Turn on interrupt on Compare Match
  
  sei(); //Enable interrupts
}
void loop()
{
 
 calculateWave(440,samples);
}
ISR(TIMER1_COMPA_vect)
{
  if(sampletime == 9999)
    sampletime = 0;
  else
    sampletime++;
   PORTD = samples[sampletime];
}
void calculateWave(int freq, unsigned char samples[] )
{
  int i;
  for(i = 0; i <= 9999; i++)
  {
    samples[i] = (volts2bin(2.5*sin(freq*i)+2.5) << 2);
  }
}
unsigned char volts2bin(float voltage) //Function for converting voltage values into 6 bit values to be written to DAC.
{
  float resolution = 0.078125; //Voltage per step in 6 bit DAC
  int voltstep = (int) (voltage/resolution);
  Serial.print("The step number is: ");
  Serial.println(voltstep);
  Serial.print("This number converted to 6 bit binary is: ");
  unsigned char DACval = (unsigned char) voltstep;
  Serial.println(DACval,BIN);
  return DACval;
}
