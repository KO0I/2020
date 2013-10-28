
const int led1 = 12;
const int led2 = 13;
// have we got values for the rates of each LED?
boolean val1 = false;
boolean val2 = false;
boolean values = false;
// rates to be chosen by the user
float freq1 = 1;
float freq2 = 2;
unsigned long led1delay,led2delay;
unsigned long led1timer,led2timer;
//-----------------------------------------------------------------------
void setup(){
pinMode(led1,OUTPUT);
pinMode(led2,OUTPUT);
Serial.begin(9600);
}

void setTime(){
  while(!values && (Serial.available()<=0)){
    //  get the rate for the first LED
    if(!val1 && !val2){
      Serial.println("Please enter a frequency value for the first LED (integer)");
      Serial.flush();
      freq1 = get_num();
      //  Period T is 1/f. But with 50% duty cycle, the signal is on 1/2 the time
      //  Therefore, the delay (in milliseconds) is 1/(2f), where f is frequency
      led1delay=(unsigned long)((1/(2*freq1))*1000);
      //  Debugging - returns period (used to make sure numbers were good
        Serial.print("Period 1 is ");
        Serial.print((2*led1delay));
        Serial.println("[ms]");
      Serial.println((int)freq1);
      val1=true;
      }
    // get the rate for the second LED
    if(val1 && !val2){
      Serial.println("Please enter a frequency value for the second LED (integer)");
      freq2 = get_num();
      led2delay=(unsigned long)((1/(2*freq2))*1000);
      Serial.print("Period 2 is ");
      Serial.print(2*led2delay);
      Serial.println("[ms]");
      Serial.println((unsigned long)freq2);
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


//TOGGLES THE LEDS ON OR OFF
void led1Switch(){
  //Serial.println("herp");
  if (digitalRead(led1) == LOW) digitalWrite(led1, HIGH);
  else if (digitalRead(led1) == HIGH) digitalWrite(led1,LOW);
  led1timer = millis();
}
void led2Switch(){
  //Serial.println("derp");
  if (digitalRead(led2) == LOW) digitalWrite(led2, HIGH);
  else if (digitalRead(led2) == HIGH) digitalWrite(led2,LOW);
  led2timer = millis();
}


void loop(){

  /*
digitalWrite(led1,LOW);
digitalWrite(led2,LOW);
delay((int)((int)(1/freq1)*1000));
digitalWrite(led1,HIGH);
digitalWrite(led2,HIGH);
delay((int)((int)(1/freq1)*1000));
*/
  if( (millis() - led1timer) >= led1delay) led1Switch();
  if( (millis() - led2timer) >= led2delay) led2Switch();
  if((Serial.read()=='r')|!values) {
    val1 = false;
    val2 = false;
    if(values) values = false;
    Serial.flush();
    setTime();
  }
}
