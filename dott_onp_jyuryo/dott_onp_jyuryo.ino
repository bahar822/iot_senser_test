#include  <Adafruit_GFX.h>
#include  <Max72xxPanel.h>

#define TRG 12
#define ECH 13
#define HIGHTIME 10


const int pinCS = 15;
const int numberOfHorizontalDisplays = 4;
const int numberOfVerticalDisplays   = 1;
Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

const int   wait = 2000;
const int spacer = 1;
const int  width = 5 + spacer;

#include <Arduino.h>

void AE_HX711_Init(void);
void AE_HX711_Reset(void);
long AE_HX711_Read(void);
long AE_HX711_Averaging(long adc,char num);
float AE_HX711_getGram(char num);

//---------------------------------------------------//
// ピンの設定
//---------------------------------------------------//
#define pin_dout  22

#define pin_slk   19

//---------------------------------------------------//
// ロードセル　Ｓ字型　ＳＣ３０１Ａ　１００ｋＧ [P-12036]
//---------------------------------------------------//
//#define OUT_VOL   0.002f      //定格出力 [V]
//#define LOAD      100000.0f   //定格容量 [g]

//---------------------------------------------------//
// ロードセル　シングルポイント（ ビーム型）　ＳＣ６０１　１２０ｋＧ [P-12035]
//---------------------------------------------------//
//#define OUT_VOL   0.001f      //定格出力 [V]
//#define LOAD      120000.0f   //定格容量 [g]

//---------------------------------------------------//
// ロードセル　シングルポイント（ ビーム型）　ＳＣ１３３　２０ｋＧ [P-12034]
//---------------------------------------------------//
#define OUT_VOL   0.002f      //定格出力 [V]
#define LOAD      20000.0f    //定格容量 [g]

//---------------------------------------------------//7p0
// ロードセル　シングルポイント（ビーム型）　ＳＣ６１６Ｃ　５００ｇ[P-12532]
//---------------------------------------------------//
//#define OUT_VOL   0.0007f   //定格出力 [V]
//#define LOAD      500.0f    //定格容量 [g]




float offset;

void setup(){

Serial.begin(9600);
pinMode(TRG, OUTPUT);
pinMode(ECH, INPUT);

Serial.println("AE_HX711 test"); 
AE_HX711_Init();
AE_HX711_Reset();
offset = AE_HX711_getGram(30); 

  
  // 明るさ調整 0~15
  matrix.setIntensity(1);

  /* ディスプレイの位置が合わない場合は下記を使う
    matrix.setPosition( 0, 0, 0) ;  // The  first display is at <0, 0>
    matrix.setPosition( 1, 1, 0) ;  // The second display is at <1, 0>
    matrix.setPosition( 2, 2, 0) ;  // The  third display is at <2, 0>
    matrix.setPosition( 3, 3, 0) ;  // The fourth display is at <3, 0>
  */

  // ディスプレイの位置定義
  matrix.setRotation( 0, 1) ;
  matrix.setRotation( 1, 1) ;
  matrix.setRotation( 2, 1) ;
  matrix.setRotation( 3, 1) ;
}
void loop(){

  float data;
  char S1[20];
  char s[20];
  data = AE_HX711_getGram(5);
  sprintf(S1,"%s [g] (0x%4x)",dtostrf((data-offset), 5, 3, s),AE_HX711_Read());
  Serial.println(S1);

  
  int diff;
  float dis;
  float pas;
  float maxn = 67;


  matrix.fillScreen(LOW);

  digitalWrite(TRG, HIGH);
  delayMicroseconds(HIGHTIME);
  digitalWrite(TRG, LOW);

  diff = pulseIn(ECH, HIGH);
  // dis = cm
  dis = (float)diff * 0.01715;

  Serial.print("dis = ");
  Serial.print(round(dis));
  Serial.print("cm :");
  
  pas = Draw_Retu(maxn,dis);

  for(int i = 0; i < (round(pas)) ; i++ ){
       Draw_Line(i);
  }
  

  matrix.write();
  delay(wait);

  
}
int Draw_Retu(float maxn,float dis){

  float pas;
  float war;
  war= dis/maxn;
  pas = 32*war;

  Serial.print("war = ");
  Serial.print(war);
  Serial.print(": pas = ");
  Serial.print(pas);
  Serial.println("");
  return pas;

}
void Draw_Line(int line){
  for (int i = 0; i < 8; i++) {
    // drawPixel(x, y, color);
    matrix.drawPixel(line, i, 0x1);
  }
}

void AE_HX711_Init(void)
{
  pinMode(pin_slk, OUTPUT);
  pinMode(pin_dout, INPUT);
}

void AE_HX711_Reset(void)
{
  digitalWrite(pin_slk,1);
  delayMicroseconds(100);
  digitalWrite(pin_slk,0);
  delayMicroseconds(100); 
}

long AE_HX711_Read(void)
{
  long data=0;
  while(digitalRead(pin_dout)!=0);
  delayMicroseconds(10);
  for(int i=0;i<24;i++)
  {
    digitalWrite(pin_slk,1);
    delayMicroseconds(5);
    digitalWrite(pin_slk,0);
    delayMicroseconds(5);
    data = (data<<1)|(digitalRead(pin_dout));
  }
  //Serial.println(data,HEX);   
  digitalWrite(pin_slk,1);
  delayMicroseconds(10);
  digitalWrite(pin_slk,0);
  delayMicroseconds(10);
  return data^0x800000; 
}


long AE_HX711_Averaging(long adc,char num)
{
  long sum = 0;
  for (int i = 0; i < num; i++) sum += AE_HX711_Read();
  return sum / num;
}

float AE_HX711_getGram(char num)
{
  #define HX711_R1  20000.0f
  #define HX711_R2  8200.0f
  #define HX711_VBG 1.25f
  #define HX711_AVDD      4.2987f//(HX711_VBG*((HX711_R1+HX711_R2)/HX711_R2))
  #define HX711_ADC1bit   HX711_AVDD/16777216 //16777216=(2^24)
  #define HX711_PGA 128
  #define HX711_SCALE     (OUT_VOL * HX711_AVDD / LOAD *HX711_PGA)
  
  float data;

  data = AE_HX711_Averaging(AE_HX711_Read(),num)*HX711_ADC1bit; 
  //Serial.println( HX711_AVDD);   
  //Serial.println( HX711_ADC1bit);   
  //Serial.println( HX711_SCALE);   
  //Serial.println( data);   
  data =  data / HX711_SCALE;


  return data;
}
