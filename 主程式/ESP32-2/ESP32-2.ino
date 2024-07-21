#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <TimedAction.h>
String url = "http://api.thingspeak.com/update?api_key=0FHO8K1KEVPQE7XW";
WiFiClientSecure client;//網路連線物件
char host[] = "notify-api.line.me";//LINE Notify API網址
char SSID[] = "SonyXZ3";
char PASSWORD[] = "0905570965";
String Linetoken = "T0YjDdgvWFqpKgg3Sc5HzmHlm8Rxj9m8XHUvdwMGmDi";
String Read = "";
void ReadData() {
Read = readLine3();
Serial.println(Read);
delay(100);
}

TimedAction readData = TimedAction(1000, ReadData);

void setup() {
  Serial.begin(4800);
  Serial2.begin(4800);

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
     }
Serial.println("連線完成");
 Serial2.setTimeout(7);

}
void loop() {
  // put your main code here, to run repeatedly:
readData.check();
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


int temperature = val2.toInt();
int humidity = val3.toInt();
int pm25 = val1.toInt(); 
int pm10 = val4.toInt();
int pm100 = val5.toInt();
int co2 = val6.toInt();

if (pm25 < 300 && pm10 < 2000 && pm100 < 300 && temperature < 50 && humidity < 99) {
  if (pm25 > 0 && pm10  > 100 && pm100  > 0 && temperature  > 10 && humidity  > 10) {
    
//    Serial.print("Task1：啟動網頁連線，at core:");
  //Serial.println(xPortGetCoreID());
    if ((int)pm25 >= 50) {
    String message = "目前環境空氣品質過差，出門請記得戴口罩";
    message += "\n溫度=" + String(((int)temperature)) + " *C";
    message += "\nPM1.0濃度=" + String(((int)pm10)) + " ug/m3";
    message += "\nPM2.5濃度=" + String(((int)pm25)) + " ug/m3";
    message += "\nPM10濃度=" + String(((int)pm100)) + " ug/m3";
    message += "\nCO2濃度" + String(((int)co2)) + " ppm";
    
    Serial.println(message);
    if (client.connect(host, 443)) {
      int LEN = message.length();
      String url = "/api/notify";
      client.println("POST " + url + " HTTP/1.1");
      client.print("Host: "); client.println(host);
      client.print("Authorization: Bearer "); client.println(Linetoken);
      client.println("Content-Type: application/x-www-form-urlencoded");
      client.print("Content-Length: "); client.println( String((LEN + 8)) );
      client.println();      
      client.print("message="); client.println(message);
      client.println();
      delay(1);
      String response = client.readString();
//      Serial.println(response);

    }
    else {
//      Serial.println("connected fail");
    }
     
  } else {
     delay(1);
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
