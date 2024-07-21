#include <SPI.h>
#include <Wire.h>
//#include <Adafruit_GFX.h>
//#include <Adafruit_SH1106.h>
//
//#define OLED_RESET -1
//Adafruit_SH1106 display(OLED_RESET);
//
//#if (SH1106_LCDHEIGHT != 64)
//#error("Height incorrect, please fix Adafruit_SH1106.h!");
//#endif

int state1;


void setup()   {                
  Serial.begin(9600);
//display.begin(SH1106_SWITCHCAPVCC, 0x3C);
// display.clearDisplay();
//pinMode(7,OUTPUT);
  
pinMode(13,OUTPUT);
pinMode(7, INPUT_PULLUP);

 
}  
void loop() {
  
//
//display.setTextSize(1);             // 設定文字大小
//  display.setTextColor(1);        // 1:OLED預設的顏色(這個會依該OLED的顏色來決定)
//  display.setCursor(25,0);             // 設定起始座標
//  display.print("Air-purifier");        // 要顯示的字串
//
//  display.setCursor(0,15);             // 設定起始座標
//  display.print("power:");        // 要顯示的字串


int val = digitalRead(7);

if (val == 0) {
  state1=!state1;
  delay(200);
  digitalWrite(13,state1);
  Serial.println("11  ");
}
if (state1 == 1) {
//display.setTextSize(1);             // 設定文字大小
//  display.setTextColor(1);        // 1:OLED預設的顏色(這個會依該OLED的顏色來決定)
//  display.setCursor(40,15 );             // 設定起始座標
//  display.print("ON");        // 要顯示的字串
// display.display();
  
}else {
  
//  display.setTextSize(1);             // 設定文字大小
//  display.setTextColor(1);        // 1:OLED預設的顏色(這個會依該OLED的顏色來決定)
//  display.setCursor(40,15);             // 設定起始座標
//  display.print("OFF");        // 要顯示的字串
//  display.display();
}
//display.clearDisplay();
  }
