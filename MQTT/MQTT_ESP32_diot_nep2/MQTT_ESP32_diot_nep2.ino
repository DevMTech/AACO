/*
 * *Configurations
  // https://www.npmjs.com/package/mqtt
  // https://www.npmjs.com/package/mosca
  Connecting to cdac
  ..........
  WiFi connected
  IP address:
  10.208.34.98
  Attempting MQTT connection...connected
  dh/response/authenticate@ghjgjhguhjgjhg
  {"action":"authenticate","token":"eyJhbGciOiJIUzI1NiJ9.eyJwYXlsb2FkIjp7ImEiOlswXSwiZSI6MTY0MDg4OTAwMDAwMCwidCI6MSwidSI6MiwibiI6WyIqIl0sImR0IjpbIioiXX19.zol7iki1KmeFwie30HYqRfE_tzMVkgQqGCzSJJlaivg"}
  dh/response/device/list@ghjgjhguhjgjhg
  Message arrived on topic: dh/response/authenticate@ghjgjhguhjgjhg. Message: {"action":"authenticate","requestId":null,"status":"success"}
  Message arrived on topic: dh/response/device/list@ghjgjhguhjgjhg. Message: {"action":"device/list","requestId":null,"status":"success","devices":[{"id":"e50d6085-2aba-48e9-b1c3-73c673e414be","name":"Sample Device","data":null,"networkId":1,"deviceTypeId":1,"isBlocked":false},{"id":"XUjh23W3baZEvr8uE8uvzzJf2gVnwVxWON6R","name":"IME Device 1","data":null,"networkId":1,"deviceTypeId":1,"isBlocked":false}]}

*/

#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>

// Replace the next variables with your SSID/Password combination
const char* ssid = "cdac";
const char* password = "";

// Add your MQTT Broker IP address, example:
const char* mqtt_server = "10.208.34.200"; // YOUR_MQTT_BROKER_IP_ADDRESS

// Set mqtt clientID
const char* clientID = "ghjgjhguhjgjhg"; // "AACO1"; // 

//set topic
char topic[150];  // .toString().c_str()

const char* outTopic = "NodeMCUout";
const char* inTopic = "NodeMCUin";

const char* SUB_TOPIC = "ATS/In";
const char* PUB_TOPIC = "vats/museum123"; // "ATS/ESP";

String DH_DEVICE_ID = "e50d6085-2aba-48e9-b1c3-73c673e414be";

//Set Auth request
char *myPayload =  "{\"action\":\"authenticate\",\"token\":\"eyJhbGciOiJIUzI1NiJ9.eyJwYXlsb2FkIjp7ImEiOlswXSwiZSI6MTY0MDg4OTAwMDAwMCwidCI6MSwidSI6MiwibiI6WyIqIl0sImR0IjpbIioiXX19.zol7iki1KmeFwie30HYqRfE_tzMVkgQqGCzSJJlaivg\"}";
//char *token = "eyJhbGciOiJIUzI1NiJ9.eyJwYXlsb2FkIjp7ImEiOlswXSwiZSI6MTY0MDg4OTAwMDAwMCwidCI6MSwidSI6MiwibiI6WyIqIl0sImR0IjpbIioiXX19.zol7iki1KmeFwie30HYqRfE_tzMVkgQqGCzSJJlaivg";

// PUB DATA FORMAT :
// { "temp": "27.46", "humi": "65.83", "lux": "545.70", "C": "432.00", "D": "3.51", "D10": "18.50", "B": "4.46", "N": "5" }
// {"yaw":34,"pitch":42,"roll":28}

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50]; // PUBLISH MSG
int value = 0;

float status = 0;

// LED Pin
const int ledPin = 2;

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  client.setBufferSize(1024);

  pinMode(ledPin, OUTPUT);
  delay(100);
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

void callback(char* topic, byte* payload, unsigned int length)
{
  // Convert the incoming byte array to a string
  payload[length] = '\0'; // Null terminator used to terminate the char array
  String message = (char*)payload;

  Serial.print("Message arrived on topic: [");
  Serial.print(topic);
  Serial.print("], \n Message: ");
  Serial.println(message);

  //  String messageTemp;
  for (int i = 0; i < length; i++)
  { char ch = (char)payload[i];
    if (ch == '{')
      Serial.println("\t");

    Serial.print(ch); // (char)message[i]);
    //messageTemp += ch; // (char)message[i];
  }
  Serial.println();

  if (String(topic) == SUB_TOPIC) // "esp32/output")
  {
    Serial.println("\n the notification is message :  ");
    Serial.println(message);
  }


  // Switch on the LED if an 1 was received as first character
  //  if ((char)message[0] == '0')
  //  { digitalWrite(relay_pin, LOW);   // Turn the LED on (Note that LOW is the voltage level)
  //    Serial.println("relay_pin -> LOW");
  //  }

  if (String(topic) == "esp32/output")
  {

    Serial.print("Changing output to ");
    if (message == "on")
    {
      Serial.println("on");
      digitalWrite(ledPin, HIGH);
    }
    else if (message == "off")
    {
      Serial.println("off");
      digitalWrite(ledPin, LOW);
    }

    if (message == "temp")
    {
      //gettemperature();
      Serial.print("Sending temperature:");
      //Serial.println(temp_c);
      //dtostrf(temp_c , 2, 2, msg);
      //client.publish(outTopic, msg);

    }
    else if (message == "humidity")
    {
      //gettemperature();
      Serial.print("Sending humidity:");
      //Serial.println(humidity);
      //dtostrf(humidity , 2, 2, msg);

      //Publish data to MQTT Topic
      //client.publish(outTopic, msg);
      //client.publish(PUB_TOPIC, msg);

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
    if (client.connect(clientID))
    {
      Serial.println("connected");

      // Once connected, publish an announcement...
      //client.publish(outTopic, clientID);
      // client.publish(outTopic, "Sonoff1 booted");
      // ... and resubscribe
      //client.subscribe(inTopic);

      // Subscribe
      sprintf(topic, "%s", ""); // Cleans the topic content
      sprintf(topic, "%s%s", "dh/response/authenticate@", clientID);
      Serial.println("\n SUB TOPIC : " + String(topic));
      client.subscribe(topic); //client.subscribe("esp32/output");
      //sprintf(payload,"{\"action\":\"%s\"}" , token);
      //delay(3000);

      client.publish("dh/request", myPayload);
      Serial.println("\n PUB TOPIC : dh/request \n PAYLOAD -> " + String(myPayload));
      //Serial.println(payload);
      delay(3000);

      sprintf(topic, "%s", ""); // Cleans the topic content
      sprintf(topic, "%s%s", "dh/response/device/list@", clientID);
      Serial.println("\n SUB TOPIC : " + String(topic));
      client.subscribe(topic);
      //delay(3000);

      // Subscribe
      //client.subscribe("dh/notification/84/9/a-9-3/sensordata");

      char *myPayload1 = "{\"action\": \"device/list\"}";
      client.publish("dh/request", myPayload1);
      Serial.println("\n PUB TOPIC : dh/request \n PAYLOAD -> " + String(myPayload1));
      Serial.println("\n");


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
  { reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 5000)
  {
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


    //    if (relayState == 1)
    //    { client.publish(outTopic, "1");
    //    }
    //    else if (relayState == 0)
    //    { client.publish(outTopic, "0");
    //    }

    // Convert the value to a char array
    char tempString[8] = "50";
    //dtostrf(temperature, 1, 2, tempString);
    //client.publish("dh/notification/1/1/9/a-9-3/sensordata", tempString);
    client.publish("dh/notification/1/1/e50d6085-2aba-48e9-b1c3-73c673e414be/sensordata", tempString);

    // Convert the value to a char array
    char humString[8];
    //dtostrf(humidity, 1, 2, humString);
    //client.publish("dh/notification/1/1/9/a-9-3/sensordata", humString);

  }
}
