void setup() {
  pinMode(4, INPUT);
  pinMode(2, OUTPUT);
  Serial.begin(115200);
  // put your setup code here, to run once:

}

void loop() {
  if(digitalRead(4)){
    Serial.println("on");
    digitalWrite(2, HIGH);
  }else{
    Serial.println("off");
    digitalWrite(2, LOW);
  }
  //delay(1000);
  // put your main code here, to run repeatedly:

}
