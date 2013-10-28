String firstName="";
String lastName="";
boolean fcomplete=false;
boolean lcomplete=false;

void setup(){
  Serial.begin(9600);
  Serial.println("Enter your name:");
  Serial.println("Please enter your FIRST name, hit Return, then enter your"
  " LAST name.");
  delay(1);
}

void loop(){
  //  Clumsy state machine: while firstname is false, nag the user for a name
  if(fcomplete==false){
  while(Serial.available()){
    //readfChar checks to see if 
    char readfChar = (char)Serial.read();

     // check if the enter key has been hit.
     // (it took me FOREVER to find that '\r' means carriage return!)
      if (readfChar == '\r'){
        Serial.print("Hello, "+firstName);
        fcomplete=true;
        //  nag for last name
        Serial.println(", could I also have your LAST name?");
        Serial.flush();
        //since we have what we want, let's leave this lame old loop
        continue;
        }
        firstName += readfChar;
    }
  }

  //
  if(fcomplete==true){
  while(Serial.available()){
    char readlChar = (char)Serial.read();

      if (readlChar == '\r'){
        Serial.println("Okay, your LAST name is "+ lastName);
        lcomplete=true;
        Serial.flush();
        //since we have what we want, let's leave this lame old loop
        continue;
      }
        lastName += readlChar;
    }
  }

    if(fcomplete && lcomplete){
      //  greet the user
      Serial.println("Nice to meet you, " + lastName + ", " + firstName);

      // clear out the name
      firstName="";
      lastName="";
      fcomplete=false;
      lcomplete=false;
      Serial.flush();
      // pause for comic effect...
      delay(1000);
      // nag the user again!
      Serial.println("Shoot, I'm so forgetful! What's your name again?  ");
      }
}


