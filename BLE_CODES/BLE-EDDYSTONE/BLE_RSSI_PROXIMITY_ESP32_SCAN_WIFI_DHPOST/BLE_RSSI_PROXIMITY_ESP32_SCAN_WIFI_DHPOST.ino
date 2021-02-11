/*
  https://circuitdigest.com/microcontroller-projects/ble-based-proximity-control-using-esp32
  https://github.com/espressif/arduino-esp32/tree/master/libraries/BLE/src

  https://circuitdigest.com/microcontroller-projects/esp32-ble-server-how-to-use-gatt-services-for-battery-level-indication
  https://circuitdigest.com/microcontroller-projects/esp32-ble-client-connecting-to-fitness-band-to-trigger-light
  https://circuitdigest.com/microcontroller-projects/esp32-based-bluetooth-ibeacon


*/

// #include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>
//#include <ESP32Ping.h> // https://github.com/marian-craciunescu/ESP32Ping
//#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

enum BLE_TAG_STATES {MISSING, FOUND};


//#define TEST_MODE

//#define DEBUG_ALL_TAGS
#define DEBUG_KNOWN_TAGS

#define LED_BUILTIN 2
#define ARRSIZE(x) (sizeof(x)/sizeof(x[0]))
#define ENDIAN_CHANGE_U16(x) ((((x)&0xFF00)>>8) + (((x)&0xFF)<<8))

int httpCode;
String deviceName = "", deviceDescription, deviceMAC, deviceIP, deviceConfigParams;
String WiFi_Status = "DISCONNECTED";
String HTTP_post_status = "FAILURE";
static uint8_t POST_EVERY_x_mS = 2000; // TO SEND POST NOTIFICATIONS AFTER EVERY 2 SECONDS
int LED2 = 2; //GPIO2 - ESP32
unsigned long totalPwrOnDuration = 0, WiFiConnAttemptDuration = 0, WiFiOnDuration = 0, WiFiOffDuration = 0, OTADuration = 0;
//RTC_DATA_ATTR long int bootCount = 0;
RTC_DATA_ATTR long int WiFiConnRetryAttempt = 0;


#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

BLEScan *pBLEScan;

String knownBLEAddresses[] = { "d4:36:39:c2:28:3c" // JDY-08
                              ,"d4:36:39:c2:0f:61" // PCB-JDY-08
                              ,"2c:6b:7d:74:ca:87" // PCB-HM10-MLT-BT05 
                              ,"24:0a:c4:83:20:c2" // ESP32-EDDYSTN
                              ,"c8:fd:19:4a:f7:27" // MLT-BT05
                              ,"c8:e7:ec:39:b8:d5" // ESTIMOTE
                              ,"d4:36:39:b1:54:f1" // HM-10 iBEACON / JDY-08
                             };
                                // ,":::::" // DUMMY
                                
bool device_found[ARRSIZE(knownBLEAddresses)] = {0};
int tag_states[ARRSIZE(knownBLEAddresses)] = {0};
String found_BLE_MAC_list;

int RSSI_THRESHOLD = -75;
int scanTime = 5; //In seconds

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks
{
    void onResult(BLEAdvertisedDevice advertisedDevice)
    {
      const char *foundBLEAdvertisedDevice = advertisedDevice.toString().c_str();
      std::string foundBLEaddr = advertisedDevice.getAddress().toString();
      std::string foundBLEname = advertisedDevice.getName();
      const char *foundBLEMfgData = advertisedDevice.getManufacturerData().c_str();
      const char *foundBLEServiceData = advertisedDevice.getServiceData().c_str();

      //std::string foundBLEAdvertisedDevice = advertisedDevice.toString();


      //std::string foundBLEMfgData = advertisedDevice.getManufacturerData();
      //std::string foundBLEServiceData = advertisedDevice.getServiceData();

      int foundBLErssi = advertisedDevice.getRSSI();
      int foundBLETXPower = advertisedDevice.getTXPower();
      //int foundBLEServiceDataCount = advertisedDevice.getServiceDataCount();

      BLEScan* foundBLEgetScan = advertisedDevice.getScan();

#ifdef DEBUG_ALL_TAGS
      Serial.printf("\n--------------------------------DEBUG_ALL_TAGS------------------------------------\n");
      //Serial.printf("Advertised Device: %s \n", foundBLEAdvertisedDevice.c_str());
      Serial.printf("\nAdvertised Device Data: %s \n", advertisedDevice.toString().c_str());
      //Serial.printf("Advertised Device 2: %s \n", foundBLEAdvertisedDevice);

      //Serial.printf("\n BLEaddr = %s", foundBLEaddr.c_str());
      //Serial.printf("\n BLEname = %s", foundBLEname.c_str());
      //Serial.printf("\n RSSI = %d", foundBLErssi);
      //Serial.printf("\n TxPower = %d \n", foundBLETXPower);
      //Serial.printf("\n MfgData = %s", foundBLEMfgData);
      //Serial.printf("\n ServiceData = %s", foundBLEServiceData);

#endif

      //puts(foundBLEMfgData);
      //for (int i = 0; i < (sizeof(knownBLEAddresses) / sizeof(knownBLEAddresses[0])); i++)
      for (int i = 0; i < ARRSIZE(knownBLEAddresses); i++)
      {
        //char *s2 = knownBLEAddresses[i];//.c_str();
        //if (strcmp(advertisedDevice.getAddress().toString().c_str(), knownBLEAddresses[i].c_str()) == 0)
        if (!strcmp(foundBLEaddr.c_str(), knownBLEAddresses[i].c_str()))
        { device_found[i] = true;

#ifdef DEBUG_KNOWN_TAGS
          Serial.printf("\n--------------------------------DEBUG_KNOWN_TAGS------------------------------------\n");
          //Serial.printf("Advertised Device: %s \n", foundBLEAdvertisedDevice.c_str());
          Serial.printf("Advertised Device Data: %s \n", advertisedDevice.toString().c_str());
          //Serial.printf("Advertised Device 2: %s \n", foundBLEAdvertisedDevice);

          Serial.printf("\n BLEaddr = %s", foundBLEaddr.c_str());
          Serial.printf("\n BLEname = %s", foundBLEname.c_str());
          Serial.printf("\n RSSI = %d", foundBLErssi);
          Serial.printf("\n TxPower = %d \n", foundBLETXPower);
          //Serial.printf("\n MfgData = %s", foundBLEMfgData);
          //Serial.printf("\n ServiceData = %s", foundBLEServiceData);



          //          Serial.printf("\n MfgData = %s", foundBLEMfgData.c_str());
          //          Serial.printf("\n ServiceData = %s", foundBLEServiceData.c_str());

#endif

          //break;
          Serial.printf("\n----------------------------------------2-------------------------------------------\n");
        }
        //else          device_found[i] = false;
      }
    }
};

void setup()
{
  Serial.begin(115200); //Enable UART on ESP32
  Serial.println("Scanning..."); // Print Scanning
  pinMode(LED_BUILTIN, OUTPUT); //make BUILTIN_LED pin as output

#ifndef TEST_MODE
  Serial.printf("\n--------------------------------SCAN MODE------------------------------------\n");
  BLEDevice::init("ESP32_SCAN_TEST");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks()); //Init Callback Function
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100); // set Scan interval
  pBLEScan->setWindow(99);  // less or equal setInterval value

#endif
  
}

//When a scan completes, we have a set of found devices.
//Each device is described by a BLEAdvertisedDevice object.
//The number of items in the set is given by getCount().
//We can retrieve a device by calling getDevice() passing in
//the index (starting at 0) of the desired device.

void loop()
{ Serial.printf("\n\n----------------------------------------1-------------------------------------------\n\n");

#ifndef TEST_MODE
  Serial.printf("\n--------------------------------SCAN MODE------------------------------------\n");
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  for (int i = 0; i < foundDevices.getCount(); i++)
  {
    BLEAdvertisedDevice device = foundDevices.getDevice(i);
    int rssi = device.getRSSI();

    //Serial.println(rssi);
    if (rssi > RSSI_THRESHOLD) //  && device_found[i] == true)
    { Serial.printf("RSSI: %d dBm\n", rssi);
      digitalWrite(LED_BUILTIN, HIGH);
    }
    else
      digitalWrite(LED_BUILTIN, LOW);
  }

  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory

  int flag = 0;
  String state = "";
  for (int i = 0; i < ARRSIZE(knownBLEAddresses); i++)
  {
    if (tag_states[i] != device_found[i])
    {
      if (device_found[i])
      { state = "1"; // "FOUND";
      }
      else
      { state = "0"; // "MISSING";
      }
      tag_states[i] = device_found[i];

      // [{"mac":"d4:36:39:c2:28:3c","state":1},{"mac":"24:0a:c4:83:20:c2","state":1},{"mac":"c8:fd:19:4a:f7:27","state":1}]

      if (flag) // 2ND MAC ADDRESS ONWARDS
      { found_BLE_MAC_list += ",{\"mac\":\"";
        found_BLE_MAC_list += knownBLEAddresses[i];
        found_BLE_MAC_list += "\"";
      }
      else // 1ST MAC ADDRESS
      { found_BLE_MAC_list += "{\"mac\":\"";
        found_BLE_MAC_list += knownBLEAddresses[i];
        found_BLE_MAC_list += "\"";
      }

      found_BLE_MAC_list += ",\"state\":";
      found_BLE_MAC_list += state;
      found_BLE_MAC_list += "}";

      //      found_BLE_MAC_list += ",\"state\":\"";
      //      found_BLE_MAC_list += state;
      //      found_BLE_MAC_list += "\"";



      //      if (flag)
      //      { found_BLE_MAC_list += ", " + knownBLEAddresses[i];
      //      }
      //      else
      //      { found_BLE_MAC_list += knownBLEAddresses[i];
      //      }
      //      found_BLE_MAC_list += "=" + state;

      flag++;
    }

    //    if (device_found[i])
    //    {
    //      if (flag)
    //      { found_BLE_MAC_list += ", " + knownBLEAddresses[i];
    //      }
    //      else
    //      { found_BLE_MAC_list += knownBLEAddresses[i];
    //      }
    //      flag++;
    //    }

    device_found[i] = false;
  }

  Serial.print("\n\nlist of known_BLE_MACs found : ");
  Serial.println(found_BLE_MAC_list);

  Serial.printf("\n\n----------------------------------------3-------------------------------------------\n\n");

  //WiFi_ON();

  if (found_BLE_MAC_list != "") // https://www.arduino.cc/en/Tutorial/BuiltInExamples/StringComparisonOperators
#endif
  {
    HTTP_POST_NOTIF();  //  delay(1500);
    WiFi_OFF();
  }

  found_BLE_MAC_list = "";

#ifdef TEST_MODE
  delay(10000);
#else
  delay(1000);
#endif
  // ESP.restart();

}


//Uncomment to Enable Debug Information
//Serial.println("*************Start**************");
//Serial.println(sizeof(knownBLEAddresses));
//Serial.println(sizeof(knownBLEAddresses[0]));
//Serial.println(sizeof(knownBLEAddresses)/sizeof(knownBLEAddresses[0]));
//Serial.println(advertisedDevice.getAddress().toString().c_str());
//Serial.println(knownBLEAddresses[i].c_str());
//Serial.println("*************End**************");
