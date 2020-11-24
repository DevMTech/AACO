/**
 *           CLIENT CODE KEEPS RESTARTING AFTER SCANNING !!!!
 *           
 * BLE CLIENT - ARDUINO VERSION - NEWER         
 * https://github.com/espressif/arduino-esp32/blob/master/libraries/BLE/examples/BLE_client/BLE_client.ino
 * 
 * BLE CLIENT - KOLBAN VERSION - OLDER
 * https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLETests/Arduino/BLE_client/BLE_client.ino
 * 
 * Reading characteristic values inside BLE_client notifyCallback
 * https://esp32.com/viewtopic.php?t=4181
 * 
 * A BLE client example that is rich in capabilities.
 * There is a lot new capabilities implemented.
 * author unknown
 * updated by chegewara
 * 
 * Starting Arduino BLE Client application...
BLE Advertised Device found: Name: ESP32 BLE SERVER, Address: 30:ae:a4:7a:dc:16, serviceUUID: 5f0ab730-e234-4438-b01e-d8f87a84a8b9, txPower: 3
Forming a connection to 30:ae:a4:7a:dc:16
 - Created client
 - Connected to server
 - Found our service
 - Found our characteristic
The characteristic value was: T20.00H50.00L1000.00E
RemoteServerReadCharacValue: T20.00H50.00L1000.00E
We are now connected to the BLE Server.

BLE CLIENT SUCCESSFULLY CONNECTED TO BLE SERVER!
Notify callback for characteristic beb5483e-36e1-4688-b7f5-ea07361b26a8 of data length 21
RemoteServerNotifyCharacValue: T20.00H50.00L1000.00E

 * 
 */

#include "BLEDevice.h"
//#include "BLEScan.h"

//#include <WiFi.h>
//#include <WiFiClient.h>
//#include <TimeLib.h>

// The remote service we wish to connect to.
static BLEUUID serviceUUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b"); // ("5f0ab730-e234-4438-b01e-d8f87a84a8b9"); // 
// The characteristic of the remote service we are interested in.
static BLEUUID    charUUID("beb5483e-36e1-4688-b7f5-ea07361b26a8");

//// The remote service we wish to connect to.
//static BLEUUID serviceUUID("0000ffe0-0000-1000-8000-00805f9b34fb"); 
//// The characteristic of the remote service we are interested in.
//static BLEUUID    charUUID("0000ffe1-0000-1000-8000-00805f9b34fb");

static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;

static BLERemoteCharacteristic* pRemoteCharacteristic;
static BLEAdvertisedDevice* myDevice;

String RemoteServerReadCharacValue;
String RemoteServerNotifyCharacValue;

static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,  size_t length,   bool isNotify) 
  {
    Serial.print("Notify callback for characteristic ");
    Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
    Serial.print(" of data length ");
    Serial.println(length);
//    Serial.print("data: ");
//    Serial.println((char*)pData);
    
    RemoteServerNotifyCharacValue = (char*)pData;
    Serial.print("RemoteServerNotifyCharacValue: ");
    Serial.println(RemoteServerNotifyCharacValue);
    Serial.println();
}

class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
  }

  void onDisconnect(BLEClient* pclient) {
    connected = false;
    Serial.println("onDisconnect");
  }
};

bool connectToServer() {
    Serial.print("Forming a connection to ");
    Serial.println(myDevice->getAddress().toString().c_str());
    
    BLEClient*  pClient  = BLEDevice::createClient();
    Serial.println(" - Created client");

    pClient->setClientCallbacks(new MyClientCallback());

    // Connect to the remove BLE Server.
    pClient->connect(myDevice);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
    Serial.println(" - Connected to server");

    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr) {
      Serial.print("Failed to find our service UUID: ");
      Serial.println(serviceUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our service");


    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
    if (pRemoteCharacteristic == nullptr) {
      Serial.print("Failed to find our characteristic UUID: ");
      Serial.println(charUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our characteristic");

//    Serial.print(" -BLE FreeHeap: ");    Serial.println(ESP.getFreeHeap());

    // Read the value of the characteristic.
    if(pRemoteCharacteristic->canRead()) {
      std::string value = pRemoteCharacteristic->readValue();
      
//      Serial.print("The characteristic value was: ");      Serial.println(value.c_str());
      
      RemoteServerReadCharacValue = pRemoteCharacteristic->readValue().c_str(); // value.c_str();        
      Serial.print("RemoteServerReadCharacValue: ");
      Serial.println(RemoteServerReadCharacValue);
    }

    if(pRemoteCharacteristic->canNotify())
      pRemoteCharacteristic->registerForNotify(notifyCallback);

    connected = true;
}
/**
 * Scan for BLE servers and find the first one that advertises the service we are looking for.
 */
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
 /**
   * Called for each advertising BLE server.
   */
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print("BLE Advertised Device found: ");
    Serial.println(advertisedDevice.toString().c_str());

    // We have found a device, let us now see if it contains the service we are looking for.
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {

      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan = true;

    } // Found our server
  } // onResult
}; // MyAdvertisedDeviceCallbacks


void setup() {
  Serial.begin(115200);
  Serial.println("Starting Arduino BLE Client application...");
  BLEDevice::init("BLE CLIENT v2");

  // Retrieve a Scanner and set the callback we want to use to be informed when we
  // have detected a new device.  Specify that we want active scanning and start the
  // scan to run for 5 seconds.
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5, false);
} // End of setup.


// This is the Arduino main loop function.
void loop() {

  // If the flag "doConnect" is true then we have scanned for and found the desired
  // BLE Server with which we wish to connect.  Now we connect to it.  Once we are 
  // connected we set the connected flag to be true.
  if (doConnect == true) {
    if (connectToServer()) {
      Serial.println("\nWe are now connected to the BLE Server.");
    } else {
      Serial.println("We have failed to connect to the server; there is nothin more we will do.");
    }
    doConnect = false;
  }

  // If we are connected to a peer BLE Server, update the characteristic each time we are reached
  // with the current time since boot.
  if (connected) {
//    Serial.println("\nBLE CLIENT SUCCESSFULLY CONNECTED TO BLE SERVER!");
 /*   String newValue = "Time since boot: " + String(millis()/1000);
    Serial.println("Setting new characteristic value to \"" + newValue + "\"");
    
    // Set the characteristic's value to be the array of bytes that is actually a string.
    pRemoteCharacteristic->writeValue(newValue.c_str(), newValue.length());*/
  }else if(doScan){
    BLEDevice::getScan()->start(0);  // this is just eample to start scan after disconnect, most likely there is better way to do it in arduino
  }
  
  delay(1000); // Delay a second between loops.
} // End of loop
