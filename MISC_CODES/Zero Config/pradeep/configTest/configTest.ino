
#include <ArduinoJson.h>
#include <SD.h>
#include <SPI.h>

char data[] = "{\"Device\": {\"MAC\": \"\",\"IP\": \"\",\"UUID\": \"-\",\"editable\": 0},\"Networking\": {\"SSID\": \"\",\"Password\": \"\",\"editable\": 1},\"Configuration\": {\"PlatformURL\": \"url\",\"No_of_switches\": \"3\",\"editable\": 1}}";
StaticJsonBuffer<1000> jb;
JsonObject& json = jb.parseObject(data);
StaticJsonBuffer<1000> jsonBuffer;
JsonVariant root=jsonBuffer.createObject();

void setup() {
  // Initialize serial port
  Serial.begin(9600);
  while (!Serial) continue;  
  root = json;
}

void loop() {
  // not used in this example
}
