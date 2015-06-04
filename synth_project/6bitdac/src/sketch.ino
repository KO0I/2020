#include <math.h>
// Voltage step per bit is the resolution of the wave/ 

#define BITRES_4 0.3125
#define BITRES_6 0.0781
#define BITRES_8 0.0195

void setup()
{
  Serial.begin(9600);
  unsigned int pin = 2;
  for(;pin< 8; pin++)
  {
    pinMode(pin,OUTPUT);
  }
  Serial.println("The DDRB register values are:");
  Serial.println(DDRD,BIN);
  delay(1000);
  Serial.println(pow(3,2));
  //PORTB = (unsigned char) 0;

}
void loop()
{
  sinwave(440);
  //volts2bin(2.5);
  
}
void sinwave(int freq)
{
  Serial.println("The delaytime is:");
  int delaytime = (1/(freq*64))*1000000; //Function to calculate time between changing DAC. Not working bc of datatypes?
  Serial.println(delaytime);
  float timescale = 6.28/64; //Trying to update as soon as it reaches threshold of next DAC level.
  //Below is function to populate a table with 6 bit values of a sin wave.
  unsigned char sintable[127];
  for(int i = 0; i < 128; i++)
  {
    sintable[i] = (volts2bin(2.5*sin(freq*i*timescale)+2.5)<<2); //Left shift of 2 because pins 0 and 1 are serial inputs.
    //2.5 for dc shift to get output range of 0-5 rather than -2.5 to 2.5V.
  }
  int w = 0;
  int samplenum = 0;
  while(samplenum<128)
  {
    PORTD = sintable[samplenum];
    delayMicroseconds(35);
    samplenum++;
    if(samplenum == 127)
    {
      samplenum = 0;
    }
    
  }
  Serial.println("Left last while...");
}
unsigned char volts2bin(float voltage) //Function for converting voltage values into 6 bit values to be written to DAC.
{
  float resolution = 0.078125; //Voltage per step in 6 bit DAC
//  int voltstep = (int) (voltage/resolution);
//  Serial.print("The step number is: ");
//  Serial.println(voltstep);
//  Serial.print("This number converted to 6 bit binary is: ");
//  unsigned char DACval = (unsigned char) voltstep;
//  Serial.println(DACval,BIN);
  return DACval;
}
