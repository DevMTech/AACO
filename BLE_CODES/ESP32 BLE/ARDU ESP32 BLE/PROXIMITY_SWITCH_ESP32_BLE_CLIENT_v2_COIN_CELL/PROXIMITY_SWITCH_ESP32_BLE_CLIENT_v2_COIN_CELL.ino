

/*
 * Program to operate ESP32 in client mode(as SCANNER) and 
 * use any BLE tag/fitness band(server) as proximity switch  
 * 
 *  https://circuitdigest.com/microcontroller-projects/esp32-ble-client-connecting-to-fitness-band-to-trigger-light
 *  
 * Reference: https://github.com/nkolban/esp32-snippets 
 * 
 * NOTE: The BLE_Tag_Addr_to_Scan, serviceUUID and charUUID should be changed based on the BLe server you are using 
 */

#include <BLEDevice.h> //Header file for BLE 

static int LED_PIN = 2; //15 BUZZER PIN

//int LED_BUILTIN = 2;
//"50:8c:b1:40:16:47"; // JDY-08 ZERO PCB 
//"98:5d:ad:23:14:bd"; // JDY-08 COIN CELL "985dad2314bd"
//"c8:fd:19:4a:f7:72"; // BLE3 (HM10) MAC (BLE_SERVER_TAG)
// Scanned Remote BLE Device : MAC "C8:FD:19:4A:F7:72", Service UUID, Characterisitics UUID obtained using any BLE Scanner App 
//String BLE_Tag_Addr_to_Scan = "c8:fd:19:4b:4d:8f"; //Hardware Bluetooth MAC of my BLE_SERVER_TAG
String BLE_Tag_Addr_to_Scan = "50:8c:b1:40:16:47"; // JDY-08 ZERO PCB

//static BLEUUID serviceUUID("0000fee0-0000-1000-8000-00805f9b34fb"); //Service UUID of BLE_SERVER_TAG obtained through nRF connect application 
//static BLEUUID    charUUID("0000fee1-0000-1000-8000-00805f9b34fb"); //Characteristic  UUID of BLE_SERVER_TAG obtained through nRF connect application 

static BLEUUID serviceUUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b"); //Service UUID of BLE_SERVER_TAG  
static BLEUUID    charUUID("beb5483e-36e1-4688-b7f5-ea07361b26a8"); //Characteristic  UUID of BLE_SERVER_TAG 

static BLERemoteCharacteristic* pRemoteCharacteristic;

BLEScan* pBLEScan; //Name the scanning device as pBLEScan
BLEScanResults foundDevices;

static BLEAddress *Server_BLE_Address;
String Scanned_BLE_Address;

bool paired = false; //boolean variable to toggle light 

bool connectToserver (BLEAddress pAddress) // Server_BLE_Address - REMOTE SERVER (BLE TAG) ADDR
{    
    BLEClient*  pClient  = BLEDevice::createClient();
    Serial.println(" - Created client");

    // Connect to the BLE Server.
    pClient->connect(pAddress); // Server_BLE_Address - REMOTE SERVER ADDR
    Serial.println(" - Connected to Server_BLE_Address - BLE_SERVER_TAG");

    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService != nullptr)
    {   Serial.println(" - Found our service");//      return true;
    }
    else
    {  return false;}

    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
    if (pRemoteCharacteristic != nullptr)
    {   Serial.println(" - Found our characteristic");
    }
    else
    {   return false;}
        
    return true;
}

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks 
{
    void onResult(BLEAdvertisedDevice advertisedDevice) 
    { Serial.printf("Scan Result: %s \n", advertisedDevice.toString().c_str());
      Server_BLE_Address = new BLEAddress(advertisedDevice.getAddress());  // static BLEAddress *    
      Scanned_BLE_Address = Server_BLE_Address->toString().c_str(); // String
    }
};

void setup() {
    Serial.begin(115200); //Start serial monitor 
    Serial.println("ESP32 BLE Server program"); //Intro message 

    BLEDevice::init("");
    pBLEScan = BLEDevice::getScan(); //create new scan
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks()); //Call the class that is defined above 
    pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster

    pinMode (LED_PIN,OUTPUT); //Declare the in-built LED pin as output 
}

void loop() {

  foundDevices = pBLEScan->start(3); //Scan for 3 seconds to find the Fitness band 

  while (foundDevices.getCount() >= 1)
  {
    if (Scanned_BLE_Address == BLE_Tag_Addr_to_Scan && paired == false)
    {
      Serial.println("Found BLE Server Device ... TURNING ON LED ON LED_PIN");
      digitalWrite (LED_PIN,HIGH);
      
//      Serial.println("Found BLE Server Device ... TOGGLING LED ON LED_PIN");
      toggle_led(10);      
      
      Serial.println("Found BLE Server Device ... connecting to Server as client");
      
      if (connectToserver(*Server_BLE_Address))
      {
        paired = true;
        Serial.println("********************LED turned ON************************");
        digitalWrite (LED_PIN,HIGH);
        break;
      }
      else
      {
        Serial.println("Pairing failed");
        break;
      }
    }
    else
    { Serial.println("DID NOT FIND BLE Server Device ... TURNING OFF LED ON LED_PIN");
      digitalWrite (LED_PIN,LOW);
      paired = false;
    }
    
    if (Scanned_BLE_Address == BLE_Tag_Addr_to_Scan && paired == true)
    {
      Serial.println("Our device went out of range");
      paired = false;
      Serial.println("********************LED OOOFFFFF************************");
      digitalWrite (LED_PIN,LOW);
      ESP.restart();
      break;
    }
    else
    {
    Serial.println("We have some other BLe device in range");
    break;
    }
  } 
}

void toggle_led(int times)
{
    for(int i=0; i<times; i++)
    { digitalWrite (LED_PIN,HIGH);
      delay(500);
      digitalWrite (LED_PIN,LOW);
      delay(500);
    }
}
