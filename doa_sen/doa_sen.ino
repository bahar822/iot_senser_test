
const int switchPin = 5;//スイッチ接続ピン
const int ledPin = 4;//LED接続ピン

void setup(){
  Serial.begin(115200);
  pinMode(switchPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(switchPin, HIGH);
}

void loop(){
  if(digitalRead(switchPin) == LOW){//磁気が離れると
    digitalWrite(ledPin, LOW);
    Serial.println("OFF");
  }
  else{//磁気を近づけると
    digitalWrite(ledPin, HIGH);
    Serial.println("ON");
  }
  delay(1000);
}
