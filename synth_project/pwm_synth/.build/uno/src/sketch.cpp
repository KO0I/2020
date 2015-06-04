#include <Arduino.h>
#include <avr/interrupt.h>
void setup();
void loop();
void waveform(byte w);
#line 1 "src/sketch.ino"
//#include <avr/interrupt.h>

#define PI2     6.283185 // 2 * PI - saves calculating it later
#define AMP     127      // Multiplication factor for the sine wave
#define OFFSET  128      // Offset shifts wave to just positive values

#define LENGTH  256  // The length of the waveform lookup table
uint8_t wave[LENGTH];   // Storage for the waveform
volatile uint8_t period=64;
uint8_t select=0;
//uint16_t osc1freq;
//uint16_t osc1phase;

#define SINE     0
#define RAMP     1
#define TRIANGLE 2
#define SQUARE   3
#define RANDOM   4

// Other waveform parameters
#define OSC1_FREQ       (4)
#define OSC1_PHASE      (3)

// Oscillator Stuff
#define PWM_VALUE OCR1AL

void setup() {
  Serial.begin(9600);

  /******** Populate the waveform lookup table with a wave ********/
  for (int i=0; i<LENGTH; i++) {
    uint8_t v = (AMP*sin((PI2/LENGTH)*i));  // Calculate current entry
    v += (AMP/4*sin((PI2/LENGTH)*(i*3))); // New step
    v += (AMP/4*sin((PI2/LENGTH)*(i*5))); // New step
    v += (AMP/4*sin((PI2/LENGTH)*(i*7))); // New step
    wave[i] = int(v+OFFSET);                // Store value as integer
    waveform(TRIANGLE);                     // Replace sine with the different cases to
                                        // Produce the different waves
  }
 
  /******** Set timer1 for 8-bit fast PWM output ********/
  pinMode(9, OUTPUT);       // Make timer's PWM pin an output
  TCCR1B  = (1 << CS10);    // Set prescaler to full 16MHz
  TCCR1A |= (1 << COM1A1);  // PWM pin to go low when TCNT1=OCR1A
  TCCR1A |= (1 << WGM10);   // Put timer into 8-bit fast PWM mode
  TCCR1B |= (1 << WGM12); 

  /******** Set up timer 2 to call ISR ********/
  TCCR2A = 0;               // We need no options in control register A
  TCCR2B = (1 << CS21);     // Set prescaller to divide by 8
  TIMSK2 = (1 << OCIE2A);   // Set timer to call ISR when TCNT2 = OCRA2
  sei();                    // Enable interrupts to generate waveform!
}

void loop() {
//select=0;
//if(select>4) select=0;
waveform(0);
OCR2A = period;               
//for(int j=0; j<LENGTH; j++){
//  Serial.print(" ");
//  Serial.print(wave[j]);
//}
// sets the frequency of the generated wave
//sweeps through all waveforms and their frequencies

//for (int j = 256; j>10; j--){
//  freq=j;
//  Serial.print("freq: ");
//  Serial.println(freq);
//  delay(50);
//  }

//delay(500);
//select++;

}

ISR(TIMER2_COMPA_vect) {  // Called each time TCNT2 == OCR2A
  static byte index=0;    // Points to successive entries in the wavetable
  uint8_t value;
  uint16_t output;
  output = wave[index++]; 
  PWM_VALUE = output;
  asm("NOP;NOP");         // Fine tuning
  TCNT2 = 6;              // Timing to compensate for time spent in ISR
}


void waveform(byte w) {
 switch(w) {

   case SINE: 
    for (uint16_t i=0; i<LENGTH; i++) 
      {uint8_t v = OFFSET+(AMP*sin((PI2/LENGTH)*i));
      wave[i]=byte(v);
    }
    break;

  case RAMP:
    for (int i=0; i<LENGTH; i++) {
      wave[i]=i;
    }
    break;

  case TRIANGLE:
    for (int i=0; i<LENGTH; i++) {
      if (i<(LENGTH/2)) {
        wave[i]=i*2;
      } else {
        wave[i]=(LENGTH-1)-(i*2);
      }
    }
    break;
    
  case SQUARE:
    for (int i=0; i<(LENGTH/2); i++) {
      wave[i]=255;
    }
    break;

  case RANDOM:
    randomSeed(2);
    for (int i=0; i<LENGTH; i++) {
      wave[i]=random(256);
    }
      break;
  }
}

