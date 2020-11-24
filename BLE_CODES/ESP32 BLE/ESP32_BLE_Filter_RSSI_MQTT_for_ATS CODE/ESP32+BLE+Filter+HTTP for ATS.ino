#include "BLEDevice.h"
#include "BLEUtils.h"
#include "BLEScan.h"
#include "BLEAdvertisedDevice.h"
#include <WiFi.h>

// ==================================================
//      HTTP code for DeviceHive
// ==================================================

#include <HTTPClient.h>

const char* ssid = "cdac";
const char* password =  "";

String Name;
String mac;
signed int rssi;

char X[8] = {'\0'};
char Y[8] = {'\0'};
// ==================================================

static BLEAddress *pServerAddress;
BLEScan* pBLEScan;
BLEClient*  pClient;
bool deviceFound = false;

int scanTime = 5; //In seconds
int i;

String knownAddresses[] = {"c8:fd:19:9c:85:98","c8:fd:19:4a:f7:72","c8:fd:19:4a:f7:46"};
//String knownAddresses[] = {"BLE1:c8:fd:19:9c:85:98","BLE2:c8:fd:19:4a:f7:46","BLE3:c8:fd:19:4a:f7:72"};
// ESP32 MAC= 24:0A:C4:82:A0:64

unsigned long entry;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice Device){
         
      pServerAddress = new BLEAddress(Device.getAddress()); 
      bool known = false;
      bool Master = false;
      Name = (Device.toString().c_str());
      rssi = (Device.getRSSI());
      mac = (knownAddresses[i].c_str());
      
      for (i = 0; i < (sizeof(knownAddresses) / sizeof(knownAddresses[0])); i++) {
        if (strcmp(pServerAddress->toString().c_str(), knownAddresses[i].c_str()) == 0) 
        {
          known = true;
          break;
        }
      }
      if (known) {
        Serial.println("BLE Device found.");
        Serial.println(Name);
        Serial.printf("MAC : "); 
        Serial.println(mac);
        Serial.printf("RSSI : ");
        Serial.println(rssi);
        Serial.println(); 
        
        if (Device.getRSSI()) {
          deviceFound = true;
        }
        else {
          deviceFound = false;
        }
        Device.getScan()->stop();
      }
    }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Scanning...");
  
  BLEDevice::init("");
  pClient  = BLEDevice::createClient();
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  Serial.println("Done");
  
  WiFi.begin(ssid, password); 
  
  while (WiFi.status() != WL_CONNECTED) { //Check for the connection
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  
}

void loop() { 

  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
  delay(500);
  
if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
  
   HTTPClient http;   
              
   http.begin("http://10.208.43.120:80/api/rest/device/S8EVC7SiqX07gc9tMFl0qxAcLIY2ghg9ngp6/notification"); //Specify destination for HTTP request
   http.addHeader("Accept" , "application/json");  //Specify content-type header
   http.addHeader("Content-Type" , "application/json");  //Specify content-type header
   http.addHeader("Authorization" , "Bearer eyJhbGciOiJIUzI1NiJ9.eyJwYXlsb2FkIjp7ImEiOlsyLDMsNCw1LDYsNyw4LDksMTAsMTEsMTUsMTYsMTddLCJlIjoxNTYxODY5MDAwMDAwLCJ0IjoxLCJ1IjoyLCJuIjpbIjIiXSwiZHQiOlsiMTEiLCIxMCJdfX0.9a--YK1lP_Vf6o_3-Oic_2U_AP5STO0RjZBQL-7Vz2Q");  //Specify content-type header
   
 //  dtostrf(mac, 5,3,X);
   dtostrf(rssi, 5,3,Y);
   
//String data = "{\"SensorType\": \"Temperature\", \"parameters\": {\"MAC\": "mac"}}"; 

 //  String data = "{\"notification\": \"ATS(BLE)\",\"timestamp\": \"2019-06-28T05:30:11.242Z\",\"parameters\": {\"MAC\": [1020d5f67g10]""}}";
 //  int httpResponseCode =  http.POST(data);
   
   String data = "{\"notification\": \"ATS(BLE)\",\"timestamp\": \"2019-06-28T05:30:11.242Z\",\"parameters\": {\"MAC\":";
        data += mac;
        data += ",\"RSSI\":";
        data += Y;
        data += "}}";
        int httpResponseCode =  http.POST(data);

   if(httpResponseCode>0){
    String response = http.getString();                       //Get the response to the request
    Serial.println(httpResponseCode);   //Print return code
    Serial.println(response);           //Print request answer
    
   }else{
    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);
   }
   http.end();  //Free resources
   
 }else{
    Serial.println("Error in WiFi connection");   
 }
}
