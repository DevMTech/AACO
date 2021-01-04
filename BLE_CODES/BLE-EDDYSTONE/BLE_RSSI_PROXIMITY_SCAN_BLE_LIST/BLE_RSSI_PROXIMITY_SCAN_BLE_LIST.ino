/*
  https://circuitdigest.com/microcontroller-projects/ble-based-proximity-control-using-esp32
  https://github.com/espressif/arduino-esp32/tree/master/libraries/BLE/src

  https://circuitdigest.com/microcontroller-projects/esp32-ble-server-how-to-use-gatt-services-for-battery-level-indication
  https://circuitdigest.com/microcontroller-projects/esp32-ble-client-connecting-to-fitness-band-to-trigger-light
  https://circuitdigest.com/microcontroller-projects/esp32-based-bluetooth-ibeacon


*/

//#define DEBUG_ALL_TAGS
#define DEBUG_KNOWN_TAGS

#define LED_BUILTIN 2
#define ARRSIZE(x) (sizeof(x)/sizeof(x[0]))
#define ENDIAN_CHANGE_U16(x) ((((x)&0xFF00)>>8) + (((x)&0xFF)<<8))

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

BLEScan *pBLEScan;
//    JDY-08             ESP32-EDDYSTN          ESTIMOTE
String knownBLEAddresses[] = {"d4:36:39:c2:28:3c", "24:0a:c4:83:20:c2", "C8:E7:EC:39:B8:D5"};
bool device_found[ARRSIZE(knownBLEAddresses)] = {0};
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
          Serial.printf("\nAdvertised Device Data: %s \n", advertisedDevice.toString().c_str());
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
  BLEDevice::init("ESP32_SCAN_TEST");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks()); //Init Callback Function
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100); // set Scan interval
  pBLEScan->setWindow(99);  // less or equal setInterval value
}

//When a scan completes, we have a set of found devices.
//Each device is described by a BLEAdvertisedDevice object.
//The number of items in the set is given by getCount().
//We can retrieve a device by calling getDevice() passing in
//the index (starting at 0) of the desired device.

void loop()
{ Serial.printf("\n\n----------------------------------------1-------------------------------------------\n\n");

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
  for (int i = 0; i < ARRSIZE(knownBLEAddresses); i++)
  {
    if (device_found[i])
    {
      if (flag)
      { found_BLE_MAC_list += ", " + knownBLEAddresses[i];
      }
      else
      { found_BLE_MAC_list += knownBLEAddresses[i];
      }
      flag++;
    }

    device_found[i] = false;
  }

  Serial.print("\n\nfound_BLE_MAC_list : ");
  Serial.println(found_BLE_MAC_list);

  found_BLE_MAC_list = "";

  Serial.printf("\n\n----------------------------------------3-------------------------------------------\n\n");

}


//Uncomment to Enable Debug Information
//Serial.println("*************Start**************");
//Serial.println(sizeof(knownBLEAddresses));
//Serial.println(sizeof(knownBLEAddresses[0]));
//Serial.println(sizeof(knownBLEAddresses)/sizeof(knownBLEAddresses[0]));
//Serial.println(advertisedDevice.getAddress().toString().c_str());
//Serial.println(knownBLEAddresses[i].c_str());
//Serial.println("*************End**************");
