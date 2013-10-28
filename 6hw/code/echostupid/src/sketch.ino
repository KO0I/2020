byte byteRead;

void setup(){
Serial.begin(9600);
}

void loop(){
  if (Serial.available()){
  byteRead = Serial.read();
  Serial.write(byteRead);
  }
}
