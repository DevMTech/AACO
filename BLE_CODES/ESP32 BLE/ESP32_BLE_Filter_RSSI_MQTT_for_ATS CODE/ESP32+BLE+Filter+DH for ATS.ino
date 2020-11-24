#include "BLEDevice.h"
#include "BLEUtils.h"
#include "BLEScan.h"
#include "BLEAdvertisedDevice.h"
#include <WiFi.h>
#include <PubSubClient.h>

signed int rssi;

// ==================MQTT Setup ================================

// Update these with values suitable for your network.
//const char* ssid = "CdacIoT";      const char* password = "cdac1234";
//const char* mqtt_server = "10.42.0.1";

const char* ssid = "cdac";      const char* password = "";
//const char* mqtt_server = "10.208.35.169";
//const char* mqtt_server = "10.208.34.154";
const char* mqtt_server = "10.208.43.20";

const char* SUB_TOPIC = "dh/response/authenticate@ESP32_ATS";
const char* PUB_TOPIC = "dh/request";
String data ="{\"action\": \"authenticate\", \"token\": \"eyJhbGciOiJIUzI1NiJ9.eyJwYXlsb2FkIjp7ImEiOlsyLDMsNCw1LDYsNyw4LDksMTAsMTEsMTUsMTYsMTddLCJlIjoxNTk2MjU2MjAwMDAwLCJ0IjoxLCJ1IjoyLCJuIjpbIjIiXSwiZHQiOlsiMTEiLCIxMCJdfX0.kSSbgK0j4RSLdvFQH09hgnvCIaJTzd8w8s1Zn1rnHaM\"}";
                                                          
//char* data = "{\"rssi\":%s}";
 
//String SensorData = "{\"action\": \"notification/insert\", \"deviceid\": \"S8EVC7SiqX07gc9tMFl0qxAcLIY2ghg9ngp6\", \"notification\": {\"notification\": \"ATS(BLE based)\",\"parameters\": {\"RSSI\":"+String(rssi) += -55+"}}}";
  String SensorData = "{\"action\": \"notification/insert\", \"deviceId\": \"S8EVC7SiqX07gc9tMFl0qxAcLIY2ghg9ngp6\", \"notification\": {\"notification\": \"ATS(BLE based)\",\"parameters\": {\"RSSI\":-55}}}";     
        /*\"parameters\": {\"RSSI\":";                                       
        data += Y;
        data += },
        "}}";*/

WiFiClient espClient;
PubSubClient client(espClient);

//================================================================

static BLEAddress *pServerAddress;
BLEScan* pBLEScan;
BLEClient*  pClient;
bool deviceFound = false;

String Name;

int scanTime = 5; //In seconds
int i;

String knownAddresses[] = {"c8:fd:19:9c:85:98", "c8:fd:19:4a:f7:72", "c8:fd:19:4a:f7:46"};
//String knownAddresses[] = {"BLE1:c8:fd:19:9c:85:98","BLE2:c8:fd:19:4a:f7:46","BLE3:c8:fd:19:4a:f7:72"};

unsigned long entry;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice Device) {

      pServerAddress = new BLEAddress(Device.getAddress());
      bool known = false;
      bool Master = false;
 /*     //String Name = (Device.toString().c_str());
      //signed int rssi = (Device.getRSSI());
      //const char* mac = (knownAddresses[i].c_str());
      //String Mac = (knownAddresses[i].c_str());
      //char* data = "{\"rssi\":%s,\"mac\":%s,\"macd\":%s}";
      //char* data = "{\"rssi\":%s,\"mac\":%s}";
      char* data = "{\"rssi\":%s}";
      char msg[64];
      char Rssi[8] = {'\0'};
      //char Mac[8] = {'\0'};
      signed int rssi;
      String Name;
  */    
      for (i = 0; i < (sizeof(knownAddresses) / sizeof(knownAddresses[0])); i++) {
        if (strcmp(pServerAddress->toString().c_str(), knownAddresses[i].c_str()) == 0)
        {
          known = true;
          break;
        }
      }
      if (known) {
        if (Device.getRSSI()) {
          deviceFound = true;
        }
        else {
          deviceFound = false;
        }
        Device.getScan()->stop();
/*
        String Name = (Device.toString().c_str());
        signed int rssi = (Device.getRSSI());
        //const char* mac = (knownAddresses[i].c_str());
        //String Mac = (knownAddresses[i].c_str());
        Serial.println();
        Serial.println("BLE Device found.");
        Serial.println(Name);
        //Serial.printf("MAC - ");
        //Serial.println(mac);
        //Serial.println(knownAddresses[i].c_str());
        Serial.printf("RSSI - ");
        Serial.println(rssi);
        
        dtostrf(rssi,5,3,Rssi);
        snprintf (msg, 64, data, Rssi);
        //snprintf (msg, 64, data, Rssi, mac);
        Serial.println(msg);
        //Serial.println(WiFi.macAddress());
      
        //Publish data to MQTT Topic
        client.publish(PUB_TOPIC, msg);
        delay(1000);

      }
      /*
      dtostrf(rssi,5,3,Rssi);
      snprintf (msg, 64, data, Rssi);
      //snprintf (msg, 64, data, Rssi, mac);
      Serial.println(msg);
      //Serial.println(WiFi.macAddress());
      
      //Publish data to MQTT Topic
      client.publish(PUB_TOPIC, msg);
      delay(1000);
      */
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
  delay(5000);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  delay(1000);
 
 
  
}
void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.macAddress());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  //To Do : check authentication status
  Serial.println();
  //device authenticate zalyavr sensordata same topic vr publish karaycha ahe
  client.publish(PUB_TOPIC, SensorData.c_str());
   client.publish("HelloWorld",SensorData.c_str());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32_ATS")) {
      Serial.println("connected");      
      client.subscribe(SUB_TOPIC);
       //client authenticated
      client.publish(PUB_TOPIC, data.c_str());
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {

  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
  delay(5000);

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  delay(1000);
 
}
