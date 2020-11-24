/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updates by chegewara

    https://github.com/espressif/arduino-esp32/blob/master/libraries/BLE/examples/BLE_server/BLE_server.ino
    
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h> 

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

//#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b" // "5f0ab730-e234-4438-b01e-d8f87a84a8b9" // 
//#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

#define SERVICE_UUID        "0000ffe0-0000-1000-8000-00805f9b34fb" // Unknown Service 
#define CHARACTERISTIC_UUID "0000ffe1-0000-1000-8000-00805f9b34fb" // Unknown Characteristic

BLEServer* pServer = NULL;
BLEService *pService = NULL;
BLECharacteristic *pCharacteristic = NULL;
BLEDescriptor* pDescriptor = NULL;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting ESP32 BLE SERVER!");

  BLEDevice::init("ESP32 BLE SERVER"); // Long name works now
  
  pServer = BLEDevice::createServer(); // BLEServer *
  pService = pServer->createService(SERVICE_UUID); // BLEService *
  pCharacteristic = pService->createCharacteristic(   //BLECharacteristic *
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ    | BLECharacteristic::PROPERTY_WRITE
                                      |  BLECharacteristic::PROPERTY_NOTIFY  | BLECharacteristic::PROPERTY_INDICATE                                           
//                                      |  BLECharacteristic::PROPERTY_BROADCAST
                                       );

  /*  BLEUUID("0d563a58-196a-48ce-ace2-dfec78acc814"),
   *   
      BLECharacteristic::PROPERTY_BROADCAST | BLECharacteristic::PROPERTY_READ  |
      BLECharacteristic::PROPERTY_NOTIFY    | BLECharacteristic::PROPERTY_WRITE |
      BLECharacteristic::PROPERTY_INDICATE
  */                                     

  pCharacteristic->setValue("T20.00H50.00L1000.00E");

  /*
    BLE2902* p2902Descriptor = new BLE2902();
    p2902Descriptor->setNotifications(true);
    pCharacteristic->addDescriptor(p2902Descriptor);
  */
  
  pService->start();
  
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  // pAdvertising->addServiceUUID(BLEUUID(pService->getUUID()));
  // pAdvertising->addServiceUUID(SERVICE_UUID); // THIS OR PREVIOUS LINE NOT BOTH REQUIRED
  // pAdvertising->start();
    
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  
  pAdvertising->setScanResponse(true);
  // Peripheral preferred Connection parameters - Advt. Intervals
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");
  delay(2000);
  DEEPSLEEP_setup();
}

void loop() {
  // put your main code here, to run repeatedly:
//  pCharacteristic->setValue("T20.00H50.00L1000.00E");
//  delay(2000);
}
