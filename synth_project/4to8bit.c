#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>

const char *byte_to_binary(int x){ //function found on the internet
  static char b[9];
  b[0] = '\0';
  int z;
  for (z = 128; z > 0; z >>= 1)
  {
    strcat(b, ((x & z) == z) ? "1" : "0");
  }
  return b;
}

int main(void){
  uint8_t i,j;
  // given a random 4bit value (0-15)
  uint8_t testbyte = 0B00001111;
  //if 0B00001111, convert it into 0B01010101
  for(i=0; i<=15; i++){
    testbyte=i;
    printf("%d or %s converts to ",testbyte,byte_to_binary(testbyte));
    if(testbyte & (1<<3)) testbyte |= (1<<6);
    if(testbyte & (1<<2)) testbyte |= (1<<4);
    if(testbyte & (1<<1)) testbyte |= (1<<2);
    if(testbyte & (1<<0)) testbyte |= (1<<0);
//    for(j=1;j<7;j+=2){
//    testbyte &= ~(1<<j);
//      }
    testbyte ^= ~(0xAA);
    printf("%d or %s\n",testbyte,byte_to_binary(testbyte));
  }
  return 0;
}
