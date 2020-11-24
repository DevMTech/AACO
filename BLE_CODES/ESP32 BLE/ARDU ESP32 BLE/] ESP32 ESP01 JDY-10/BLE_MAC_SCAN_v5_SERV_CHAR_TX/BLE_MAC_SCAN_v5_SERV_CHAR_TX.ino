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
//#include <BLEUtils.h>
//#include <BLEScan.h>
//#include <BLEAdvertisedDevice.h>


static BLEUUID serviceUUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b"); //Remote Service UUID of BLE_SERVER_TAG  
static BLEUUID    charUUID("beb5483e-36e1-4688-b7f5-ea07361b26a8"); //Remote Characteristic  UUID of BLE_SERVER_TAG 
static BLERemoteCharacteristic* pRemoteCharacteristic;
BLEScanResults foundDevices;
static BLEAddress *Server_BLE_Address;
String Scanned_BLE_Address;


BLEScan* pBLEScan; //Name the scanning device as pBLEScan
int scanTime = 5; //In seconds
static uint8_t scan_count = 0;

//"50:8c:b1:40:16:47"; // JDY-08 ZERO PCB 
//"98:5d:ad:23:14:bd"; // JDY-08 COIN CELL "985dad2314bd"
//"c8:fd:19:4a:f7:72"; // BLE3 (HM10) MAC (BLE_SERVER_TAG)
//String BLE_Tag_Addr_to_Scan;
String BLE_Tag_Addrs_to_Scan[] = {"c8:fd:19:4a:be:1d"}; // "98:5d:ad:23:14:bd","50:8c:b1:40:16:47"}; // JDY-08 ZERO PCB
static const uint8_t KNOWN_TAGS_COUNT = 1; // BLE_Tag_Addrs_to_Scan[].length(); // not working
uint8_t TagFoundStatus[KNOWN_TAGS_COUNT]={0};
uint8_t paired[KNOWN_TAGS_COUNT] = {0}; //pairing check  
String Status="0";
String ScanData="";
static int LED_PIN = 2;
static int INDICATOR_PIN = 15; //D15 GPIO15 BUZZER PIN
static bool FOUND = false;
   

//SoftwareSerial ESP_SoftSerial(2, 3); // Arduino : RX | TX <-> TX | RX : ESP8266

bool connectToserver (BLEAddress pAddress) // Server_BLE_Address - REMOTE SERVER (BLE TAG) ADDR
{    
    BLEClient*  pClient  = BLEDevice::createClient();
    Serial.println(" - Created client");

    // Connect to the BLE Server.
    pClient->connect(pAddress); // Server_BLE_Address - REMOTE SERVER ADDR
    Serial.println(" - Connected to Server_BLE_Address - BLE_SERVER_TAG");

    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService != nullptr)
    {   Serial.println(" - Found our service");//      return true;
    }
    else
    {  return false;}

    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
    if (pRemoteCharacteristic != nullptr)
    {   Serial.println(" - Found our characteristic");
    }
    else
    {   return false;}
        
    return true;
}

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks 
{
    void onResult(BLEAdvertisedDevice advertisedDevice) 
    { Server_BLE_Address = new BLEAddress(advertisedDevice.getAddress());  // static BLEAddress *    
      Scanned_BLE_Address = Server_BLE_Address->toString().c_str(); // String
      
//      Serial.printf("Scan Result: Advertised Device: %s \n", advertisedDevice.toString().c_str());
      String ScanString = advertisedDevice.toString().c_str(); // [scan_count++]
      ScanData+=ScanString;  

      BLE_SCAN_TAGS_BY_MAC_ADDR();
    }
};

void BLE_setup()
{
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks()); //Call the class that is defined above
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100); // 100
  pBLEScan->setWindow(90);  // 99 less or equal setInterval value 
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
{ digitalWrite (INDICATOR_PIN, LOW); digitalWrite (LED_PIN, LOW);
  for(uint8_t i=0; i<KNOWN_TAGS_COUNT; i++)
  {  TagFoundStatus[i]=0;
  }
//  btStart();  
  foundDevices = pBLEScan->start(scanTime, false); // BLEScanResults 
  //Scan for 'scanTime' seconds to find the Advertising Tag
    Serial.println("Scan done!");
//  Serial.print("\nDevices found: ");  Serial.println(foundDevices.getCount());  

//  Serial.println("\n\nScanData : "+ScanData+"\n");  
//  Serial.println("\nKNOWN_TAGS_COUNT : "+String(KNOWN_TAGS_COUNT)+"\n");
  
//BLE_SCAN_TAGS_BY_MAC_ADDR();   
//  BLE_SCAN_TAGS();
//  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
//  btStop();

    for(uint8_t i=0; i<KNOWN_TAGS_COUNT; i++)
    { 
      if(!TagFoundStatus[i]) 
      {Status="0"; digitalWrite (INDICATOR_PIN, HIGH); digitalWrite (LED_PIN, HIGH);}
      else {Status="1";} 
                        
      String str_Tx =String('T')+BLE_Tag_Addrs_to_Scan[i]+String('H')+Status+String('L')+String("500");
      Serial.println(str_Tx);
    }
  
  // "\"50:8c:b1:40:16:47\":\"1\", \"98:5d:ad:23:14:bd\":\"0\""
//  String str_Tx="\""+BLE_Tag_Addrs_to_Scan[0]+"\":\""+Status+"\""; // ;, \"98:5d:ad:23:14:bd\":\"0\""
  

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
