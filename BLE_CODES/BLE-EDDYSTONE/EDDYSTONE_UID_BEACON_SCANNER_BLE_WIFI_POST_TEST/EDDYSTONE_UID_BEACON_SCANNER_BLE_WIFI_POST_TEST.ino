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


#include <WiFi.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>
//#include <ESP32Ping.h> // https://github.com/marian-craciunescu/ESP32Ping

int httpCode;
String deviceName = "", deviceDescription, deviceMAC, deviceIP, deviceConfigParams;
String WiFi_Status = "DISCONNECTED";
String HTTP_post_status = "FAILURE";
static uint8_t POST_EVERY_x_mS = 2000; // TO SEND POST NOTIFICATIONS AFTER EVERY 2 SECONDS
int LED2 = 2; //GPIO2 - ESP32
unsigned long totalPwrOnDuration = 0, WiFiConnAttemptDuration = 0, WiFiOnDuration = 0, WiFiOffDuration = 0, OTADuration = 0;
//RTC_DATA_ATTR long int bootCount = 0;
RTC_DATA_ATTR long int WiFiConnRetryAttempt = 0;


//String deviceMAC; // , deviceName = "", deviceDescription, deviceIP, deviceConfigParams;

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

//  BLEDevice::init("ESP_EDDY_SCAN1");
//  scanner = BLEDevice::getScan(); // singleton object
//  scanner->setAdvertisedDeviceCallbacks(new AdvertisedDeviceHandler());

//  Serial.printf("> Start scanning...(%d sec)\n", SCAN_TIME);
//  BLEScanResults devices = scanner->start(SCAN_TIME);
//  Serial.printf("> Devices found: %d\n", devices.getCount());
  Serial.println("*************************************************************************************************");
}

void loop() 
{ 
  BLEDevice::init("ESP_EDDY_SCAN1");
  scanner = BLEDevice::getScan(); // singleton object
  scanner->setAdvertisedDeviceCallbacks(new AdvertisedDeviceHandler());

  //btStart();
  
  unsigned int scanTime = millis();
  Serial.printf("> Start scanning...(%d sec)\n", SCAN_TIME);
  BLEScanResults devices = scanner->start(SCAN_TIME);
  scanTime = millis()-scanTime;
  Serial.printf("> Devices found: %d\n", devices.getCount());
  Serial.printf(" ScanTime = %u millisec.\n", scanTime);
  
  //btStop();
  
  Serial.println("\n*************************************************************************************************");

  //WiFi_ON();
  HTTP_POST_NOTIF();  //  delay(1500);
  WiFi_OFF();
  
  delay(5000);

  // ESP.restart();
}
