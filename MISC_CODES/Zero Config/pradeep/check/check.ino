
#define DEBUG

#ifdef DEBUG
  #define Debug(x)    Serial.print(x)
  #define Debugln(x)  Serial.println(x)
  #define Debugf(...) Serial.printf(__VA_ARGS__)
  #define Debugflush  Serial.flush
#else
  #define Debug(x)    {}
  #define Debugln(x)  {}
  #define Debugf(...) {}
  #define Debugflush  {}
#endif

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Ticker.h>
#include "FS.h"

extern "C" {
  #include "user_interface.h" //Needed for the reset command
}

MDNSResponder mdns;
ESP8266WebServer server(80);
WiFiClient wifiClient;
Ticker btn_timer;
Ticker otaTickLoop;

String hostName ="CDAC-ESP";

//##### Flags ##### They are needed because the loop needs to continue and cant wait for long tasks!
int rstNeed=0;   // Restart needed to apply new settings
int configToClear=0; // determine if config should be cleared.
int otaFlag=0;
boolean inApMode=0;

//##### Global vars ##### 
int webtypeGlob;
int otaCount=300; //imeout in sec for OTA mode
int current; //Current state of the button
unsigned long count = 0; //Button press time counter
String st; //WiFi Stations HTML list
char* host; //The DNS hostname

char json[] = "{\"Device\": {\"MAC\": \"\",\"IP\": \"\",\"UUID\":\"-\"},\"Networking\": {\"SSID\": \"\",\"Password\": \"\"},\"Configuration\": {\"PlatformURL\": \"url\",\"Button_1\":\"bus\",\"Button_2\":2,\"Button_3\":3,\"Button_4\":4,\"Button_5\":5,\"Button_6\":6,\"Button_7\":7,\"Button_8\":8,\"Button_9\":9,\"Button_10\":10,\"Button_11\":11,\"Button_12\":12,\"Button_13\":13,\"Button_14\":14,\"Button_15\":15,\"Button_16\":16,\"Button_17\":17},\"Firmware\": {\"Application\": \"Tactile\",\"Version\": \"1.0\"},\"Actions\": [{\"Action_id\": 1, \"req\": [\"ButtonID\", \"Value\"] ,\"res\":[\"Status\"],\"Description\": \"Please set the value to publish on button press\"},{\"Action_id\": 2, \"req\": [\"PlatformURL\", \"URL\"] ,\"res\":[\"Status\"],\"Description\": \"Please set the platform URL\"}],\"Events\": [{\"Event_id\": 2,\"req\": [\"ButtonID\", \"Value\", \"PressType\"],\"res\":[\"Status\"],\"Description\": \"Triggers on event of button press\"}]}";
StaticJsonBuffer<1000> jsonBuffer;
JsonObject& iRoot = jsonBuffer.parseObject(json);


StaticJsonBuffer<1000> jB;
JsonObject& root=jB.createObject();

// the setup function runs once when you press reset or power the board
void setup() {
  
  Serial.begin(115200);
  delay(10);
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
  }
  uint8_t mac[6];
  WiFi.macAddress(mac);
  hostName += "-";
  hostName += macToStr(mac);
  //root["Device"]["MAC"] = macToStr(mac);
  String hostTemp=hostName;
  hostTemp.replace(":","-");
  host = (char*) hostTemp.c_str();
  parseObjects(iRoot);
  loadConfig();
  Serial.print(root["Device"]["MAC"].as<char*>());
  initWiFi();
}

// the loop function runs over and over again forever
void loop() {
//buildPage(root);
  if (webtypeGlob == 1){
    server.handleClient();
    delay(1);
  } 
}

void buildPage( JsonObject &obj){
  for( const auto& kv : obj ) {
      if(kv.value.is<JsonObject>()){
        Serial.print(kv.key);
        Serial.println(" : ");
        JsonObject& temp = kv.value.as<JsonObject>();
        buildPage(temp);
      }
      else
        Serial.print(kv.key);
        Serial.print(" : ");
        Serial.println(kv.value.as<char*>());
  }
}

void parseObjects( JsonObject &obj){
  for( const auto& kv : obj ) {
      Serial.println(kv.key);
      if(kv.value.is<char*>())
        Serial.println(kv.value.as<char*>());
      else if(kv.value.is<JsonArray>()){
        JsonArray& temp = kv.value.as<JsonArray>();
        parseArrays(temp);
      }
      else if(kv.value.is<JsonObject>()){
        JsonObject& temp = kv.value.as<JsonObject>();
        parseObjects(temp);
      }
      else
        Serial.println(kv.value.as<char*>());
  }
}

void parseArrays( JsonArray &ar){
  for( const auto& value : ar ) {
    if(value.is<char*>())
        Serial.println(value.as<char*>());
      else if(value.is<JsonArray>()){
        JsonArray& temp = value.as<JsonArray>();
        parseArrays(temp);
      }
      else if(value.is<JsonObject>()){
        JsonObject& temp = value.as<JsonObject>();
        parseObjects(temp);
      }
      else
        Serial.println(value.as<char*>());
  }
}
