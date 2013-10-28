//As noted in assignment, we were allowed to use the libraries

int status[8];

void setup()
{
  Serial.begin(9600);
  Serial.println("AVR Status Register");
}

void loop()
{
  int i;
  for(i=0; i<=7; i++){
    status[i]=bitRead(0x3F,i);
    Serial.print("Bit ");
    Serial.print(i);
    Serial.print(" Status: ");
    if(status[i]==0){
    Serial.println(" Off");
    }
    else{
    Serial.println(" On");
    }
  }
  //twiddle thumbs
  while(1){}
}
