// Copyright (c) 2019 Sho Kuroda <krdlab@gmail.com>
// 
// esp32-eddystone library
// 

//#ifdef ESP32
//  #include <WiFi.h>
//#endif

#include <Arduino.h>
#include "uid.hpp" // #include "eddystone/uid.hpp" // #include <eddystone.hpp>
#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

String deviceMAC; // , deviceName = "", deviceDescription, deviceIP, deviceConfigParams;

BLEScan* scanner; //  = BLEDevice::getScan(); // singleton object

int SCAN_TIME = 5; // scanning time (sec) // const 

class AdvertisedDeviceHandler : public BLEAdvertisedDeviceCallbacks 
{
private:
  const std::string NAMESPACE = "AABBCCDDEEFF11223344";
  const std::string INSTANCE  = "112233445566";
public:
  void onResult(BLEAdvertisedDevice device) 
  {
    bool found = false;
    if (EddystoneUid::checkPayload(device.getPayload())) 
    {
      EddystoneUid uid(device.getPayload());
      
      if (uid.has(NAMESPACE, INSTANCE)) 
      {
        Serial.printf("> Target beacon found.\n");
        Serial.printf("  Address: %s\n", device.getAddress().toString().c_str());
        found = true;
      }
    }
    if (!found) 
    {
      Serial.printf("> %s\n", device.toString().c_str());
    }
  }
};

void setup() 
{
  Serial.begin(115200);
  while (!Serial) 
  {
    delay(10);
  }

//  deviceMAC = WiFi.macAddress(); //macToStr(mac);  //mac;
//  Serial.print("\n\nMAC : "); // 
//  Serial.println(deviceMAC); // (WiFi.macAddress());

  BLEDevice::init("ESP_EDDY_SCAN1");
  scanner = BLEDevice::getScan(); // singleton object
  scanner->setAdvertisedDeviceCallbacks(new AdvertisedDeviceHandler());

//  Serial.printf("> Start scanning...(%d sec)\n", SCAN_TIME);
//  BLEScanResults devices = scanner->start(SCAN_TIME);
//  Serial.printf("> Devices found: %d\n", devices.getCount());
  Serial.println("*************************************************************************************************");
}

void loop() 
{ unsigned int scanTime = millis();
  Serial.printf("> Start scanning...(%d sec)\n", SCAN_TIME);
  BLEScanResults devices = scanner->start(SCAN_TIME);
  scanTime = millis()-scanTime;
  Serial.printf("> Devices found: %d\n", devices.getCount());
  Serial.printf(" ScanTime = %u millisec.\n", scanTime);
  Serial.println("\n*************************************************************************************************");
  delay(5000);
}
