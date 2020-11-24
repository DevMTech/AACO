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

//#define SERVICE_UUID        "5070eae3-05ae-4af4-8f93-6dac9c4cf5c7"
//#define CHARACTERISTIC_UUID "f3e75829-1ce3-44ec-b7d4-46445313043e"

#define BAUD_RATE 115200 //static const uint8_t
//static const uint8_t BAUD_RATE = 115200; //

//------------------ COMMENT/UNCOMMENT FOR I2C BASED ON MCU CHOSEN -----------------//
 
#include <Wire.h>
//   ESP32           SDA-21, SCL-22
static const uint8_t SDA_PIN = 21; // ESP32
static const uint8_t SCL_PIN = 22; // ESP32

//   ESP12E-NODEMCU  SDA-4-D2, SCL-5-D1 
//static const uint8_t SDA_PIN = 4; // ESP12E-NODEMCU
//static const uint8_t SCL_PIN = 5; // ESP12E-NODEMCU

//   ESP8266 ESP01   SDA-0, SCL-2
//static const uint8_t SDA_PIN = 0; // ESP01
//static const uint8_t SCL_PIN = 2; // ESP01
//static const uint8_t TX0_PIN = 1; // ESP01
//static const uint8_t RX0_PIN = 3; // ESP01

//   ARDUINO         SDA-5, SCL-4
//static const uint8_t SDA_PIN = 5; // ESP01
//static const uint8_t SCL_PIN = 4; // ESP01

//------------------ COMMENT/UNCOMMENT FOR I2C BASED ON MCU CHOSEN -----------------//
/******************************************SENSOR PARAMETERS***************************************************/

float tempC = 0, tempF = 0, pressure = 0, alti = 0, humidity = 0;
double lux; // Resulting lux value
boolean good; // True if neither sensor is saturated
String deviceName="", deviceDescription, deviceMAC, deviceIP, deviceConfigParams;

/******************************************SENSOR PARAMETERS***************************************************/
/******************************************BATTERY PARAMETERS***************************************************/

//float R2 = 10.0, R1 = 46.4;
float R2 = 12.0, R1 = 33.0; // 
int BAT = A0;              //Analog channel A0 as used to measure battery voltage
unsigned int raw=0;
float volt=0.0;
// RatioFactor = (R1 + R2) / R2 : CALCUATE USING ABOVE FORMULAE AND MODIFY VALUE OF RatioFactor AS NECESSARY
float RatioFactor=(R1 + R2) / R2; //3.75 // 5.8125; // 5.714;  //Resistors Ration Factor
/******************************************BATTERY PARAMETERS***************************************************/

void VOLT_loop()
{
  int value = LOW;
  float Tvoltage=0.0;
  float Vvalue=0.0, Rvalue=0.0;

//  raw = analogRead(A0);
//  Serial.print("\nraw: ");
//  Serial.println(raw);
//  volt = RatioFactor*3.3*raw/1023.0; 
//  Serial.print("\nBattery Volt: ");
//  Serial.println(volt);
//  Serial.println();


  for(unsigned int i=0;i<10;i++)
  {
    Vvalue=Vvalue+analogRead(BAT);         //Read analog Voltage
    delay(5);                              //ADC stable
//    Serial.println(analogRead(BAT));
  }
  Vvalue=(float)Vvalue/10.0;            //Find average of 10 values
  Rvalue=(float)(Vvalue/1023.0)*3.3;      //Convert Voltage in 5v factor  
//  Rvalue=(float)(Vvalue/1024.0);

  Tvoltage=Rvalue*RatioFactor;          //Find original voltage by multiplying with factor
  Serial.print("\nBattery Voltage = ");
  Serial.print(Tvoltage);
  Serial.println(" V\n");

  volt = Tvoltage;
  Serial.print("\nBattery Volt: ");
  Serial.println(volt);
  Serial.println();
 
}
/******************************************BATTERY MONITOR***************************************************/

void I2C_setup()
{
    pinMode(SDA_PIN,INPUT_PULLUP);
    pinMode(SCL_PIN,INPUT_PULLUP);
//    pinMode(TX0_PIN,INPUT_PULLUP);
//    pinMode(RX0_PIN,INPUT_PULLUP);

 Wire.begin(SDA_PIN,SCL_PIN);
//  Wire.begin(0,2);    //   ESP8266 ESP01   SDA-0, SCL-2
//  Wire.begin(4,5);    //   ESP12E-NODEMCU  SDA-D2-4, SCL-D1-5
//  Wire.begin(21,22);  //   ESP32           SDA-21, SCL-22
//  Wire.begin(5,4);    //   ARDUINO         SDA-5, SCL-4

    bool found = I2C_SCANNER_loop();
    if(!found)  I2C_SCANNER_loop();
}


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
                        BLECharacteristic::PROPERTY_READ   
                      | BLECharacteristic::PROPERTY_WRITE  
                      | BLECharacteristic::PROPERTY_NOTIFY 
//                      | BLECharacteristic::PROPERTY_INDICATE
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

void setup() 
{
  Serial.begin(BAUD_RATE);       //  Serial.flush();   
  delay(1000);
  Serial.println("\n\n*********************START UP CODE***********************\n\n");
//    Serial.printf("BAUD_RATE : %d \n"+BAUD_RATE); 
  Serial.println("BAUD_RATE : "+String(BAUD_RATE));
  
  pinMode(A0, INPUT);
  Serial.println("\nBattery Volt> RatioFactor : "+String(RatioFactor));
  
  I2C_setup();

  BME_setup();
  LUX_setup();

  BLE_INIT();  
  
  delay(200);
  
}

void loop() 
{   
  String BLE_data = ""; // NULL; //    char* BLE_data = "";
    bool flag = false;
//    VOLT_loop();    
    volt = 3.36; 
    
    BME_loop();
    LUX_loop();
//    printValues();
    
//    if(tempC && humidity && lux) 
    {    BLE_data = "T"+String(tempC)+"H"+String(humidity)+"L"+String(lux)+"V"+String(volt);
        Serial.println("BLE_data "+BLE_data);
        flag = true;
    }
//    else
//    { 
//      ESP.restart();
//      BME_setup();
//      LUX_setup();
//    }    
    
    if (deviceConnected) // notify changed value
    { Serial.println("\nDevice Connected ...!!");           
      if(flag)
      { 
        /*   https://github.com/nkolban/esp32-snippets/issues/79
         *   https://stackoverflow.com/questions/1662107/whats-the-difference-between-stdstring-and-stdbasic-string-and-why-are-bot
         *   https://en.cppreference.com/w/cpp/string/basic_string
         *   http://www.cplusplus.com/reference/string/basic_string/
        std::basic_string is a class template for making strings out of character types, 
        std::string is a typedef for a specialization of that class template for char. 
        Yes they are both needed (or at least required by the standard).*/
        std::string BLE_msg((char*)&BLE_data, BLE_data.length()); // (uint64_t*) (char*)
        Serial.print("\nBLE_msg ");      Serial.println(BLE_msg.c_str()); // toString().
        
        pCharacteristic->setValue(BLE_msg);
//        pCharacteristic->setValue("T28.38H48.36L471.80V3.36"); // DUMMY STRING FOR TESTING
        pCharacteristic->notify();  // pCharacteristic->notify(true);
        p2902Descriptor->setNotifications(true);
//        p2902Descriptor->setValue("SENSOR VALUES"); // ("A,B,C");

        delay(2000);

        pCharacteristic->setValue("T15.38H53.36L21.80V3.54"); // DUMMY STRING FOR TESTING
        pCharacteristic->notify();
        p2902Descriptor->setNotifications(true);

        
//        p2902Descriptor = new BLE2902(); // Create a BLE Descriptor
//        p2902Descriptor->setValue(BLE_msg);
//        p2902Descriptor->setValue("T15.38H53.36L521.80V3.54");
//        p2902Descriptor->setNotifications(true);
//        pCharacteristic->addDescriptor(p2902Descriptor);
        

        flag = false;
        delay(2000);

          
      }
//        Serial.println("T30.00H60.00L500.00E - X,Y,Z");
//        pCharacteristic->setValue("T30.00H60.00L500.00E");
//        pCharacteristic->notify();
//        p2902Descriptor->setValue("X,Y,Z");

//        pCharacteristic->setValue((uint8_t*)&value, 4); value++;    
//        Serial.println("T20.00H50.00L1000.00E - A,B,C"); 
       
//        delay(1000);  // delay(3); // bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
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
