/* BLE SCAN & HTTP POST DO NOT WORK TOGETHER
 * USE BLE SCAN & SEND HTTP-POST DATA TSERIALLY TO ESP-01 
 * 
   Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
   Ported to Arduino ESP32 by Evandro Copercini

   Name: JDY-08, 
   Address: 50:8c:b1:40:16:47, 
   manufacturer data: 593588a0508cb1401647, 
   serviceUUID: 0000ffe0-0000-1000-8000-00805f9b34fb

   BATTERY STATUS
   DEVICE STATUS - SEND TO DH SERVER
*/

#include <WiFi.h>           
#include <HTTPClient.h>
      
//      #include <WiFiMulti.h>
//      WiFiMulti wifiMulti;


#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

BLEScan* pBLEScan;
BLEClient* pClient;
BLEAddress* pServerAddress;
int scanTime = 5; //In seconds
static uint8_t scan_count = 0;

String Status="0";
String ScanData="";
static int LED_PIN = 2;
static int INDICATOR_PIN = 15; //D15 GPIO15 BUZZER PIN
static bool FOUND = false;
    
const char* ssid = "cdac";          const char* password = "";

//"50:8c:b1:40:16:47"; // JDY-08 ZERO PCB 
//"c8:fd:19:4a:be:1d"; // JDY-08 COIN CELL // "98:5d:ad:23:14:bd";  "985dad2314bd" 
//"c8:fd:19:4a:f7:72"; // BLE3 (HM10) MAC (BLE_SERVER_TAG)
//"30:ae:a4:7a:dc:16"  // ESP32 DEVKITV1 BLK

String BLE_Tag_Addrs_to_Scan[] = {"c8:fd:19:4a:be:1d","c8:fd:19:4a:f7:72","30:ae:a4:7a:dc:16","50:8c:b1:40:16:47"}; 
static uint8_t KNOWN_TAGS_COUNT = sizeof(BLE_Tag_Addrs_to_Scan) / sizeof(BLE_Tag_Addrs_to_Scan[0]);   // BLE_Tag_Addrs_to_Scan[].length(); // not working

//String BLE_Tag_Addr_to_Scan;



class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks 
{   void onResult(BLEAdvertisedDevice advertisedDevice) 
    { String ScanString = advertisedDevice.toString().c_str(); // [scan_count++]
      pServerAddress = new BLEAddress(advertisedDevice.getAddress());
      ScanData+=ScanString;
      Serial.println("CHECK 0!");
      Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());      
    }
};

void BLE_setup()
{
  BLEDevice::init(""); // BLEScanPostV3
  pClient  = BLEDevice::createClient();
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100); // 100
  pBLEScan->setWindow(99);   // 99 less or equal setInterval value
}
  
void SCAN_BLE_TAGS()
{ Serial.println("\nStarting to Scan..."); 
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  Serial.println("CHECK 1!");
  Serial.print("\nDevices found: ");  Serial.println(foundDevices.getCount());  
  Serial.println("Scan done!");
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
  
  Serial.println("\n\nScanData : "+ScanData+"\n");  
//  Serial.println("\nKNOWN_TAGS_COUNT : "+String(KNOWN_TAGS_COUNT)+"\n");
}

void SEARCH_BLE_TAGS()
{
  FOUND = false; // false; 
  Status = "0";
  
  for(uint8_t i=0; i<KNOWN_TAGS_COUNT; i++)
  {
    if(ScanData.indexOf(BLE_Tag_Addrs_to_Scan[i]) > 0)
    {  Serial.println("\n\n "+BLE_Tag_Addrs_to_Scan[i]+" FOUND - DEVICE IN RANGE\n");
       FOUND = true; digitalWrite (INDICATOR_PIN, LOW); Status = "1";    //  break;    
    }
    else
    {  
//      Serial.println("\n\n "+BLE_Tag_Addrs_to_Scan[i]+" NOT FOUND - OUT OF RANGE!!\n");
//       FOUND = false;  Status = "0";
       digitalWrite (INDICATOR_PIN, HIGH); digitalWrite (LED_PIN, HIGH);   
    }
  }
       
//  if(FOUND) {digitalWrite (INDICATOR_PIN, LOW);  digitalWrite (LED_PIN, LOW);  Status = "1";}
//  else      {digitalWrite (INDICATOR_PIN, HIGH); digitalWrite (LED_PIN, HIGH); Status = "0";}  
}

void WiFi_setup()
{
  Serial.print("\n\nMAC : ");
  Serial.println(WiFi.macAddress());
  Serial.print("\nConnecting to ");
//  Serial.println(ssid); 
  
  WiFi.begin(ssid, password);
//  wifiMulti.addAP(ssid, password);
  
  int8_t c=0;
  while (WiFi.status() != WL_CONNECTED && c++<100) 
  { delay(10); //Serial.print("."); //  Serial.println("Connecting..");
  }
  
  if(WiFi.status() == WL_CONNECTED)
  {
    Serial.println("\nWiFi Connection to "+ String(ssid)+" established!");  
    Serial.print("\nIP address:\t");
    Serial.println(WiFi.localIP());  

//    deviceDescription="Antiquity Ambient Condition Observer";
//    deviceMAC=WiFi.macAddress();                //macToStr(mac);  //mac;
//    deviceIP=WiFi.localIP().toString().c_str();
    Serial.println("\n");
  }
  else
  {Serial.println("\n========================FAILED TO CONNECT TO WiFi==================\n");
  } 

  delay(3000);
}

void setup() 
{
  Serial.begin(115200);
//  Serial.println("Scanning...");

  pinMode (LED_PIN,OUTPUT); //Declare the in-built LED pin as output
  pinMode (INDICATOR_PIN,OUTPUT); //Declare BUZZER PIN as output
  
  WiFi_setup();

  BLE_setup();  
}

void loop() 
{ Serial.println("\n=======================LOOP STARTS==================\n");
//  btStart();
//  BLE_setup();  
  SCAN_BLE_TAGS();
//  btStop();
  SEARCH_BLE_TAGS();
  ScanData="";
    
  delay(2000);
  
//  WiFi.mode(WIFI_ON);
//  WiFi_setup();
  HTTP_POST_NOTIF();   
//  delay(1000);
//  WiFi.mode(WIFI_OFF); //  WiFi.mode(WIFI_MODE_NULL);

  Serial.println("\n=======================LOOP ENDS==================\n");
  
  delay(2000);
//  ESP.restart();
}

/*    bool known = false;
      for (i = 0; i < (sizeof(knownAddresses) / sizeof(knownAddresses[0])); i++) 
      { if (strcmp(pServerAddress->toString().c_str(), knownAddresses[i].c_str()) == 0)
        { known = true;
          break;
        }
      }
      if (known) 
      { if (Device.getRSSI()) 
        { deviceFound = true;
        }
        else 
        { deviceFound = false;
        }
        Device.getScan()->stop();

        String Name = (Device.toString().c_str());
        signed int rssi = (Device.getRSSI());
        //const char* mac = (knownAddresses[i].c_str());
        //String Mac = (knownAddresses[i].c_str());
        Serial.println();
        Serial.println("BLE Device found.");
        Serial.println(Name);
        //Serial.printf("MAC - ");
        //Serial.println(mac);
        //Serial.println(knownAddresses[i].c_str());
        Serial.printf("RSSI - ");
        Serial.println(rssi);
        
        dtostrf(rssi,5,3,Rssi);
        snprintf (msg, 64, data, Rssi);
        //snprintf (msg, 64, data, Rssi, mac);
        Serial.println(msg);
        //Serial.println(WiFi.macAddress());
        
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
digitalWrite(PIN, best > CUTOFF ? HIGH : 
LOW);
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

/*
   WiFiSTA.cpp
   
 * Start Wifi connection
 * if passphrase is set the most secure supported mode will be automatically selected
 * @param ssid const char*          Pointer to the SSID string.
 * @param passphrase const char *   Optional. Passphrase. Valid characters in a passphrase must be between ASCII 32-126 (decimal).
 * @param bssid uint8_t[6]          Optional. BSSID / MAC of AP
 * @param channel                   Optional. Channel of AP
 * @param connect                   Optional. call connect
 * @return
 
wl_status_t WiFiSTAClass::begin(const char* ssid, const char *passphrase, int32_t channel, const uint8_t* bssid, bool connect)

    WiFi.config(local_IP_STA, gateway_STA, subnet_STA, primaryDNS)

    IPAddress primaryDNS(8, 8, 8, 8);
       IPAddress secondaryDNS(8, 8, 4, 4);
       WiFi.begin(x[0].c_str(),x[1].c_str());
       delay(50);
       if(!WiFi.config(local_IP_STA, gateway_STA, subnet_STA))
       Serial.println("Couldn't configure STATIC IP ! Starting DHCP IP !");
       delay(50);
    
*/
