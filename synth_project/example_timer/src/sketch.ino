#define CLOCK_FREQ 16000000
#define FREQUENCY 440

void timer1_init(uint16_t freq){
  SREG &= ~(1<<7);
  OCR1A = CLOCK_FREQ/1024/freq;
  SREG |= (1<<7);
}

void setup(){
  Serial.begin(9600);
  SREG &= ~(1<<7);
  TCCR1A=0;
  TCCR1B=0;
  TCNT1=0;
  TCCR1B |= (1<<WGM12);
  //TCCR1B |= ((1<<CS12)|(1<<CS10));
  TCCR1B |= (1<<CS12);
  //TCCR1B |= (1<<CS10);
  SREG |= (1<<7);
  DDRB |= (1<<5);
  TIMSK1 |= (1<<OCIE1A);
}

ISR(TIMER1_COMPA_vect){
  PORTB ^= (1<<5);
}

void loop(){
  timer1_init(FREQUENCY/2);
}
