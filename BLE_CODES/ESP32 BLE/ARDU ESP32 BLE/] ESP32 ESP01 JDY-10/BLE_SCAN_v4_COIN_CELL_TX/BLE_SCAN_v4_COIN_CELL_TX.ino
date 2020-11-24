/*
   Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
   Ported to Arduino ESP32 by Evandro Copercini

   Name: JDY-08, 
   Address: 50:8c:b1:40:16:47, 
   manufacturer data: 593588a0508cb1401647, 
   serviceUUID: 0000ffe0-0000-1000-8000-00805f9b34fb

   BATTERY STATUS
   DEVICE STATUS - SEND TO DH SERVER

   ESP32: TX = GPIO1; RX = GPIO3
*/

//#include <SoftwareSerial.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

BLEScan* pBLEScan;
int scanTime = 5; //In seconds
static uint8_t scan_count = 0;

String Status="0";
String ScanData="";
static int LED_PIN = 2;
static int INDICATOR_PIN = 15; //D15 GPIO15 BUZZER PIN
static bool FOUND = false;
static const uint8_t KNOWN_TAGS_COUNT = 1;   // BLE_Tag_Addrs_to_Scan[].length(); // not working

//"50:8c:b1:40:16:47"; // JDY-08 ZERO PCB 
//"98:5d:ad:23:14:bd"; // JDY-08 COIN CELL "985dad2314bd"
//"c8:fd:19:4a:f7:72"; // BLE3 (HM10) MAC (BLE_SERVER_TAG)

String BLE_Tag_Addrs_to_Scan[] = {"c8:fd:19:4a:be:1d"}; // "98:5d:ad:23:14:bd","50:8c:b1:40:16:47"}; // JDY-08 ZERO PCB

//String BLE_Tag_Addr_to_Scan;

//SoftwareSerial ESP_SoftSerial(2, 3); // Arduino : RX | TX <-> TX | RX : ESP8266

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks 
{
    void onResult(BLEAdvertisedDevice advertisedDevice) 
    {
      String ScanString = advertisedDevice.toString().c_str(); // [scan_count++]
      ScanData+=ScanString;
//      Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
      
    }
};

void BLE_setup()
{
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100); // 100
  pBLEScan->setWindow(90);  // 99 less or equal setInterval value 
}
  
void BLE_SCAN_TAGS()
{  
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
//  Serial.print("\nDevices found: ");  Serial.println(foundDevices.getCount());  
//  Serial.println("Scan done!");

//  Serial.println("\n\nScanData : "+ScanData+"\n");  
//  Serial.println("\nKNOWN_TAGS_COUNT : "+String(KNOWN_TAGS_COUNT)+"\n");

  FOUND = true; // false; 
  Status = "1";
  
  for(uint8_t i=0; i<KNOWN_TAGS_COUNT; i++)
  {
    if(ScanData.indexOf(BLE_Tag_Addrs_to_Scan[i]) > 0)
    {  
//      Serial.println("\n\n "+BLE_Tag_Addrs_to_Scan[i]+" FOUND - DEVICE IN RANGE\n");
//       FOUND = true; digitalWrite (INDICATOR_PIN, LOW);
       
    
    }
    else
    {  
//      Serial.println("\n\n "+BLE_Tag_Addrs_to_Scan[i]+" NOT FOUND - OUT OF RANGE!!\n");
       FOUND = false; digitalWrite (INDICATOR_PIN, HIGH); digitalWrite (LED_PIN, HIGH);
       Status = "0"; 
    }
  }
  
  ScanData="";
 
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
    
  if(FOUND) {digitalWrite (INDICATOR_PIN, LOW);  digitalWrite (LED_PIN, LOW); }
  else      {digitalWrite (INDICATOR_PIN, HIGH); digitalWrite (LED_PIN, HIGH);}  
}

void setup() 
{
  Serial.begin(9600);

//  ESP_SoftSerial.begin(9600);     delay(500);
  
//  Serial.println("Scanning...");

  pinMode (LED_PIN,OUTPUT); //Declare the in-built LED pin as output
  pinMode (INDICATOR_PIN,OUTPUT); //Declare BUZZER PIN as output
  
  BLE_setup();  
}

void loop() 
{
//  btStart();  
  BLE_SCAN_TAGS();
//  btStop();
  
  // "\"50:8c:b1:40:16:47\":\"1\", \"98:5d:ad:23:14:bd\":\"0\""
//  String str_Tx="\""+BLE_Tag_Addrs_to_Scan[0]+"\":\""+Status+"\""; // ;, \"98:5d:ad:23:14:bd\":\"0\""
  String str_Tx =String('T')+BLE_Tag_Addrs_to_Scan[0]+String('H')+Status+String('L')+String("500");
      Serial.println(str_Tx);

//    ESP_SoftSerial.print(str_Tx);  //    ESP_SoftSerial.write(str_Tx);

  delay(3000);
//  ESP.restart();
}

/*
https://stackoverflow.com/questions/54890524/how-to-increase-beacon-scan-sample-rate-use-esp32

I using the esp32 to detection beacon through rssi of beacon, but i meet some problem, first is rssi value is no stable so i need more sample of rssi, the second problem is esp32 scan sample rate is too slow, that faster speed only have one second scan rate, below is my used code

#include <BLEAdvertisedDevice.h>
#include <BLEDevice.h>
#include <BLEScan.h>

const int PIN = 2;
const int CUTOFF = -60;

void setup() {
pinMode(PIN, OUTPUT);
BLEDevice::init("");
}

void loop() {
BLEScan *scan = BLEDevice::getScan();
scan->setActiveScan(true);
BLEScanResults results = scan->start(1);
int best = CUTOFF;
for (int i = 0; i < results.getCount(); i++) 
{
BLEAdvertisedDevice device = 
results.getDevice(i);
int rssi = device.getRSSI();
if (rssi > best) {
  best = rssi;
 }
}
digitalWrite(PIN, best > CUTOFF ? HIGH : LOW);
}

*/

/*
  if(ScanData.indexOf(BLE_Tag_Addr_to_Scan) > 0)
  {  Serial.println("\n\n "+BLE_Tag_Addr_to_Scan+" FOUND - DEVICE IN RANGE\n");
     FOUND = true; digitalWrite (INDICATOR_PIN,LOW);
  }
  else
  {  Serial.println("\n\n "+BLE_Tag_Addr_to_Scan+" NOT FOUND - OUT OF RANGE!!\n");
     FOUND = false; digitalWrite (INDICATOR_PIN,HIGH);
  }
  
*/
