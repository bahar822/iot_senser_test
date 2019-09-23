#define TRG 12
#define ECH 13
#define HIGHTIME 10
void setup() {
Serial.begin(9600);
pinMode(TRG, OUTPUT);
pinMode(ECH, INPUT);
}

void loop() {
int diff;
float dis;
digitalWrite(TRG, HIGH);
delayMicroseconds(HIGHTIME);
digitalWrite(TRG, LOW);
diff = pulseIn(ECH, HIGH);
dis = (float)diff * 0.01715;
Serial.print(dis);
Serial.println("cm");
delay(1000);
}
