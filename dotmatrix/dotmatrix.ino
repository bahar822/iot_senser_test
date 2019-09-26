#include  <Adafruit_GFX.h>
#include  <Max72xxPanel.h>

const int pinCS = 15;
const int numberOfHorizontalDisplays = 4;
const int numberOfVerticalDisplays   = 1;
Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

const int   wait = 50;
const int spacer = 1;
const int  width = 5 + spacer;

void setup(){
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
  matrix.fillScreen(LOW);

  Draw_Line(1);
  Draw_Line(2);

  matrix.write();
  delay(wait);
}


void Draw_Line(int line){
  for (int i = 0; i < 8; i++) {
    // drawPixel(x, y, color);
    matrix.drawPixel(line, i, 0x1);
  }
}
