/* 
    Video: https://www.youtube.com/watch?v=oCMOYS71NIU
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updated by chegewara

   Create a BLE server that, once we receive a connection, will send periodic notifications.
   The service advertises itself as: 4fafc201-1fb5-459e-8fcc-c5c9c331914b
   And has a characteristic of: beb5483e-36e1-4688-b7f5-ea07361b26a8

   The design of creating the BLE server is:
   1. Create a BLE Server
   2. Create a BLE Service
   3. Create a BLE Characteristic on the Service
   4. Create a BLE Descriptor on the characteristic
   5. Start the service.
   6. Start advertising.

   A connect hander associated with the server starts a background task that performs notification
   every couple of seconds.

   0x02010611074B9131C3C9C5CC8F9E45B51F01C2AF4F0512120040000201061209424C4533324E6F74696679536572766572020A03051212004000
02 01 06 
11 07 4B9131C3C9C5CC8F9E45B51F01C2AF4F  // SERVICE UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
05 12 12004000

02 01 06 
12 09 424C4533324E6F74696679536572766572 // BLE32NotifyServer
02 0A 03 
05 12 12004000
*/

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h> /*0x2902 - GATT Descriptor - Client Characteristic Configuration  
                    org.bluetooth.descriptor.gatt.client_characteristic_configuration*/

BLEServer*          pServer         = NULL;
BLEService*         pService        = NULL;
BLECharacteristic*  pCharacteristic = NULL;
//BLEDescriptor*      pDescriptor     = NULL;
BLE2902*            p2902Descriptor = NULL;

bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

//#define SERVICE_UUID        "c2460b65-913b-45c8-8ad4-47267dae04c8" // "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
//#define CHARACTERISTIC_UUID "0f00d30c-0ffa-4bc2-ae09-1a0694cacea7" // "beb5483e-36e1-4688-b7f5-ea07361b26a8"

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};


void BLE_INIT()
{
    // Create the BLE Device
  BLEDevice::init("BLE32NotifyServer");

  
  pServer = BLEDevice::createServer(); // Create the BLE Server
  pServer->setCallbacks(new MyServerCallbacks());

  // Creating a CUSTOM SERVICE, CHARCTERISTIC & DESCRIPTOR (HAVING CUSTOM DATA)
  pService = pServer->createService(SERVICE_UUID); // Create the BLE Service // BLEService * 

  pCharacteristic = pService->createCharacteristic( // Create a BLE Characteristic
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );

// https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml

   /*0x2902 - GATT Descriptor - Client Characteristic Configuration*/
//  BLE2902* p2902Descriptor = new BLE2902(); // Create a BLE Descriptor
  p2902Descriptor = new BLE2902(); // Create a BLE Descriptor
  p2902Descriptor->setValue("T,H,L");
  p2902Descriptor->setNotifications(true);
  pCharacteristic->addDescriptor(p2902Descriptor);
  
//  pDescriptor->setValue("T,H,L");
//  pCharacteristic->addDescriptor(&pDescriptor);  
//  pCharacteristic->addDescriptor(new BLE2902()); // Create a BLE Descriptor

  pService->start(); //Start the BLE GATT Service 

  BLEAdvertising *pAdvertising = pServer->getAdvertising();   // Start advertising
  pAdvertising->addServiceUUID(SERVICE_UUID); //  pServer->getAdvertising()->addServiceUUID(pService);
  pAdvertising->setScanResponse(true);
  // Peripheral preferred Connection parameters - Advt. Intervals
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMaxPreferred(0x12);
  pAdvertising->start(); // Start advertising

//  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
//  pAdvertising->addServiceUUID(SERVICE_UUID);
//  pAdvertising->setScanResponse(false);
//  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
//  BLEDevice::startAdvertising();

  Serial.println("Characteristic defined! Now you can read it in your phone!");
  Serial.println("Waiting for a client connection to notify...");
}

void setup() {
  Serial.begin(115200);

  BLE_INIT();  
  
  delay(200);
  
}
/*
void loop()
{ 
//  static uint8_t value = 0;
//  pCharacteristic->setValue((uint8_t*)&value++, 1); //    value++;

  Serial.println("T20.00H50.00L1000.00E");
  pCharacteristic->setValue("T20.00H50.00L1000.00E");
  
  pCharacteristic->notify();
  delay(2000);
//  DEEPSLEEP_setup(5); // duration in seconds
}*/

void loop() 
{   if (deviceConnected) // notify changed value
    {   Serial.println("\nDevice Connected ...!!");   
//        pCharacteristic->setValue((uint8_t*)&value, 4); value++;

        Serial.println("T20.00H50.00L1000.00E - A,B,C");
        pCharacteristic->setValue("T20.00H50.00L1000.00E");
        pCharacteristic->notify();
        p2902Descriptor->setValue("A,B,C");
        
        delay(2000);
        
        Serial.println("T30.00H60.00L500.00E - X,Y,Z");
        pCharacteristic->setValue("T30.00H60.00L500.00E");
        pCharacteristic->notify();
        p2902Descriptor->setValue("X,Y,Z");
        
        delay(1000);  // delay(3); // bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
    }
    
    if (!deviceConnected && oldDeviceConnected) // disconnecting
    {   Serial.println("\nDevice Disconnecting ...!!");
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    
    if (deviceConnected && !oldDeviceConnected)  // connecting
    {   // do stuff here on connecting
        Serial.println("\nDevice Connecting ...!!");
        Serial.println("T10.00H40.00L1500.00E - P,Q,R");
        pCharacteristic->setValue("T10.00H40.00L1500.00E");        
        p2902Descriptor->setValue("P,Q,R");
        delay(500);
        oldDeviceConnected = deviceConnected;
    }
    delay(2000);

//    DEEPSLEEP_setup(5); // duration in seconds
}
