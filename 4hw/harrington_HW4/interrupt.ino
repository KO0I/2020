//  using attachInterrupt(), count the number of times a signal on Digital
//  Pin 2 goes low (0V).
//  Next, print the number of times that have gone low

//  First, init global variables
//  Since count values are not ever going to be negative, use unsigned ints
volatile unsigned int count = 0; // for the ISR
volatile unsigned int last_count = 0;
//  This is a software debounce to prevent the values from blowing up
volatile short trig = 1;

void setup()
{
  Serial.begin(9600);
  pinMode(2,INPUT_PULLUP);
  //(interrupt, function, PINMODE)
  attachInterrupt(0,pin2_ISR,FALLING);
}

//  Now, here is the Interrupt Service Routine (ISR) for Pin 2
void pin2_ISR(){
  if(trig){
  count++;
  //  By setting trig to zero (false), this acts as a software debounce
  trig=0;
  }
}

void loop()
{
  if(!trig){
    last_count = count;
    Serial.println(last_count);
    delay(100);
    //  Allow trigger to run again
    trig=1;
    }
}
