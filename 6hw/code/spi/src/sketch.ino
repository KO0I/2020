/*****************************************************************
*  SPI Register reading time
*****************************************************************/
#include <SPI.h>
#define MP3DREQ 2
#define MP3CS 6

//  read any register from 0x00 to 0x0F

void setup(){
  //  As seen in example code
  pinMode(MP3_DREQ, INPUT);
  pinMode(MP3_XCS, OUTPUT);
  Serial.begin(57600);
  Serial.println("Hello, give us a serial number from 0x00 to 0xFF");
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV16);
  rgr = Serial.parsInt();
  while(!digitalRead(MP3DREQ));
    digitalWrite(MP3CS,LOW);

  //  read opCode
  SPI.transfer(0x03);

  SPI.transfer(rgr

}



void loop(){
}
