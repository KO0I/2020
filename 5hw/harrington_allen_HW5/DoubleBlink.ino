
const int led1 = 12;
const int led2 = 13;
// have we got values for the rates of each LED?
boolean val1 = false;
boolean val2 = false;
// rates to be chosen by the user
float freq1 = 10;
float freq2 = 2;
//-----------------------------------------------------------------------
void setup(){
pinMode(led1,OUTPUT);
pinMode(led2,OUTPUT);
Serial.begin(9600);

//  get the rate for the first LED
if(!val1 && !val2){
  Serial.println("Please enter a value for the first LED");
  
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
  
  return atoi(number); 
}


void loop(){
digitalWrite(led1,LOW);
digitalWrite(led2,LOW);
delay((int)((1/freq1)*1000));
digitalWrite(led1,HIGH);
digitalWrite(led2,HIGH);
delay((int)((1/freq1)*1000));
}
