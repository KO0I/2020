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
#define NOTEON  144
#define NOTEOFF 128
// analog inputs for controlling the sound
#define POT_0   (0)
#define POT_1   (1)
#define POT_2   (2)
#define POT_3   (3)
#define POT_4   (4)
#define POT_5   (5)
volatile uint8_t waveform=0;
volatile uint8_t waveform2=0;
volatile uint8_t wavemod=LENGTH;
volatile uint8_t index=1;
volatile uint8_t index2=1;
volatile uint8_t val=0;
volatile uint8_t val2=0;
volatile uint8_t phase2=0;
volatile uint8_t amp2=0;
volatile uint16_t f;
volatile uint16_t frequency=DEF_FREQ;
volatile uint16_t frequency2=DEF_FREQ;
volatile uint16_t run=0;
//----- MIDI -----
volatile uint8_t incomingByte;
volatile uint8_t action=2; //0 is note off, 1 is note on
volatile uint8_t command;
volatile uint8_t note;
volatile uint8_t velocity;
//uint8_t wave2[LENGTH];
uint8_t selector=0;
uint8_t wave[4][LENGTH]={
  //sine=0
    {
    32, 35, 38, 41, 44, 47, 49, 52, 54, 56, 58, 60, 61, 62, 63, 63, 63, 
    63, 63, 62, 61, 60, 58, 56, 54, 52, 49, 47, 44, 41, 38, 35, 32, 29, 
    26, 23, 20, 17, 15, 12, 10, 8, 6, 4, 3, 2, 1, 1, 0, 1, 1, 2, 3, 4, 6, 
    8, 10, 12, 15, 17, 20, 23, 26, 29,
    //second cycle for phase shift
    32, 35, 38, 41, 44, 47, 49, 52, 54, 56, 58, 60, 61, 62, 63, 63, 63, 
    63, 63, 62, 61, 60, 58, 56, 54, 52, 49, 47, 44, 41, 38, 35, 32, 29, 
    26, 23, 20, 17, 15, 12, 10, 8, 6, 4, 3, 2, 1, 1, 0, 1, 1, 2, 3, 4, 6, 
    8, 10, 12, 15, 17, 20, 23, 26, 29,
    },
  //ramp=1
    { 
    0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,
    0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63},
  //triangle=2
    { 
    0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,62,62,60,58,56,54,52,50,48,46,44,42,40,38,36,34,32,30,28,26,24,22,20,18,16,14,12,10,8,6,4,2,0,
    0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,62,62,60,58,56,54,52,50,48,46,44,42,40,38,36,34,32,30,28,26,24,22,20,18,16,14,12,10,8,6,4,2,0},
  //square=3
    {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63}
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
  //----- OSC2 SETUP -----
  
  TCCR2A = 0;             // low when TCNT1=OCR1A
  TCCR2B = 0;
  TCCR2B  = (1<<CS10);    // no pre-scale - 16MHz clock
  TCNT2  = 0;             // init counter
  TCCR2B |= (1<<WGM12);   // ctc mode
  TIMSK2 = (1<<OCIE2A);   // Call ISR on match
  OCR2A  = CLOCK_FREQ/frequency2; // Period for wavetable
  
  //----- MIDI SETUP -----
  //TCCR2A = 0;
  //TCCR2B = 0;
  //TCNT2 = 0;
  //OCR2A = 255;
  //TCCR2A |= (1<<WGM21);   //CTC mode
  //TCCR2B |= (1<<CS11);    //8-bit prescale
  //TIMSK2 |= (1<<OCIE2A);
  DDRB |= (1<<5);
  PORTB &= ~(1<<5);
  //PORTB |= (1<<5);
  //----- OTHER -----
  SREG |= (1<<7);
    Serial.begin(31250);
  //Serial.begin(9600);
  //  wave2gen(LENGTH,2);
}
void loop(){
  wavemod    =  (analogRead(POT_0)>>2);
  waveform   =  (analogRead(POT_1)>>8);
  waveform2  =  (analogRead(POT_2)>>8);
  checkMIDI();
  frequency  =  mapMIDI(note); 
  // if shift by 3, we can shift the phase from 0 to 180
  // if shift by 4, we can shift the phase from 0 to 90 (higher precision)
  phase2     =  (analogRead(POT_3)>>5);
  amp2     =  (analogRead(POT_4)>>5);
  //Serial.println(phase);
  /*
  for(uint8_t j=24; j<=95; j++){
    frequency = mapMIDI(j);
    wavemod = analogRead(POT_0)/4;
    //Serial.println(j);
    delay(90);
  }*/
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
  if(index==(LENGTH>>1)) index=0;
  
  //--- crude normalization ---
  val = ((wave[waveform][index])>>1)+val2;
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
   //asm("NOP;NOP;NOP;NOP;"); //for the finer tuning
  //TCNT1 = 50; //Compensating for ISR run time
}
  //----- OSCILLATOR2 ISR -----
  ISR(TIMER2_COMPA_vect){
  if((index2)>=(LENGTH>>2)) index2=0; 
  val2 = (wave[waveform2][(index+phase2)]>>1)*amp2;
  index2++;
  }
  
  void checkMIDI(){
      incomingByte=Serial.read();
      if(incomingByte != 0) {
      }
   /* 
    if (incomingByte==NOTEON) {
      action=1;
    }
    else if (incomingByte==NOTEOFF) {
      action=0;
      PORTB |= (1<<5);
    }
  */  
  }
