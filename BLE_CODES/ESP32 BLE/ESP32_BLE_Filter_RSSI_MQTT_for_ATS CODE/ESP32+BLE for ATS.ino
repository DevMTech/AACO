#include "BLEDevice.h"
#include "BLEUtils.h"
#include "BLEScan.h"
#include "BLEAdvertisedDevice.h"
#include <WiFi.h>

static BLEAddress *pServerAddress;
BLEScan* pBLEScan;
BLEClient*  pClient;
bool deviceFound = false;

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
     
      for (i = 0; i < (sizeof(knownAddresses) / sizeof(knownAddresses[0])); i++) {
        if (strcmp(pServerAddress->toString().c_str(), knownAddresses[i].c_str()) == 0) 
        {
          known = true;
          break;
        }
      }
      if (known) {
        Serial.println("BLE Device found.");
        Serial.println(Device.toString().c_str());
        Serial.printf("MAC = "); 
        Serial.println(knownAddresses[i].c_str());
        Serial.printf("RSSI = ");
        Serial.println(Device.getRSSI());
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
