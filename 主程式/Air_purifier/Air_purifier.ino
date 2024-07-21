#include <SPI.h>    
#include <Wire.h>      
#include <SoftwareSerial.h>
#include <DHT.h>
#include "kSeries.h"
#include <DS3231.h>  //匯入函式庫
#include <SD.h>
#include <TimedAction.h>

#include "Adafruit_CCS811.h"

#define DHTPIN 8     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
#define fan 4

int maxHum = 60;
int maxTemp = 40;
const int chipSelect = 53;
DHT dht(DHTPIN, DHTTYPE);
DS3231 Clock;

Adafruit_CCS811 ccs;
kSeries K_30(12,13);
int sec;
int minute1;
int hour1;
int date1;
int month1;
int year1;
int dow1;
int delaytime = 10000;
int delaytime2 = 2000;
int delaytime3 = 500;
int delaytime4 = 1000;
long pmat10 = 0;    
long pmat25 = 0;    
long pmat100 = 0;    
float humandity;
float temperature;
bool Century=false;
bool h12;
bool PM;
byte ADay, AHour, AMinute, ASecond, ABits;
bool ADy, A12h, Apm;
byte year, month, date, hour, minute, second;
char buffer [16];

int valMultiplier = 1;
char latitude = 0;
char longitude = 0;
int co2;
int TVOC;
char PM25;
char SO2;
char O3;
char CO;
char AQI;
char NO2;
File myFile;
String filename = "Data.csv";     //要寫入的檔案名稱

void Control() {
  if(pmat25 >= 50 || co2 >= 1500) {
  digitalWrite(10, HIGH);
} else {
  digitalWrite(10, LOW);
}
}

void ReadAQIData() {
  if (Serial1.available()) {

  String Read = readLine3() ;
//Serial.println(Read);


int lastcommaPos; 
  int commaPos = Read.indexOf(',', 0); 
  String val1 = Read.substring(0, commaPos); 
  lastcommaPos = commaPos + 1; 
  commaPos = Read.indexOf(',', lastcommaPos); 
  String val2 = Read.substring(lastcommaPos, commaPos); 
  lastcommaPos = commaPos + 1; 
  commaPos = Read.indexOf(',', lastcommaPos); 
  String val3 = Read.substring(lastcommaPos, commaPos); 
  lastcommaPos = commaPos + 1; 
  commaPos = Read.indexOf(',', lastcommaPos); 
  String val4 = Read.substring(lastcommaPos, commaPos);
  lastcommaPos = commaPos + 1; 
  commaPos = Read.indexOf(',', lastcommaPos);
  String val5 = Read.substring(lastcommaPos, commaPos); 
  lastcommaPos = commaPos + 1; 
  commaPos = Read.indexOf(',', lastcommaPos); 
  String val6 = Read.substring(lastcommaPos, commaPos); 

int PM25 = val2.toInt();
int AQI = val3.toInt();
int O3 = val1.toInt(); 
int NO2 = val4.toInt();
//float CO = val5.toInt();
int SO2 = val6.toInt();

  String command26 = "forecast.t4.txt=\""+String(SO2)+"\"";  //設定控制螢幕指令
  Serial2.print(command26);
  endNextionCommand();
String command27 = "forecast.t7.txt=\""+String(O3)+"\"";
  Serial2.print(command27);
  endNextionCommand();
  String command28 = "forecast.t10.txt=\""+String(val5)+"\"";
  Serial2.print(command28);
  endNextionCommand();
  String command29 = "forecast.t13.txt=\""+String(PM25)+"\"";
  Serial2.print(command29);
  endNextionCommand();
  String command30 = "forecast.t16.txt=\""+String(AQI)+"\"";
  Serial2.print(command30);
  endNextionCommand();
  
 
} else {
  
}
}

void ReadTVOC() {
  if(ccs.available()){
    if(!ccs.readData()){
      TVOC = ccs.getTVOC();  //獲取TVOC數據傳送至主板
//      Serial.print(TVOC);
    }
  }
}

void ReadCO2() {
  co2 = K_30.getCO2('p'); //獲取CO2數據傳送至主板
  //Serial.println(co2);
}

void WriteSD () {
   myFile = SD.open(filename, FILE_WRITE);  //打開SD卡中excel檔並且設定寫入模式
if (myFile) {
myFile.print("20");  
myFile.print(year1);  //寫入時間(年分)
myFile.print("-");
myFile.print(month1);
myFile.print("-");
myFile.print(date1);
myFile.print(",");
myFile.print(hour1);
myFile.print("-");
myFile.print(minute1);
myFile.print("-");
myFile.print(sec);
myFile.print(",");

//myFile.print(latitude, 6);
//myFile.print(",");
//myFile.print(longitude, 6);
//myFile.print(",");

myFile.print(pmat10);
myFile.print("ug/m3");
myFile.print(",");
myFile.print(pmat25);
myFile.print("ug/m3");
myFile.print(",");
myFile.print(pmat100);
myFile.print("ug/m3");
myFile.print(",");
myFile.print(temperature);
myFile.print("'C");
myFile.print(",");
myFile.print(humandity);
myFile.println("%");

myFile.print(co2);
myFile.println("ppm");

myFile.print(TVOC);
myFile.println("ppb");
myFile.println();
myFile.close();
//     Serial.println("done.");
}
}



TimedAction Write = TimedAction (delaytime, WriteSD);
TimedAction readCO2 = TimedAction(delaytime2, ReadCO2);
TimedAction readTVOC = TimedAction(delaytime3, ReadTVOC);
TimedAction readAQIData = TimedAction(delaytime4, ReadAQIData);
TimedAction control = TimedAction(delaytime3, Control);

void setup() {
  Serial1.begin(4800);
  Serial3.begin(9600);
  Serial.begin(4800);
  Serial2.begin(9600);
  pinMode(fan, OUTPUT);
  pinMode(53,OUTPUT);             //保留pin10, SD Library需要使用
  pinMode(10,OUTPUT);
  dht.begin();
  Wire.begin();
  ccs.begin();
  while (!SD.begin(4)) {}

}

void loop() {
readpm25();
readdht();
Write.check();
readCO2.check();
readTVOC.check();
HMIwrite();
readAQIData.check();
control.check();

}

void readpm25(){    
int count = 0;    
unsigned char c;    
unsigned char high;    
while (Serial3.available()) {     
c = Serial3.read();    
if((count==0 && c!=0x42) || (count==1 && c!=0x4d)){    

break;    
}    
if(count > 27){     
// Serial.println("complete");    
break;    
}    
else if(count == 10 || count == 12 || count == 14 || count == 24 || count == 26    
|| count == 16 || count == 18 || count == 20 || count == 22    
) {    
high = c;    
}    
else if(count == 11){    
pmat10 = 256*high + c;   //獲取PM10數據   

}    
else if(count == 13){    //獲取PM2.5數據
pmat25 = 256*high + c;    
     
}    
else if(count == 15){    //獲取PM1.0數據
pmat100 = 256*high + c;    

}    
//else if(count == 25){            //獲取溫度數據
//temperature = (256*high + c)/10;     
//}    
//else if(count == 27){                
//humandity = (256*high + c)/10;    
//}       
count++; 


}    
while(Serial3.available()) Serial3.read();    

  
  sec=Clock.getSecond();//獲取秒
  minute1=Clock.getMinute();//獲取分
  hour1=Clock.getHour(h12, PM);//獲取小時
  date1=Clock.getDate();//獲取日
  month1=Clock.getMonth(Century);//獲取月份
  year1=Clock.getYear();//獲取年分(只有後兩位數)
  dow1=Clock.getDoW();//獲取星期



   
   
 String sendData =  String("")+""+ pmat25 +"," + temperature +","+ humandity +"," + pmat10 +"," + pmat100 +","+ co2;  //將各感測器集成成一串方便傳輸
Serial1.println(sendData);  //傳送資料至ESP32
  Serial.println(sendData);

}  

void readdht () {
   humandity = dht.readHumidity();  //讀取濕度感測器並傳送至主板
   temperature = dht.readTemperature();  
}



void endNextionCommand() {  //結數字元
Serial2.write(0xff);
Serial2.write(0xff);
Serial2.write(0xff);
}




void HMIwrite() {
String command1 = "page1.t12.txt=\""+String(pmat10)+"\"";
Serial2.print(command1);
endNextionCommand();
String command2 = "page1.t13.txt=\""+String(pmat25)+"\"";
Serial2.print(command2);
endNextionCommand();   
String command3 = "page1.t14.txt=\""+String(pmat100)+"\"";
Serial2.print(command3);
endNextionCommand();

String command4 = "page1.t9.txt=\""+String(hour1)+"\"";
Serial2.print(command4);
endNextionCommand();
String command5 = "page1.t21.txt=\""+String(minute1)+"\"";
Serial2.print(command5);
endNextionCommand();   
String command6 = "page1.t25.txt=\""+String(sec)+"\"";
Serial2.print(command6);
endNextionCommand();
String command7 = "page1.t41.txt=\""+String(dow1)+"\"";
Serial2.print(command7);
endNextionCommand();
String command8 = "page1.t38.txt=\""+String(date1)+"\"";
Serial2.print(command8);
endNextionCommand();
String command9 = "page1.t36.txt=\""+String(month1)+"\"";
Serial2.print(command9);
endNextionCommand();
String command10 = "page1.t34.txt=\""+String(year1)+"\"";
Serial2.print(command10);
endNextionCommand();



  
  String command11 = "page1.t17.txt=\""+String(temperature)+"\"";
Serial2.print(command11);
endNextionCommand();
String command12 = "page1.t3.txt=\""+String(humandity)+"\"";
Serial2.print(command12);
endNextionCommand();

String command25 = "page1.t23.txt=\""+String(TVOC)+"\"";
  Serial2.print(command25);
  endNextionCommand();

  String command18 = "page1.t22.txt=\""+String(co2)+"\"";
Serial2.print(command18);
endNextionCommand();


if (pmat25 < 15 ) {
  String command19 = "page1.t27.txt=\""+String("excellent")+"\"";
  Serial2.print(command19);
  endNextionCommand();
}else if (pmat25 > 15 && pmat25 < 35 ) {
  String command20 = "page1.t28.txt=\""+String("good")+"\"";
  Serial2.print(command20);
  endNextionCommand();
}else if (pmat25 > 35 && pmat25 < 54 ) {
  String command21 = "page1.t29.txt=\""+String("normal")+"\"";
  Serial2.print(command21);
  endNextionCommand();
}else if (pmat25 > 54 && pmat25 < 150 ) {
  String command22 = "page1.t30.txt=\""+String("bad")+"\"";
  Serial2.print(command22);
  endNextionCommand();
}else if (pmat25 > 150 && pmat25 < 250 ) {
  String command23 = "page1.t31.txt=\""+String("worse")+"\"";
  Serial2.print(command23);
  endNextionCommand();
}else if (pmat25 > 250 && pmat25 < 500) {
  String command24 = "page1.t32.txt=\""+String("worst")+"\"";
  Serial2.print(command24);
  endNextionCommand();

}
}


String readLine3(){
      String s =  "";
      char c;
      while((c = Serial1.read()) !='\n'){
          s += c;
      }  
      delay(10);
      return s;
}
