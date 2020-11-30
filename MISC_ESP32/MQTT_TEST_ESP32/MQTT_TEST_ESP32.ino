/*********

  ESP32 MQTT – Publish and Subscribe with Arduino IDE
  https://randomnerdtutorials.com/esp32-mqtt-publish-subscribe-arduino-ide/
  ESP32: Publishing messages to MQTT topic
  https://techtutorialsx.com/2017/04/24/esp32-publishing-messages-to-mqtt-topic/

  H:\HCDC\Zero Config\ZeroConfig\TactileZCOTA\GigaPix_v6
  
*********/

#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>


// Replace the next variables with your SSID/Password combination
const char* ssid = "NeMo";
const char* password = "19stm3289";

// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";
const char* mqtt_server = "10.208.34.163";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

float temperature = 0;
float humidity = 0;

// LED Pin
const int ledPin = 4;

void setup() 
{
  Serial.begin(115200);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pinMode(ledPin, OUTPUT);
}

void setup_wifi() 
{
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) 
{
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) 
  {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  if (String(topic) == "esp32/output") 
  {
    Serial.print("Changing output to ");
    if(messageTemp == "on")
    {
      Serial.println("on");
      digitalWrite(ledPin, HIGH);
    }
    else if(messageTemp == "off")
    {
      Serial.println("off");
      digitalWrite(ledPin, LOW);
    }
  }
}

void reconnect() 
{
  // Loop until we're reconnected
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32Client")) 
    {
      Serial.println("connected");
      // Subscribe
      client.subscribe("dh/notification/84/10/a-10-3/sensordata");
    } 
    else 
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() 
{
  if (!client.connected()) 
  {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 5000) 
  {
    lastMsg = now;
    
    
    // Convert the value to a char array
    char tempString[8];
    //dtostrf(temperature, 1, 2, tempString);
    client.publish("dh/notification/84/10/a-10-3/sensordata", tempString);
    
    // Convert the value to a char array
    char humString[8];

    client.publish("dh/notification/84/10/a-10-3/sensordata", humString);
  }
}
