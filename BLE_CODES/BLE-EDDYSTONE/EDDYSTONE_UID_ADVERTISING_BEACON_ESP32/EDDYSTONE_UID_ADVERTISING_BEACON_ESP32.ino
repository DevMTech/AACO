// Copyright (c) 2019 Sho Kuroda <krdlab@gmail.com>
// 
// esp32-eddystone library
// 

#include <BLEDevice.h>
#include <BLEServer.h>
#include <Arduino.h>
#include "uid.hpp" // #include "eddystone/uid.hpp" // #include <eddystone.hpp>

const std::string NAMESPACE = "edd1ebeac04e5defa017"; // 10BYTES

//const std::string NAMESPACE = "AABBCCDDEEFF11223344"; // 10BYTES
const std::string INSTANCE  = "112233445566"; // 6BYTES

BLEAdvertising *pAdvertising; //  = pServer->getAdvertising();

void setBeacon(BLEAdvertising* pAdvertising) 
{
  EddystoneUid uid(NAMESPACE, INSTANCE);
  BLEAdvertisementData adData;
  uid.compose(adData);
  pAdvertising->setAdvertisementData(adData);
}

void setup() 
{
  Serial.begin(115200);
  while (!Serial) 
  {   delay(10);
  }
  
  BLEDevice::init("ESP32EDDYSTN");
  BLEServer *pServer = BLEDevice::createServer();
  pAdvertising = pServer->getAdvertising();
  setBeacon(pAdvertising);

  Serial.println("Starting to Advertise ...");
  pAdvertising->start();
  delay(1000);
//  pAdvertising->stop();
//  Serial.println("Advertising stopped.");
}

void loop() 
{
//  Serial.println("Advertising for 1 second ...");
//  pAdvertising->start();
//  delay(1000);
//  pAdvertising->stop();
//  Serial.println("Advertising stopped.");

  delay(1000);
}
