//#include "BLEDevice.h"
//#define FLORA_ADDR "c4:7c:8d:66:e9:11"
//#define BLYNK_NO_BUILTIN   // Disable built-in analog & digital pin operations
//#include <TimeLib.h>
//#include "WiFi.h"
//#include <WiFiClient.h>
//#include <BlynkSimpleEsp32.h>
//#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
//#define TIME_TO_SLEEP  600        /* Time ESP32 will go to sleep (in seconds) */
//
//BLEClient* pClient;
//
////**********TOKEN (OK)**********//
//char auth[] = "YOUR TOKEN";
//
////**********WIFI (OK)**********//
//char ssid[] = "YOUR WIFI SSID";
//char pass[] = "YOUR WIFI PASSWORD";
//
//// The remote service we wish to connect to.
//static BLEUUID serviceUUID("00001204-0000-1000-8000-00805f9b34fb");
//
//// The characteristic of the remote service we are interested in.
//static BLEUUID uuid_sensor_data("00001a01-0000-1000-8000-00805f9b34fb");
//static BLEUUID uuid_write_mode("00001a00-0000-1000-8000-00805f9b34fb");
//static BLEAddress floraAddress(FLORA_ADDR);
//static BLERemoteCharacteristic* pRemoteCharacteristic;
//float temp;
//int moisture;
//int light;
//int conductivity;
//int freeHeapstart;
//int freeHeapstop;
//  
//void getSensorData(BLEAddress pAddress){
//  btStart();
//  //delay(500);
//  Serial.print("============================ Forming a connection to Flora device ===============================");
//  Serial.print(pAddress.toString().c_str());
//  // Connect to the remove BLE Server.
//  if (!pClient->connect(pAddress)){
//  pClient->disconnect();
//  ESP.restart();
//  } else {
//  Serial.println(" - Connected to Flora");
//  }
//  
//  // Obtain a reference to the service we are after in the remote BLE server.
//  BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
//  if (pRemoteService == nullptr) {
//    Serial.print("Failed to find our service UUID: ");
//    Serial.println(serviceUUID.toString().c_str());
//  } else {
//    Serial.println(" - Found our service");
//  }
// 
//  pRemoteCharacteristic = pRemoteService->getCharacteristic(uuid_write_mode);
//  uint8_t buf[2] = {0xA0, 0x1F};
//  pRemoteCharacteristic->writeValue(buf, 2, true);
//  
//  // Obtain a reference to the characteristic in the service of the remote BLE server.
//  pRemoteCharacteristic = pRemoteService->getCharacteristic(uuid_sensor_data);
//  Serial.println(pRemoteService->toString().c_str());
//  if (pRemoteCharacteristic == nullptr) {
//    Serial.print("Failed to find our characteristic UUID: ");
//    Serial.println(uuid_sensor_data.toString().c_str());
//  } else {
//      Serial.println(" - Found our characteristic");
//    }
//    
//  // Read the value of the characteristic.
//  std::string value = pRemoteCharacteristic->readValue();
//  Serial.print("The characteristic value was: ");
//  const char *val = value.c_str();
//
//  Serial.print("Hex: ");
//  for (int i = 0; i < 16; i++) {
//    Serial.print((int)val[i], HEX);
//    Serial.print(" ");
//  }
//  Serial.println(" ");
//
//  temp = (val[0] + val[1] * 256) / ((float)10.0);
//  moisture = val[7];
//  light = val[3] + val[4] * 256;
//  conductivity = val[8] + val[9] * 256;
//  
//  char buffer[64];
//  
//  Serial.print("Temperature: ");
//  Serial.println(temp);
//
//  Serial.print("Moisture: ");
//  Serial.println(moisture);
//  
//  Serial.print("Light: ");
//  Serial.println(light);
//  
//  Serial.print("Conductivity: ");
//  Serial.println(conductivity);
//  pClient->disconnect();
//  delay(500);
//  btStop();
//  delay(500);
//  sendSensorData();
//}
//
//void sendSensorData() {
//  Blynk.begin(auth, ssid, pass);
//  Blynk.connect();
//  Blynk.virtualWrite(V6, temp);
//  Blynk.virtualWrite(V7, moisture);
//  delay(500);
//  Blynk.disconnect();
//  WiFi.mode(WIFI_OFF);
////  freeHeapstop = ESP.getFreeHeap();
////  Serial.print("FREEHEAP END : ");
////  Serial.println(freeHeapstop);
//  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR); // Setup wake up interval
//  delay(500);
//  esp_deep_sleep_start(); // Start deep sleep
//}
//
//void setup() {
//  Serial.begin(115200);
//  Serial.println("Starting Flora client...");
//  BLEDevice::init("");
//  pClient  = BLEDevice::createClient();
//  delay(500);
//  getSensorData(floraAddress);
//}
//
//void loop() {
//}*/
