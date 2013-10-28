/******************************************************************
* Analog Reading
******************************************************************/

//  Global Variables

const int analogPin = 5;
volatile int value  = 0;    //  A value from 0 - 1023
volatile float voltage = 0; //  A voltage

// R1 = R2 = 12 kOhm
// Expected output voltage is R2/(R1+R2)5=60.6 mV
// Measured before plugging everything in was 60.9 mV

void setup()
{
  //  We'll need Serial to see the voltage
  Serial.begin(9600);
}

void loop()
{
  //  Voltage divider at analog pin 5
  value = analogRead(A5);
  //  analogRead returns values from 0-1023
  voltage = value*(5/1023.0);
  Serial.print(voltage);
  Serial.println("[V]");
  //  output was 0.05 V for R2 to ground and R1 to node
  //  If reversed; 4.95 V was measured.
  delay(2000);
}
