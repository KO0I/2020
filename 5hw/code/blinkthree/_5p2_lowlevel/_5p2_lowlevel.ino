
#define LED1 12
#define LED2 13
#define CLOCK 16000000
// have we got values for the rates of each LED?
boolean val1 = false;
boolean val2 = false;
boolean values = false;
// rates to be chosen by the user
float freq1 = 1;
float freq2 = 2;
unsigned long led1delay,led2delay;
unsigned long led1timer,led2timer;
//-----------------------------------------------------------------------

void timer1_init(int freq)  // initialize timer1
{
  // noInterrupts();    // disable all interrupts
  SREG &= ~(1 << 7);    // disable all interrupts

  TCCR1A = 0;    // clear Timer/Counter 1 Control Register
  TCCR1B = 0;
  TCNT1  = 0;    // clear Timer/Counter 1

  // set modes in Timer/Counter 1 Control Register
  // TCCR1A remains all zeros
  TCCR1B |= (1 << WGM12);  // Clear Timer on Compare (CTC) mode
  TCCR1B |= ((1 << CS12) | (1 << CS10));    // 1024 prescaler

  // set Output Compare Register 1A to desired number of counts
  OCR1A = CLOCK/1024/freq;    // compare match register 16MHz/1024/2Hz

  // set Output Compare Interrupt Enable 1A bit in Timer Interrupt Mask 1
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt

  // interrupts();      // enable all interrupts 
  SREG |= (1 << 7 );    // enable all interrupts
}



void timer2_init(int freq)  // initialize timer2
{
  // noInterrupts();    // disable all interrupts
  SREG &= ~(1 << 7);    // disable all interrupts

  //TCCR2A = 0;    // clear Timer/Counter 2 Control Register
  //TCCR2B = 0;
  TCNT2  = 0;    // clear Timer/Counter 2

  // set modes in Timer/Counter 1 Control Register
  // TCCR1A remains all zeros
  TCCR2A |= (1 << WGM21);  // Clear Timer on Compare (CTC) mode
  //TCCR2B |= (1 << CS22);    // 
  TCCR2B |=0x07;

  // set Output Compare Register 1A to desired number of counts
  OCR2A = 255/freq;    // compare match register 16MHz/1024/2Hz

  // set Output Compare Interrupt Enable 1A bit in Timer Interrupt Mask 1
  TIMSK2 |= (1 << OCIE2A);  // enable timer compare interrupt

  // interrupts();      // enable all interrupts 
  SREG |= (1 << 7 );    // enable all interrupts
}



void setup(){
// CLKPR = (1<<CLKPCE);
// CLKPR = B00000001; 
DDRB |= (1<<5);
DDRB |= (1<<4);
Serial.begin(9600);

while(!values && (Serial.available()<=0)){
    //  get the rate for the first LED
    if(!val1 && !val2){
      Serial.println("Please enter a frequency value for the first LED (integer)");
      Serial.flush();
      int freq1 = get_num();
      Serial.println(freq1);
      timer1_init(freq1);
      delay(100);
      timer2_init(freq1);
      val1=true;
      }
    // get the rate for the second LED
    /*
    if(val1 && !val2){
      Serial.println("Please enter a frequency value for the second LED (integer)");
      freq2 = get_num();
      led2delay=(unsigned long)((1/(2*freq2))*1000);
      Serial.print("Period 2 is ");
      Serial.print(2*led2delay);
      Serial.println("[ms]");
      Serial.println((unsigned long)freq2);
      val2=true;
      }
      if(val1 && val2){
        Serial.print("LED1 blinks at ");
        Serial.print((int)freq1);
        Serial.print(" and LED2 blinks at ");
        Serial.print((int)freq2);
        Serial.println(" [Hz]");
        values = true;
      }
   */   
  }
}



int get_num() { //  Get a number from the user
  char number[20];
  int index = 0;
  while(Serial.available() <= 0); 
  delay(50);
  while(Serial.available() > 0) {
     number[index] = Serial.read();
     index++;
  }
  number[index] = 0;
  Serial.flush();
  return atoi(number); 
}

// ISRs for flipping the LED states

ISR(TIMER1_COMPA_vect) {  
  PORTB ^= (1 << 5); // flip LED1
}

ISR(TIMER2_COMPA_vect) {  
  PORTB ^= (1 << 4); // flip LED2
  TCNT2=0;
}

void loop(){}
