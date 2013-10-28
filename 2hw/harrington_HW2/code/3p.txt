
#define LED_PIN 13
// A nice, default period to get things started.
unsigned long period = 1000;
// calls input as a string
String periodChar = "";
boolean gotTime  = false;

void setup()
{
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  Serial.println("Enter time in milliseconds, and hit Return: ");
}

unsigned long readInput()
{
  unsigned long a=0;
  boolean done = false;
  while(Serial.available() && done==false)
  {
    // Reads in a string from Serial
    char readPeriod = (char)Serial.read();
    // check if Return key has been hit.
    if(readPeriod == '\r'){
      // if so, then break the loop 
      done = true;
    }
    periodChar += readPeriod;
    // Uses toInt and then casts as unsigned long
  }
  if(done==true){
    a = periodChar.toInt();
    delay(10);
    Serial.println("Period is" + a);
    Serial.flush();
    done == false;
  }
  return a;
}

void loop()
{
  if(gotTime==false){
    //call function to read numerical input from serial
    readInput(&period);
    }
  else if(gotTime==true){
    digitalWrite(LED_PIN, HIGH);
    delay(period);
    digitalWrite(LED_PIN, LOW);
    delay(period);
    }
}

