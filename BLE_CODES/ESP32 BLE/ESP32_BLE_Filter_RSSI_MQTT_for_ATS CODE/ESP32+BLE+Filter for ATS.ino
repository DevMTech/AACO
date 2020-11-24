#include "BLEDevice.h"
#include "BLEUtils.h"
#include "BLEScan.h"
#include "BLEAdvertisedDevice.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include "mqtt_client.h"

// ==================MQTT Setup ================================

// Update these with values suitable for your network.
const char* ssid = "CdacIoT";      const char* password = "cdac1234";
const char* mqtt_server = "10.42.0.1";

//const char* ssid = "cdac";      const char* password = "";
//const char* mqtt_server = "10.208.35.169";

//const char* ssid = "Xperia";      const char* password = "Hello1234";
//const char* mqtt_server = "10.208.35.169";

const char* SUB_TOPIC = "ATS/In";
const char* PUB_TOPIC = "ATS/ESP";
char* data = "{\"Mac\":%s,\"RSSI\":%s}";
char msg[30];
char MAC[8] ={'\0'};
char RSSI[8] = {'\0'};

WiFiClient espClient;
PubSubClient client(espClient);

//===============================================================

static BLEAddress *pServerAddress;
BLEScan* pBLEScan;
BLEClient*  pClient;
bool deviceFound = false;

String Name;
String Mac;
signed int rssi;

int scanTime = 5; //In seconds
int i;

String knownAddresses[] = {"c8:fd:19:9c:85:98","c8:fd:19:4a:f7:72","c8:fd:19:4a:f7:46"};
//String knownAddresses[] = {"BLE1:c8:fd:19:9c:85:98","BLE2:c8:fd:19:4a:f7:46","BLE3:c8:fd:19:4a:f7:72"};

unsigned long entry;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice Device){
         
      pServerAddress = new BLEAddress(Device.getAddress()); 
      bool known = false;
      bool Master = false;
      String Name = (Device.toString().c_str());
      long int rssi = (Device.getRSSI());
      String Mac = (knownAddresses[i].c_str());
      
      for (i = 0; i < (sizeof(knownAddresses) / sizeof(knownAddresses[0])); i++) {
        if (strcmp(pServerAddress->toString().c_str(), knownAddresses[i].c_str()) == 0) 
        {
          known = true;
          break;
        }
      }
      if (known) {
        Serial.println("BLE Device found.");
        Serial.println(Name);
        Serial.printf("MAC : "); 
        Serial.println(Mac);
        Serial.printf("RSSI : ");
        Serial.println(rssi);
        Serial.println();
       
        if (Device.getRSSI()) {
          deviceFound = true;
        }
        else {
          deviceFound = false;
        }
        Device.getScan()->stop();
      }
    }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Scanning...");
  
  BLEDevice::init("");
  pClient  = BLEDevice::createClient();
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  Serial.println("Done");
}

void loop() { 

  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
  delay(500);
}

