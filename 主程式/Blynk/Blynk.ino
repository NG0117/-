#define BLYNK_PRINT Serial
#define BLYNK_USE_DIRECT_CONNECT
#include <BlynkSimpleEsp32_BLE.h>
#include <BLEDevice.h>
#include <BLEServer.h>
char auth[] = "DEzCLUOfEVxh_82TNsl3gMwkIfYUTL3A";
int val;
int val2;
int state1;



//公用變數區
byte temperature = 0;
byte humidity = 0;
bool SendFlag = false;
//宣告任務Task1
TaskHandle_t Task1;

//任務1副程式Task1_senddata
void Task1_senddata(void * pvParameters ) {
  //無窮迴圈
  for (;;) {
    //偵測上傳旗標是否為true
    if (SendFlag) {
//      Serial.print("Task1：啟動網頁連線，at core:");
//      Serial.println(xPortGetCoreID());

//副程式


     
      //修改完畢，修改傳送旗標=false
      SendFlag = false;
      
    } else {
      //Task1休息，delay(1)不可省略
      delay(1);
    }
  }
}

void setup()
{
  Serial.begin(9600);
  Blynk.setDeviceName("Blynk");
  Blynk.begin(auth);
  pinMode(2,OUTPUT);
  pinMode(15,OUTPUT);
  pinMode(4,INPUT);
   ledcSetup(0, 9000, 8);
  ledcAttachPin(15, 0);








  //在核心0啟動任務1
  xTaskCreatePinnedToCore(
    Task1_senddata, /*任務實際對應的Function*/
    "Task1",        /*任務名稱*/
    10000,          /*堆疊空間*/
    NULL,           /*無輸入值*/
    0,                 /*優先序0*/
    &Task1,       /*對應的任務變數位址*/
    0);                /*指定在核心0執行 */
}

void loop()
{
//  Serial.print("loop主流程：溫濕度讀取，at core:");
//  Serial.println(xPortGetCoreID());
  
Blynk.run();

  SendFlag = true;
delay(100);
}
BLYNK_WRITE(V1)
{
val = param.asInt();
//Serial.println(param.asInt());
  ledcWrite(0, val);
}
