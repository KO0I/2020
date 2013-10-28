//Written for ECEN 2020 Fall 2010
//University of Colorado Boulder
//R. Pomeroy robert.pomeroy@colorado.edu

//Program:
//Developed with the Arduino Uno Board and Arduino IDE
//Program asks for a first name then lsat name using the serial monitor
//It then prints the names back to the user

char cByte = 0;         // incoming serial byte
char strFirstName[20];
char strLastName[20];

void setup()
{
  // start serial port at 9600 bps:
  Serial.begin(9600);

}

void loop()
{
  int index = 0;
  Serial.println(); //print new line char to make it look nicer
  
  Serial.flush(); //get rid of anything in the buffer (whatever character was hit to start the loop)
  
  Serial.println("Please enter your first name: ");
  while (Serial.available() <= 0); //wait for the user to send something
  delay(100);
  while ((Serial.available() > 0) && (index < 19)) {
      strFirstName[index] = Serial.read(); //loop through the input buffer and add characters to our string
      index++;
  }
  strFirstName[index]=0;
  
  Serial.println("Please enter your last name: ");
  while (Serial.available() <= 0); //wait for the user to send something
  delay(100);
  index = 0;
  while ((Serial.available() > 0) && (index < 19)) {
      strLastName[index] = Serial.read(); // loop through the input buffer for the last name
      index++;
  }
  strLastName[index]=0;
  
  Serial.print("Your name is: ");
  Serial.print(strLastName);
  Serial.print(", ");
  Serial.println(strFirstName);
  Serial.println("Done.");
  
  
}

