#include "BLEDevice.h"
//#include "BLEUtils.h"
#include "BLEScan.h"
#include "BLEAdvertisedDevice.h"
#include <WiFi.h>
#include <PubSubClient.h>

// ==================MQTT Setup ================================

// Update these with values suitable for your network.
//const char* ssid = "CdacIoT";      const char* password = "cdac1234";
//const char* mqtt_server = "10.42.0.1";

const char* ssid = "cdac";      const char* password = "";
//const char* mqtt_server = "10.208.35.169";
const char* mqtt_server = "10.208.34.154";

//const char* ssid = "Xperia";      const char* password = "Hello1234";
//const char* mqtt_server = "10.208.35.169";

const char* SUB_TOPIC = "ATS/In";
const char* PUB_TOPIC = "ATS/ESP";
//char* data = "{\"x\":%s,\"y\":%s,\"z\":%s}";

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
      String Name = (Device.toString().c_str());
      signed int rssi = (Device.getRSSI());
      const char* mac = (knownAddresses[i].c_str());
      //String Mac = (knownAddresses[i].c_str());
      //char* data = "{\"rssi\":%s,\"mac\":%s,\"macd\":%s}";
      char* data = "{\"rssi\":%s,\"mac\":%s}";
      char msg[64];
      char Rssi[8] = {'\0'};
      char Mac[8] = {'\0'};
      
      for (i = 0; i < (sizeof(knownAddresses) / sizeof(knownAddresses[0])); i++) {
        if (strcmp(pServerAddress->toString().c_str(), knownAddresses[i].c_str()) == 0)
        {
          known = true;
          break;
        }
      }
      if (known) {
        Serial.println();
        Serial.println("BLE Device found.");
        Serial.println(Name);
        Serial.printf("MAC - ");
        Serial.println(mac);
        //Serial.println(knownAddresses[i].c_str());
        Serial.printf("RSSI - ");
        Serial.println(rssi);

        if (Device.getRSSI()) {
          deviceFound = true;
        }
        else {
          deviceFound = false;
        }
        Device.getScan()->stop();
      }
      dtostrf(rssi,5,3,Rssi);

      snprintf (msg, 64, data, Rssi, mac);
      Serial.println(msg);
      
      //Publish data to MQTT Topic
      client.publish(PUB_TOPIC, msg);
      delay(1000);
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
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32Client")) {
      Serial.println("connected");      
      client.subscribe(SUB_TOPIC);
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

