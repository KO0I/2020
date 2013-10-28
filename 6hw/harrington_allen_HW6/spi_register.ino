#include <SPI.h>

// set pin 10 as the slave select
#define CS 6
#define DREQ 2

void setup() {
  pinMode (CS, OUTPUT);
  pinMode (DREQ, INPUT);
  Serial.begin(57600);
  SPI.begin(); 
  Serial.println("ENTER AN ADDRESS");
}

void loop() {
  unsigned int recChar;  //Recieve a char
  if (Serial.available())
  {
    recChar = Serial.parseInt();  //Wait for SPI data
    unsigned int readReg = readRegister((unsigned char)recChar);
    Serial.println(readReg, HEX);
  }
}
unsigned int readRegister(unsigned char address)
{
  while(!digitalRead(DREQ));
  digitalWrite(CS,LOW);
  SPI.transfer(0x03); //Read instruction 
  SPI.transfer(address);  //Read a register, this input is buffered
  
  char response1 = SPI.transfer(0xFF); //Read byte, send full byte, MSB
  while(!digitalRead(DREQ));
  char response2 = SPI.transfer(0xFF); //Read byte, send full byte, LSB  
  while(!digitalRead(DREQ));
  digitalWrite(CS,HIGH);
  
  
  int resultvalue = response1 << 8;
  resultvalue |= response2;
  return resultvalue;
}
