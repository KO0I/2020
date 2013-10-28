//  Failed attempt to get two LEDs to blink accurately and 
//  independently based on user input. timer2 simply blinks too 
//  fast!
#define LED1 12
#define LED2 13
#define CLOCK 16000000
// have we got values for the rates of each LED?
boolean val1 = false;
boolean val2 = false;
boolean values = false;
// rates to be chosen by the user
//-----------------------------------------------------------------------

void timer1_init(int freq)  // timer1
{
  //  this code is closely related to an example given in class
  SREG &= ~(1 << 7);
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  TCCR1B |= (1 << WGM12);
  TCCR1B |= ((1 << CS12) | (1 << CS10));    // 1024

  // set Output Compare Register 1A to desired number of counts
  OCR1A = CLOCK/1024/freq;   
  // compare match register 16MHz/1024/freq - largely accurate
  // due to being a 16-bit register

  TIMSK1 |= (1 << OCIE1A);

  SREG |= (1 << 7 );
}



void timer2_init(int freq)  // timer2
{
  SREG &= ~(1 << 7);
  //  works -- kind of... the blink is far too quick, even
  //  when the input is 1Hz. This is because of my (poor)
  //  choice in register selection.
  TCCR2A = 0;    
  TCCR2B = 0;
  TCNT2  = 0;   
  
  //  based on a close reading of the datasheet, the best
  //  values I could settle on for the requirements were as follows
  TCCR2A |= (1 << WGM21);  
  TCCR2B |=0x07; //1024 prescale - most aggresive

  //255 is the most I can downclock! It still is not slow enough
  OCR2A = 255/freq;

  TIMSK2 |= (1 << OCIE2A);  

  SREG |= (1 << 7 );   
}



void setup(){
// I attempted to prescale everything, but this mangled
// serial communications badly (couldn't quite get the Baud
// rate to play well with the re-scaling
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
      val1=true;
      }
    // get the rate for the second LED
    if(val1 && !val2){
      Serial.println("Please enter a frequency value for the second LED (integer)");
      int freq2 = get_num();
      Serial.println(freq2);
      timer1_init(freq2);
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
// these work! But... the time scaling of TCCR1A ruins
// everything!

ISR(TIMER1_COMPA_vect) {  
  PORTB ^= (1 << 5); // flip LED1
}

ISR(TIMER2_COMPA_vect) {  
  PORTB ^= (1 << 4); // flip LED2
}

void loop(){}
