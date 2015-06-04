#include <Arduino.h>
#include <MIDI.h>
uint16_t mapMIDI(byte note_input);
void HandleNote(byte channel, byte pitch, byte velocity);
void setup();
void loop();
#line 1 "src/sketch.ino"
#define USEMIDI 1
#ifdef USEMIDI
//#include <MIDI.h>
#endif
/*****************************************************
  SYNTHESIZER PROJECT - SAM KORN & PATRICK HARRINGTON
 *****************************************************/
#define DACBITS 6
#define LENGTH 128 //length of waveform LUT is (2^DACBITS)*2
#define PI2 6.285 //approximate value for 2*pi
#define AMP 32
#define OFFSET 32
#define DEF_FREQ 954
//#define DEF_FREQ 7644
#define CLOCK_FREQ 250000 // 16M/64
#define LOWEST_MIDINOTE 24
#define HIGHEST_MIDINOTE 96
// analog inputs for controlling the sound
#define POT_0   (0)
#define POT_1   (1)
#define POT_2   (2)
#define POT_3   (3)
#define POT_4   (4)
#define POT_5   (5)
//----- Oscillator Parameters -----
volatile uint8_t waveform=0;
volatile uint8_t waveform2=0;
volatile uint8_t wavemod=LENGTH;
volatile uint8_t decayTime=0;
volatile uint8_t index=1;
volatile uint8_t index2=1;
volatile uint8_t val=0;
volatile uint8_t val2=0;
volatile uint8_t phase2=0;
volatile uint8_t amp2=0;
//volatile uint16_t f;
volatile uint16_t frequency=DEF_FREQ;
volatile uint16_t frequency2=DEF_FREQ;
volatile uint8_t cur_freq;
volatile uint8_t arp=1;
volatile uint8_t note=60;
volatile uint8_t decay=0;
volatile uint8_t oscamp=1;
//volatile uint16_t run=0;
//-- Debug LEDs---
#define debug1 11;
#define debug2 12;
#define debug3 13;
//----- MIDI -----
byte MIDInote;
byte MIDIvelocity;
volatile uint8_t MIDInoteon = 0;
//---- STORED WAVES -----
uint8_t wave[4][LENGTH]={
  //sine=0
    {16, 18, 19, 20, 22, 24, 24, 26, 27, 28, 29, 30, 30, 31, 32, 32, 32, 
      32, 32, 31, 30, 30, 29, 28, 27, 26, 24, 24, 22, 20, 19, 18, 16, 14, 
        13, 12, 10, 8, 8, 6, 5, 4, 3, 2, 2, 1, 0, 0, 0, 0, 0, 1, 2, 2, 3, 4, 
        5, 6, 8, 8, 10, 12, 13, 14, 16, 18, 19, 20, 22, 24, 24, 26, 27, 28, 
        29, 30, 30, 31, 32, 32, 32, 32, 32, 31, 30, 30, 29, 28, 27, 26, 24, 
        24, 22, 20, 19, 18, 16, 14, 13, 12, 10, 8, 8, 6, 5, 4, 3, 2, 2, 1, 0, 
        0, 0, 0, 0, 1, 2, 2, 3, 4, 5, 6, 8, 8, 10, 12, 13, 14

    //second cycle for phase shift
    },
  //ramp=1
    {
     0, 0, 1, 2, 2, 2, 3, 4, 4, 4, 5, 6, 6, 6, 7, 8, 8, 8, 9, 10, 10, 10,
      11, 12, 12, 12, 13, 14, 14, 14, 15, 16, 16, 16, 17, 18, 18, 18, 19, 
     20, 20, 20, 21, 22, 22, 22, 23, 24, 24, 24, 25, 26, 26, 26, 27, 28, 
    28, 28, 29, 30, 30, 30, 31, 32, 0, 0, 1, 2, 2, 2, 3, 4, 4, 4, 5, 6, 
   6, 6, 7, 8, 8, 8, 9, 10, 10, 10, 11, 12, 12, 12, 13, 14, 14, 14, 15, 
  16, 16, 16, 17, 18, 18, 18, 19, 20, 20, 20, 21, 22, 22, 22, 23, 24, 
  24, 24, 25, 26, 26, 26, 27, 28, 28, 28, 29, 30, 30, 30, 31, 32}, 
  //triangle=2
    { 0, 0, 1, 2, 2, 2, 3, 4, 4, 4, 5, 6, 6, 6, 7, 8, 8, 8, 9, 10, 10, 10, 
      11, 12, 12, 12, 13, 14, 14, 14, 15, 16, 16, 16, 17, 18, 18, 18, 19, 
        20, 20, 20, 21, 22, 22, 22, 23, 24, 24, 24, 25, 26, 26, 26, 27, 28, 
        28, 28, 29, 30, 30, 30, 31, 32, 0, 0, 1, 2, 2, 2, 3, 4, 4, 4, 5, 6, 
        6, 6, 7, 8, 8, 8, 9, 10, 10, 10, 11, 12, 12, 12, 13, 14, 14, 14, 15, 
        16, 16, 16, 17, 18, 18, 18, 19, 20, 20, 20, 21, 22, 22, 22, 23, 24, 
        24, 24, 25, 26, 26, 26, 27, 28, 28, 28, 29, 30, 30, 30, 31, 32},
  //square=3
    {
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 
        32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 
        32, 32, 32, 32,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 
        32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 
        32, 32, 32, 32}
    };

  // These numbers were calculated using (16000000/64/frequency) where frequency
  const uint16_t midiTable[] = {
  //C     C#    D     D#    E     F     F#    G     G#    A     A#    B
    7644, 7214, 6809, 6427, 6066, 5726, 5404, 5101, 4815, 4544, 4289, 4049, //-3
    3821, 3607, 3404, 3213, 3033, 2862, 2702, 2550, 2407, 2271, 2144, 2024, //-2
    1910, 1803, 1702, 1606, 1516, 1431, 1350, 1275, 1203, 1135, 1071, 1010, //-1
    954,  901,  850,  802,  757,  715,  674,  637,  601,  567,  535,  505,  // 0
    476,  450,  424,  400,  378,  357,  336,  317,  300,  283,  267,  252,  //+1
    238,  224,  212,  200,  188,  178,  168,  158,  149,  140,  132,  123,  //+2
    119 //C7 Final note of my piano                                         //+3                                     
  };
  uint16_t mapMIDI(byte note_input){
  return midiTable[(note_input-LOWEST_MIDINOTE)];
}

void HandleNote(byte channel, byte pitch, byte velocity){
  MIDInote = pitch;
  note = pitch;
  MIDIvelocity = velocity;
  if(velocity) { //If Velocity == 0 then it is a note off
      if(pitch > LOWEST_MIDINOTE && pitch < HIGHEST_MIDINOTE) { //Range C1 to C7
        MIDInoteon = 1;
        frequency = mapMIDI(pitch);
        cur_freq = pitch;
      }
   }
   else {
     //delayMicroseconds(1); 
     MIDInoteon = 0;
     Serial.flush();
  }
}
/*
void HandlePitchBend(byte channel, int bend){
  volatile uint8_t ppitchbend;
  volatile uint8_t npitchbend;
  if(bend<64){
  npitchbend=0;
  ppitchbend=64-bend;
  }
  else if(bend>64){
  ppitchbend=0;
  npitchbend=bend-64;
  }
  else{
  ppitchbend=0;
  npitchbend=0;
  }
}  
*/
void setup(){
  SREG &= ~(1<<7);
  //  PORTD: Pins 2 to 8
  //  PORTB: Pins 9 to 13
  //----- OSC1 SETUP -----
  TCCR1A = 0;             // low when TCNT1=OCR1A
  TCCR1B = 0;
  TCCR1B  = (1<<CS10);    // no pre-scale - 16MHz clock
  TCNT1  = 0;             // init counter
  TCCR1B |= (1<<WGM12);   // ctc mode
  TIMSK1 = (1<<OCIE1A);   // Call ISR on match
  OCR1A  = CLOCK_FREQ/frequency; // Period for wavetable
  //----- OSC2 SETUP -----
  
  TCCR2A = 0;             // low when TCNT1=OCR1A
  TCCR2B = 0;
  TCCR2B  = (1<<CS10);    // no pre-scale - 16MHz clock
  TCNT2  = 0;             // init counter
  TCCR2B |= (1<<WGM12);   // ctc mode
  TIMSK2 = (1<<OCIE2A);   // Call ISR on match
  OCR2A  = CLOCK_FREQ/frequency2; // Period for wavetable
  
  //----- Decay Setup -----
  TCCR0A =0;
  TCCR0A =0;
  TCCR0A = ((1<<CS12)|(1<<CS10));
  TCNT0  = 0;
  TCCR0B |=(1<<WGM12);
  TIMSK0 = (1<<OCIE0A);
  OCR0A  = CLOCK_FREQ/16/2;

  //----- Enable interupts, Debug LEDS

  SREG |= (1<<7);
  DDRD = 0B011111100;  
  PORTD &= ~(0B011111100);
  DDRB  |= (1<<5);
  PORTB &= ~(1<<5);
  //---- DEBUG LEDS ----
  //DDRB = 0xB111000; //Debug pins on 11, 12, and 13

  //----- MIDI SETUP -----
#ifndef USEMIDI
  Serial.begin(9600); //Can't have this with MIDI
  MIDInoteon=0;
#endif
#ifdef USEMIDI
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.setHandleNoteOn(HandleNote);
//  MIDI.setHandlePitchBend(HandlePitchBend);
  //PORTB |= 0B110000;
#endif
  //  wave2gen(LENGTH,2);
}

void loop(){
  MIDI.read();
  wavemod    =  (analogRead(POT_0)>>2);
  waveform   =  (analogRead(POT_1)>>8);
  waveform2  =  (analogRead(POT_2)>>8);
  phase2     =  (analogRead(POT_3)>>5);
  amp2       =  (analogRead(POT_4)>>5);
  decayTime  =  (analogRead(POT_5)>>2);

  //if(decay=32) 
  //decay=31; 
  //else{
  //  decay++;
  //}
  //decay=decay+0.1;
}

//----- OSCILLATOR ISR -----
ISR(TIMER1_COMPA_vect){ //if TCNT1 == OCR1A
  // if the index reaches the end of the wavetable, start over
  if(index==(LENGTH>>1)) {index=0;}
  //if(!MIDInoteon){
  if(MIDInoteon) oscamp=1;
    val = (((wave[waveform][index]+wave[waveform][index+decay])>>1)+val2)*oscamp;
  //}
  //else{ 
  //val = (((wave[waveform][index])>>1)+val2);
  //}
  //--- crude normalization ---
  if(val>63)val=63;
  
  //--- "graceful" normalization ---
  //val = wave[waveform][(index+phase)];
  //val = (val+val2 - ((val*val2)/AMP));
  // debugging
  //PORTB ^= (1<<5);
  OCR1A = frequency;
  val<<=2;
  DDRD = wavemod;
  PORTD = val;
  index++;
  }

//----- OSCILLATOR2 ISR -----
ISR(TIMER2_COMPA_vect){
  if((index2)>=(LENGTH>>2)) index2=0; 
  //
  val2 = ((wave[waveform2][(index+phase2)]+wave[waveform2][(index+phase2+amp2)]>>1)*amp2)*oscamp;
  index2++;
}
//----- DECAY OSC -----
uint16_t divider=0;
ISR(TIMER0_COMPA_vect){
  if(!MIDInoteon){
    if(divider<decayTime){ 
      divider++;
      PORTB &= ~(1<<5);
    }
    else{ 
      divider=0; 
      if(decay<33)decay++;
      else{ decay=33;
        oscamp=0;}
      PORTB |= (1<<5);
    }
  }else decay=0;
}

