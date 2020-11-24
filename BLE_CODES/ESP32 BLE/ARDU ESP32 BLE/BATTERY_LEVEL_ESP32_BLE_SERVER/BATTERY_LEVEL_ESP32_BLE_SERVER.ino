/*Program to use GATT service on ESP32 to send Battery Level
 * ESP32 works as server - Mobile as client
 *  https://circuitdigest.com/microcontroller-projects/esp32-ble-server-how-to-use-gatt-services-for-battery-level-indication
 *  
 *  RAW ADVT. DATA: 0x02010603030F180512200040000201060C09424C452042617474657279020A0303030F18051220004000
 *  
 *  LEN.  TYPE  VALUE                   TYPE NAME (ref. col. 2)
 *  All figures in hex (0x)
 *  
 *  02    01    06                      Flags
 *  03    03    0F18                    Complete List of 16-bit Service Class UUIDs (0x180F - GATT Service UUID)
 *  05    12    20004000                Slave Connection Interval Range (40.00ms - 80.00ms)\
 *  
 *  02    01    06                      Flags
 *  0C    09    424C452042617474657279  Complete Local Name - "BLE Battery"
 *  02    0A    03                      Tx Power Level (3dBm)
 *  03    03    0F18                    Complete List of 16-bit Service Class UUIDs (0x180F)
 *  05    12    20004000                Slave Connection Interval Range (40.00ms - 80.00ms)
 *  
 *  LEN. - length of EIR packet (TYPE + VALUE) in bytes 
 *  TYPE - GAP datatypes - https://www.bluetooth.com/specifications/assigned-numbers/generic-access-profile/
 */

 
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h> //Library to use BLE as server
#include <BLE2902.h> 
/*0x2902 - GATT Descriptor - Client Characteristic Configuration  org.bluetooth.descriptor.gatt.client_characteristic_configuration*/

bool _BLEClientConnected = false;

/*  https://www.novelbits.io/bluetooth-gatt-services-characteristics/
 *  https://devzone.nordicsemi.com/nordic/short-range-guides/b/bluetooth-low-energy/posts/ble-characteristics-a-beginners-tutorial
 *  http://dev.ti.com/tirex/content/simplelink_cc2640r2_sdk_1_40_00_45/docs/blestack/ble_user_guide/html/ble-stack-3.x/gatt.html 
 *  
 * GATT Services
 * https://www.bluetooth.com/specifications/gatt/services/
 * Name                             Uniform Type Identifier                                         Assigned Number   Specification
 * Battery Service                  org.bluetooth.service.battery_service                           0x180F            GSS
 * 
 * GATT Characteristics
 * https://www.bluetooth.com/specifications/gatt/characteristics/
 * Name                             Uniform Type Identifier                                         Assigned Number   Specification
 * Battery Level                    org.bluetooth.characteristic.battery_level                      0x2A19            GSS
 * 
 * GATT Descriptors
 * https://www.bluetooth.com/specifications/gatt/descriptors/
 * Name                             Uniform Type Identifier                                         Assigned Number   Specification
 * Characteristic User Description  org.bluetooth.descriptor.gatt.characteristic_user_description   0x2901            GSS
 * Client Characteristic Configuration  org.bluetooth.descriptor.gatt.client_characteristic_configuration   0x2902    GSS
 * 
*/
// GATT Service UUID          0x180F
#define BatteryService BLEUUID((uint16_t)0x180F)  

// GATT Characteristic UUID   0x2A19
BLECharacteristic BatteryLevelCharacteristic(BLEUUID((uint16_t)0x2A19), 
                              BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY); 
                              
// GATT Descriptor UUID       0x2901
BLEDescriptor BatteryLevelDescriptor(BLEUUID((uint16_t)0x2901));  

class MyServerCallbacks : public BLEServerCallbacks 
{
    void onConnect(BLEServer* pServer) 
    {      _BLEClientConnected = true;
    };

    void onDisconnect(BLEServer* pServer) 
    {      _BLEClientConnected = false;
    }
};

void InitBLE() {
  BLEDevice::init("BLE Battery"); // Provide BLE Server Name to be Advertised 
  
  BLEServer *pServer = BLEDevice::createServer();  // Create the BLE Server
  pServer->setCallbacks(new MyServerCallbacks());  // Attach callback

  // Create the BLE GATT Service // pBattery - pointer to BatteryService of BLEserver
  BLEService *pBattery = pServer->createService(BatteryService);        // GATT Service UUID          0x180F

  
  pBattery->addCharacteristic(&BatteryLevelCharacteristic);             // GATT Characteristic UUID   0x2A19

  /*link the descriptor with characteristics, and set the values*/
  BatteryLevelDescriptor.setValue("Percentage 0 - 100");
  BatteryLevelCharacteristic.addDescriptor(&BatteryLevelDescriptor);    // GATT Descriptor UUID       0x2901 
  BatteryLevelCharacteristic.addDescriptor(new BLE2902());               /*GATT Descriptor UUID       0x2902 
                                                                                      - Client Characteristic Configuration*/
  pBattery->start();  //Start the BLE GATT Service
  pServer->getAdvertising()->addServiceUUID(BatteryService); // Add/Attach Service          
  pServer->getAdvertising()->start(); // Start advertising
  //Asking the ESP32 to advertise so that other devices 
  //like our phone can discover it and connect to it
}

void setup() {
  Serial.begin(115200);
  Serial.println("Battery Level Indicator - BLE");
  InitBLE();
}

uint8_t level = 57, last_level=0; // DUMMY BATTERY LEVEL
  
void loop() 
{ // tell the descriptor what is the value of the battery 
  // in percentage that should be sent to the client (Phone)
  Serial.println(int(level));
  BatteryLevelCharacteristic.setValue(&level, 1);
  if(last_level!=level)
  {  BatteryLevelCharacteristic.notify();
  }
  
  last_level = level;  
  level++;
  
  if (int(level)==100)  level=0;

  delay(5000);
}

// "T24.94H53.01L515.07"
