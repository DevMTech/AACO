/*
   Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
   Ported to Arduino ESP32 by Evandro Copercini

   Name: JDY-08, 
   Address: 50:8c:b1:40:16:47, 
   manufacturer data: 593588a0508cb1401647, 
   serviceUUID: 0000ffe0-0000-1000-8000-00805f9b34fb

   BATTERY STATUS
   DEVICE STATUS - SEND TO DH SERVER
   
*****************************************************************************

Scan done!
RSSI : -73
M30:ae:a4:7a:dc:16S0
Mc8:fd:19:4a:be:1dS0
M98:5d:ad:23:14:bdS0
M50:8c:b1:40:16:47S0
Scan Result: BLE Advertised Device found: Name: BLE32NotifyServer, Address: 30:ae:a4:7a:dc:16, serviceUUID: 4fafc201-1fb5-459e-8fcc-c5c9c331914b, txPower: 3 
 - haveServiceUUID() && isAdvertisingService(serviceUUID)
4fafc201-1fb5-459e-8fcc-c5c9c331914b


Inside SCAN_BLE_TAGS_BY_MAC_ADDR

 30:ae:a4:7a:dc:16 FOUND BLE SERVER DEVICE IN RANGE
 ... connecting to Server as Client

 - Created client


   ESP32: TX = GPIO1; RX = GPIO3
*/

//#include <SoftwareSerial.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>


static BLEUUID serviceUUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b"); //Remote Service UUID of BLE_SERVER_TAG  
static BLEUUID    charUUID("beb5483e-36e1-4688-b7f5-ea07361b26a8"); //Remote Characteristic  UUID of BLE_SERVER_TAG 

//static BLEUUID serviceUUID("5070eae3-05ae-4af4-8f93-6dac9c4cf5c7"); //Remote Service UUID of BLE_SERVER_TAG  
//static BLEUUID    charUUID("f3e75829-1ce3-44ec-b7d4-46445313043e"); //Remote Characteristic  UUID of BLE_SERVER_TAG 

static BLERemoteCharacteristic* pRemoteCharacteristic;
BLEScanResults scanResultDevicesFound;
static BLEAddress *Server_BLE_Address;
String Scanned_BLE_Address;
String RemoteServerReadCharacValue;
String RemoteServerNotifyCharacValue;

BLEScan* pBLEScan; //Name the scanning device as pBLEScan
int scanTime = 5; //In seconds
static uint8_t scan_count = 0;

//"50:8c:b1:40:16:47"; // JDY-08 ZERO PCB 
//"c8:fd:19:4a:be:1d"; // JDY-08 COIN CELL // "98:5d:ad:23:14:bd";  "985dad2314bd" 
//"c8:fd:19:4a:f7:72"; // BLE3 (HM10) MAC (BLE_SERVER_TAG)
//"30:ae:a4:7a:dc:16"  // ESP32 DEVKITV1 BLK
//"24:0a:c4:83:20:c2"  // ESP32 NARROW DEVKIT1
//"cc:50:e3:a9:4c:da", // ESP32 DEVKITV1 BLK
// ":::::",
//String BLE_Tag_Addr_to_Scan;
String BLE_Tag_Addrs_to_Scan[] = {"cc:50:e3:a9:4c:da","24:0a:c4:83:20:c2","c8:fd:19:4a:be:1d","c8:fd:19:4a:f7:72","30:ae:a4:7a:dc:16","50:8c:b1:40:16:47"}; 
static const uint8_t KNOWN_TAGS_COUNT = 4; // BLE_Tag_Addrs_to_Scan[].length(); // not working
uint8_t TagFoundStatus[KNOWN_TAGS_COUNT]={0};
uint8_t paired[KNOWN_TAGS_COUNT] = {0}; //pairing check  
String Status="0";
String ScanData="";
static int LED_PIN = 2;
static int INDICATOR_PIN = 15; //D15 GPIO15 BUZZER PIN
static bool FOUND = false;
const int CUTOFF = -60; // dB   
static bool doConnect = false;
static bool connected = false;
static bool doScan = false;
//SoftwareSerial ESP_SoftSerial(2, 3); // Arduino : RX | TX <-> TX | RX : ESP8266

static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,  size_t length,   bool isNotify) 
  {
    Serial.print(" - Notify callback for characteristic ");
    Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
    Serial.print(" of data length ");
    Serial.println(length);
//    Serial.print("data: ");
//    Serial.println((char*)pData);
    
    RemoteServerNotifyCharacValue = (char*)pData;
    Serial.print(" - RemoteServerNotifyCharacValue: ");
    Serial.println(RemoteServerNotifyCharacValue);
    Serial.println();

    //      String str_Tx =String('T')+BLE_Tag_Addrs_to_Scan[i]+String('H')+Status+String('L')+String("500");
      String str_Tx =String('M')+Scanned_BLE_Address+String('S')+RemoteServerNotifyCharacValue;
      Serial.println(str_Tx);
}

class MyClientCallback : public BLEClientCallbacks 
{
  void onConnect(BLEClient* pclient) 
  {
    Serial.println(" - onConnect");
  }

  void onDisconnect(BLEClient* pclient) 
  {
    connected = false;
    Serial.println(" - onDisconnect");
    ESP.restart();
//    scanResultDevicesFound = pBLEScan->start(scanTime, false);
//    loop();
  }
};

bool connectToServer (BLEAddress pAddress) // Server_BLE_Address - REMOTE SERVER (BLE TAG) ADDR
{   Serial.println("\n\nInside connectToServer (BLEAddress pAddress)"); 
    BLEClient*  pClient  = BLEDevice::createClient();
    Serial.println(" - Created client");

    pClient->setClientCallbacks(new MyClientCallback());
    
    // Connect to the BLE Server.
    pClient->connect(pAddress); // Server_BLE_Address - REMOTE SERVER ADDR
    Serial.println(" - Connected to Server_BLE_Address - BLE_SERVER_TAG");

    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService != nullptr)
    {   Serial.println(" - Found our service");//      return true;
        Serial.println(serviceUUID.toString().c_str());
    }
    else
    { Serial.print(" - Failed to find our service UUID: ");
      Serial.println(serviceUUID.toString().c_str());
      Serial.println(" - DISCONNECTING");
      pClient->disconnect(); 
      return false;
    }

    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
    if (pRemoteCharacteristic != nullptr)
    {   Serial.println(" - Found our characteristic");
        Serial.println(charUUID.toString().c_str());
    }
    else
    { Serial.print(" - Failed to find our characteristic UUID: ");
      Serial.println(charUUID.toString().c_str());  
      pClient->disconnect();
      return false;
    }

    Serial.print(" - BLE FreeHeap: ");    Serial.println(ESP.getFreeHeap());

    // Read the value of the characteristic.
    if(pRemoteCharacteristic->canRead()) 
    { 
      Serial.println(" - Remote characteristic value read: ");   
      
      RemoteServerReadCharacValue = pRemoteCharacteristic->readValue().c_str(); // value.c_str();        
      Serial.print(" - RemoteServerReadCharacValue: ");
      Serial.println(RemoteServerReadCharacValue);
    }

    if(pRemoteCharacteristic->canNotify())
      pRemoteCharacteristic->registerForNotify(notifyCallback);

    connected = true; 
       
    return true;
}

// Scan for BLE servers and find the first one that advertises the service we are looking for.
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks 
{
    void onResult(BLEAdvertisedDevice advertisedDevice) // Called for each advertising BLE server.
    { Server_BLE_Address = new BLEAddress(advertisedDevice.getAddress());  // static BLEAddress *    
      Scanned_BLE_Address = Server_BLE_Address->toString().c_str(); // String
      
      Serial.printf("Scan Result: BLE Advertised Device found: %s \n", advertisedDevice.toString().c_str());
//      String ScanString = advertisedDevice.toString().c_str(); // [scan_count++]
//      ScanData+=ScanString;  

      // We have found a device, let us now see if it contains the service we are looking for.
      if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) 
      { 
//        BLEDevice::getScan()->stop();
//        myDevice = new BLEAdvertisedDevice(advertisedDevice); // static BLEAdvertisedDevice* 
        Serial.println(" - haveServiceUUID() && isAdvertisingService(serviceUUID)");//      return true;
        Serial.println(serviceUUID.toString().c_str());
        doConnect = true;
        doScan = true;
      } // Found our server

      for(uint8_t i=0; i<KNOWN_TAGS_COUNT; i++)
      {  TagFoundStatus[i]=0;
      }
      Serial.println("\n\nCALLING SCAN_BLE_TAGS_BY_MAC_ADDR() from onResult()");
      SCAN_BLE_TAGS_BY_MAC_ADDR();
      
    } // onResult
}; // MyAdvertisedDeviceCallbacks



void BLE_setup()
{ BLEDevice::init("MAC_SCAN_CLIENT");
  pBLEScan = BLEDevice::getScan(); //create new scan // BLEScan*
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks()); //Call the class that is defined above
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(1349); // 1349
  pBLEScan->setWindow(499);  // 499 less or equal setInterval value 
//pBLEScan->start(scanTime, false);
}
  
void setup() 
{
  Serial.begin(115200); //(9600);

//  ESP_SoftSerial.begin(9600);     delay(500);
  
//  Serial.println("Scanning...");

  pinMode (LED_PIN,OUTPUT); //Declare the in-built LED pin as output
  pinMode (INDICATOR_PIN,OUTPUT); //Declare BUZZER PIN as output

  for(uint8_t i=0; i<KNOWN_TAGS_COUNT; i++)
  {  TagFoundStatus[i]=0; paired[i]=0;
  }
  
  BLE_setup();  
}

void loop() 
{ digitalWrite (INDICATOR_PIN, LOW); digitalWrite (LED_PIN, LOW);
  
//  btStart();  
  scanResultDevicesFound = pBLEScan->start(scanTime, false); // BLEScanResults 
  //Scan for 'scanTime' seconds to find the Advertising Tag
    Serial.println("Scan done!");
//  Serial.print("\nDevices found: ");  Serial.println(scanResultDevicesFound.getCount());  

//  Serial.println("\n\nScanData : "+ScanData+"\n");  
//  Serial.println("\nKNOWN_TAGS_COUNT : "+String(KNOWN_TAGS_COUNT)+"\n");
  


    int8_t best = CUTOFF;
    for (uint8_t i = 0; i < scanResultDevicesFound.getCount(); i++) 
    {
        BLEAdvertisedDevice device = scanResultDevicesFound.getDevice(i);
//        Serial.println("BLE device : "+String(device));
        int rssi = device.getRSSI();
        Serial.println("RSSI : "+String(rssi));
        if (rssi > best) {   best = rssi;  }
    }
    digitalWrite(LED_PIN, best > CUTOFF ? HIGH : LOW);
    
    
    for(uint8_t i=0; i<KNOWN_TAGS_COUNT; i++)
    { 
      if(!TagFoundStatus[i]) 
      {Status="0"; digitalWrite (INDICATOR_PIN, HIGH); digitalWrite (LED_PIN, HIGH);}
      else {Status="1";} 
                        
//      String str_Tx =String('T')+BLE_Tag_Addrs_to_Scan[i]+String('H')+Status+String('L')+String("500");
      String str_Tx =String('M')+BLE_Tag_Addrs_to_Scan[i]+String('S')+Status;
      Serial.println(str_Tx);
    }
  
  // "\"50:8c:b1:40:16:47\":\"1\", \"98:5d:ad:23:14:bd\":\"0\""
//  String str_Tx="\""+BLE_Tag_Addrs_to_Scan[0]+"\":\""+Status+"\""; // ;, \"98:5d:ad:23:14:bd\":\"0\""
  

//    ESP_SoftSerial.print(str_Tx);  //    ESP_SoftSerial.write(str_Tx);

      
        // If the flag "doConnect" is true then we have scanned for and found the desired
  // BLE Server with which we wish to connect.  Now we connect to it.  Once we are 
  // connected we set the connected flag to be true.
 /* if (doConnect == true) 
  {
    if (connectToServer(*Server_BLE_Address)) 
    {  Serial.println("\nWe are now connected to the BLE Server.");
    } 
    else 
    {  Serial.println("We have failed to connect to the server; there is nothin more we will do.");
    }
    doConnect = false;
  }

  // If we are connected to a peer BLE Server, update the characteristic each time we are reached
  // with the current time since boot.
  if (connected) 
  {
//    Serial.println("\nBLE CLIENT SUCCESSFULLY CONNECTED TO BLE SERVER!");
//   String newValue = "Time since boot: " + String(millis()/1000);
//    Serial.println("Setting new characteristic value to \"" + newValue + "\"");
    
//    // Set the characteristic's value to be the array of bytes that is actually a string.
//    pRemoteCharacteristic->writeValue(newValue.c_str(), newValue.length());
  }
  else if(doScan)
  {
    BLEDevice::getScan()->start(5, false);  // this is just eample to start scan after disconnect, most likely there is better way to do it in arduino
  }*/
  Serial.println("\n\nCALLING SCAN_BLE_TAGS_BY_MAC_ADDR() from loop()");
  SCAN_BLE_TAGS_BY_MAC_ADDR();   
//  SCAN_BLE_TAGS();
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
//  btStop();

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
