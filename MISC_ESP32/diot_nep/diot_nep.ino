#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>



/*
 * *Configurations
 */
 
// Replace the next variables with your SSID/Password combination
const char* ssid = "cdac";
const char* password = "";

// Add your MQTT Broker IP address, example:
const char* mqtt_server = "10.208.34.200";
//const char* mqtt_server = "YOUR_MQTT_BROKER_IP_ADDRESS";

// Set mqtt clientID
const char* clientID = "AACO1"; // "ghjgjhguhjgjhg";

//set topic
char topic[150];

//Set Auth request

char *payload =  "{\"action\":\"authenticate\",\"token\":\"eyJhbGciOiJIUzI1NiJ9.eyJwYXlsb2FkIjp7ImEiOlswXSwiZSI6MTY0MDg4OTAwMDAwMCwidCI6MSwidSI6MiwibiI6WyIqIl0sImR0IjpbIioiXX19.zol7iki1KmeFwie30HYqRfE_tzMVkgQqGCzSJJlaivg\"}";
//char *token = "eyJhbGciOiJIUzI1NiJ9.eyJwYXlsb2FkIjp7ImEiOlswXSwiZSI6MTY0MDg4OTAwMDAwMCwidCI6MSwidSI6MiwibiI6WyIqIl0sImR0IjpbIioiXX19.zol7iki1KmeFwie30HYqRfE_tzMVkgQqGCzSJJlaivg";


WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

float status = 0;

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  client.setBufferSize(1024);
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
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  if (String(topic) == "esp32/output") {
    Serial.print("the notification is ");
    Serial.println(messageTemp);
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(clientID)) {
      Serial.println("connected");
      // Subscribe
      //client.subscribe("esp32/output");
      sprintf(topic, "%s", ""); // Cleans the topic content
      sprintf(topic, "%s%s", "dh/response/authenticate@", clientID);
      Serial.println(topic);
      client.subscribe(topic);
      //sprintf(payload,"{\"action\":\"%s\"}" , token);
      //delay(3000);
      client.publish("dh/request", payload);
      Serial.println(payload);
      delay(3000);

      sprintf(topic, "%s", ""); // Cleans the topic content
      sprintf(topic, "%s%s", "dh/response/device/list@", clientID);
      Serial.println(topic);
      client.subscribe(topic);
      //delay(3000);
      char *payload1 = "{\"action\": \"device/list\"}";
      client.publish("dh/request", payload1);
      //Serial.println(payload1);
      
      
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
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    
    // Temperature in Celsius
    //status = 10; //getInkStatus();   
    
    // Convert the value to a char array
    //char statusString[8];
    //dtostrf(status, 1, 2, statusString);
    //Serial.print("ink_status : ");
    //Serial.println(statusString);
    //client.publish("esp32/status", statusString);
    //client.publish("dh/request", payload);
    //Serial.println(payload);
    //client.publish("esp32/status", payload);
  }
}
