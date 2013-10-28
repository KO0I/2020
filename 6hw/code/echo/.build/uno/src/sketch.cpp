#include <Arduino.h>

void setup();
void USART_Init(void);
unsigned char USART_Receive(void);
void USART_Transmit( unsigned char datum );
void loop();
#line 1 "src/sketch.ino"
/*****************************************************************
* Take a character when it is typed in, and repeats it back
* -Does not use Serial Library
* -Uses UCSR0C register for async communication
**A***************************************************************/
unsigned char c = 0;


void setup(){
  USART_Init();
}

/*****************************************************************
*  Code for setting up USART on ATmega328
*****************************************************************/

void USART_Init(void){
  // CLEAR THE REGISTERS
  // The absence of this part crippled our code and poured out 
  // copious amounts of spaghetti on our Serial Monitors.
  // It was a dark time, I tell you...
  UCSR0A = 0;
  UCSR0B = 0;
  UCSR0C = 0;  
  //  Set baud rate for 9600, Clock speed is 16MHz
  UBRR0H = 0;
  // 
  UBRR0L = 207; // can also be written as 0xCF

  //  Set U2X to double the transmission speed
  UCSR0A |= (1<<U2X0);
  //  Enable receiver and transmitter
  UCSR0B |= (1<<TXEN0)|(1<<RXEN0);
  //  Set frame format: asyncronous, no parity, 1 stop bit, 8bit data
  UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
}

/*****************************************************************
*  Code for setting up USART Receive on ATmega328
*****************************************************************/

unsigned char USART_Receive(void){
  while(!(UCSR0A & (1<<RXC0)) );
  //  Get and return received data from the buffer
  return UDR0;
}

/*****************************************************************
*  Code for setting up USART Transmit on ATmega328
*****************************************************************/

void USART_Transmit( unsigned char datum ){
  while ( !(UCSR0A & (1<<UDRE0)) );
  //  place data in buffer
  UDR0 = datum;
}

void loop(){

  c = USART_Receive();
  //  this code grabs a character
  //  this code sends it to the serial monitor
  USART_Transmit(c);
}
