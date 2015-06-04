#include <Arduino.h>

void wave2gen(uint8_t len, uint8_t type);
uint16_t mapMIDI(uint16_t note_input);
void setup();
void loop();
#line 1 "src/sketch.ino"
/*****************************************************
  SYNTHESIZER PROJECT - SAM KORN & PATRICK HARRINGTON
 *****************************************************/
#define DACBITS 6
#define LENGTH 64 //length of waveform LUT is 2^DACBITS
#define PI2 6.285 //approximate value for 2*pi
#define AMP LENGTH/2
#define OFFSET LENGTH/2
#define DEF_FREQ 440
#define CLOCK_FREQ 250000 // 16M/64
#define LOWEST_MIDINOTE 24 
// analog inputs for controlling the sound
#define POT_0   (0)
#define POT_1   (1)
#define POT_2   (2)
#define POT_3   (3)
#define POT_4   (4)
#define POT_5   (5)
volatile uint8_t wavemod=LENGTH;
volatile uint8_t index=1;
volatile uint8_t val=0;
volatile uint8_t phase=0;
volatile uint16_t f;
volatile uint16_t frequency=DEF_FREQ;
volatile uint16_t run=0;
//----- MIDI -----
uint8_t command;
uint8_t note;
uint8_t velocity;
uint8_t wave2[LENGTH];

const uint8_t wave[LENGTH] = {
    32, 35, 38, 41, 44, 47, 49, 52, 54, 56, 58, 60, 61, 62, 63, 63, 63, 
    63, 63, 62, 61, 60, 58, 56, 54, 52, 49, 47, 44, 41, 38, 35, 32, 29, 
    26, 23, 20, 17, 15, 12, 10, 8, 6, 4, 3, 2, 1, 1, 0, 1, 1, 2, 3, 4, 6, 
    8, 10, 12, 15, 17, 20, 23, 26, 29,
};
void wave2gen(uint8_t len, uint8_t type){
  if(type==1){
    for(uint8_t i=0; i<len; i++){
    // calculate and store values as integers
      uint8_t voltage = (AMP*sin((PI2/len)*i));
      if(voltage == 32) voltage=31;
      wave2[i] = voltage+OFFSET;
    }
    if (type==2){  //square!
      for(uint8_t i=0; i<len/2; i++){
        wave2[i]=255;
      }
    }
  }
}
// These numbers were calculated using (16000000/64/frequency) where frequency
const uint16_t midiTable[] = {
  //C     C#    D     D#    E     F     F#    G     G#    A     A#    B
    7644, 7214, 6809, 6427, 6066, 5726, 5404, 5101, 4815, 4544, 4289, 4049, //-3
    3821, 3607, 3404, 3213, 3033, 2862, 2702, 2550, 2407, 2271, 2144, 2024, //-2
    1910, 1803, 1702, 1606, 1516, 1431, 1350, 1275, 1203, 1135, 1071, 1010, //-1
    954,  901,  850,  802,  757,  715,  674,  637,  601,  567,  535,  505,  // 0
    476,  450,  424,  400,  378,  357,  336,  317,  300,  283,  267,  252,  //+1
    238,  224,  212,  200,  188,  178,  168,  158,  149,  140,  132,  123,  //+2
};
uint16_t mapMIDI(uint16_t note_input){
  return midiTable[(note_input-LOWEST_MIDINOTE)];
}
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
  //----- MIDI SETUP -----
  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2 = 0;
  OCR2A = 255;
  TCCR2A |= (1<<WGM21);   //CTC mode
  TCCR2B |= (1<<CS11);    //8-bit prescale
  TIMSK2 |= (1<<OCIE2A);
  SREG |= (1<<7);
  DDRD = 0B011111100;  
  PORTD &= ~(0B011111100);
  DDRB  |= 0B00010000;
  PORTB &= ~(0B00010000);
  //  Serial.begin(31250);
  Serial.begin(9600);
  //  wave2gen(LENGTH,2);
}
void loop(){
  //wave[index]=sine[index];
  //phase = analogRead(POT_2)/4;
  // play through notes in midiTable with a one-second delay out
  
  for(uint8_t j=24; j<=95; j++){
    frequency = mapMIDI(j);
    wavemod = analogRead(POT_0)/4;
    //Serial.println(j);
    delay(90);
  }
  /*
  for(uint8_t k=23; k>=12; k--){
    frequency = midiTable[k];
    if(k==13||k==15||k==18||k==20||k==22) k--;
    delay(350);
  }*/
}
  //----- OSCILLATOR ISR -----
  ISR(TIMER1_COMPA_vect){ //if TCNT1 == OCR1A
  // if the index reaches the end of the wavetable, start over
  if((index)>=LENGTH) index=0;
  val = wave[index];
  OCR1A = frequency;
  val<<=2;
  DDRD = wavemod;
  PORTD = val;
  index++;
   //asm("NOP;NOP;NOP;NOP;"); //for the finer tuning
  //TCNT1 = 50; //Compensating for ISR run time
}
  //----- MIDI INPUT ISR -----
  ISR(TIMER2_COMPA_vect) {//checks for incoming midi every 128us
  do{
    if (Serial.available()){
      command = Serial.read();        //read first byte
      OCR1A = mapMIDI(Serial.read()); //read next byte and set it right away
      velocity = Serial.read();       //read final byte
    }
  }
  while (Serial.available() > 2);//when at least three bytes availablei
}
