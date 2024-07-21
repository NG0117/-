
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>


char ssid[] = "SonyXZ3";
char password[] = "0905570965";
String url = "http://api.thingspeak.com/update?api_key=0FHO8K1KEVPQE7XW";
char url1[] = "http://opendata2.epa.gov.tw/AQI.json"; //PM2.5的網址
WiFiClientSecure client;//網路連線物件
char host[] = "notify-api.line.me";//LINE Notify API網址
char SSID[] = "SonyXZ3";
char PASSWORD[] = "0905570965";
String Linetoken = "T0YjDdgvWFqpKgg3Sc5HzmHlm8Rxj9m8XHUvdwMGmDi";
bool SendFlag = false;
TaskHandle_t Task1;

void Task1_senddata(void * pvParameters ) {
  for (;;) {
if (WiFi.status() != WL_CONNECTED) {
  WiFi.begin(ssid, password);
  } else {

HTTPClient http;
http.begin(url1); 
int httpCode = http.GET();
//  Serial.print("httpCode=");
//  Serial.println(httpCode);
//5.檢查網頁連線是否正常
if (httpCode == HTTP_CODE_OK) {
//6.取得網頁內容
String payload = http.getString();
//    Serial.print("payload=");
//7.將資料顯示在螢幕上
//    Serial.println(payload);
DynamicJsonDocument AQJarray(payload.length()*2);
deserializeJson(AQJarray, payload);//解析payload為JSON Array格式
String PingtungPM25=AQJarray[55]["PM2.5"];//第0個是基隆的PM2.5
String PingtungAQI=AQJarray[55]["AQI"];
String PingtungSO2 = AQJarray[55]["SO2"];
String PingtungO3 = AQJarray[55]["O3"];
String PingtungNO2 = AQJarray[55]["NO2"];
String PingtungCO = AQJarray[55]["CO"];




//Serial.print("屏東PM2.5：" + PingtungPM25);
//Serial.println("ppm");
//Serial.println("屏東AQI:" + PingtungAQI);
//Serial.print("屏東O3:" + PingtungO3);
//Serial.println("ppb");
//Serial.print("屏東NO2:" + PingtungNO2);
//Serial.println("ppb");
//Serial.println("屏東CO:" + PingtungCO + "ppm");
//Serial.println("  ");

String sendData =  String("")+""+ PingtungPM25 +"," + PingtungAQI +","+ PingtungO3 +"," + PingtungNO2 +"," + PingtungCO +","+ PingtungSO2;
Serial2.println(sendData);
}
http.end();
//delay(500);
}

}
}


void setup() {
  
  Serial.begin(4800);
  Serial2.begin(4800);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
     }
//Serial.println("連線完成");
 Serial2.setTimeout(7);
  
   xTaskCreatePinnedToCore(
    Task1_senddata, /*任務實際對應的Function*/
    "Task1",        /*任務名稱*/
    10000,          /*堆疊空間*/
    NULL,           /*無輸入值*/
    0,                 /*優先序0*/
    &Task1,       /*對應的任務變數位址*/
    0);                /*指定在核心0執行 */
    
   
}

void loop() {
      
  // put your main code here, to run repeatedly:
if (WiFi.status() != WL_CONNECTED) {
  WiFi.begin(ssid, password);
  } 

  if (Serial2.available() > 0) {
    
  
//  Serial.print("loop主流程：溫濕度讀取，at core:");
//  Serial.println(xPortGetCoreID());
  
  String Read = readLine3();
  Serial.println(Read);

  int lastcommaPos; 
  int commaPos = Read.indexOf(',', 0); // find first comma 
  String val1 = Read.substring(0, commaPos); // grab first value 
  lastcommaPos = commaPos + 1; 
  commaPos = Read.indexOf(',', lastcommaPos); // find next comma 
  String val2 = Read.substring(lastcommaPos, commaPos); // grab next value 
  lastcommaPos = commaPos + 1; 
  commaPos = Read.indexOf(',', lastcommaPos); // find next comma 
  String val3 = Read.substring(lastcommaPos, commaPos); // grab next value 
  lastcommaPos = commaPos + 1; 
  commaPos = Read.indexOf(',', lastcommaPos); // find next comma 
  String val4 = Read.substring(lastcommaPos, commaPos); // grab next value 
  lastcommaPos = commaPos + 1; 
  commaPos = Read.indexOf(',', lastcommaPos); // find next comma 
  String val5 = Read.substring(lastcommaPos, commaPos); // grab next value 
  lastcommaPos = commaPos + 1; 
  commaPos = Read.indexOf(',', lastcommaPos); // find next comma 
  String val6 = Read.substring(lastcommaPos, commaPos); // grab next value

  
  char temperature = val2.toInt();
  char humidity = val3.toInt();
  int pm25 = val1.toInt();
  int pm10 = val4.toInt();
  int pm100 = val5.toInt();
  int co2 = val6.toInt();


if (pm25 < 300 && pm10 < 2000 && pm100 < 300 && temperature < 50 && humidity < 99 && co2 < 10000) {
  if (pm25 > 0 && pm10  > 100 && pm100  > 0 && temperature  > 10 && humidity  > 10 && co2 > 250) {
  
//  Serial.println("啟動網頁連線");
  HTTPClient http;
  String url1 = url + "&field1=" + (int)temperature + "&field2=" + (int)humidity + "&field3=" + (int)pm25 + "&field5=" + (int)pm10 + "&field6=" + (int)pm100 + "&field4=" + (int)co2;
  //String url1 = url + "&field1=" + (int)temperature + "&field2=" + (int)humidity;
  http.begin(url1);
  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK)      {
    String payload = http.getString();

//    Serial.print("網頁內容=");
//    Serial.println(payload);
//
//    Serial.print(val1);
//    Serial.print("  ");
//    Serial.print(val2);
//    Serial.print("   ");
//    Serial.println(val3);
//    Serial.print("   ");
//    Serial.println(val4);
//    Serial.print("   ");
//    Serial.println(val5);
  } else {
    //讀取失敗
//    Serial.println("網路傳送失敗");
  }
  http.end();
  //delay(500);//休息20秒
  SendFlag = true;
}
}

}
}

String readLine3(){
      String s =  "";
      char c;
      while((c = Serial2.read()) !='\n'){
          s += c;
      }  
      delay(10);
      return s;
}
