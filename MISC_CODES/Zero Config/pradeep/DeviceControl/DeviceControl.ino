
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid = "AndroidAP";
const char* password = "tiptopmail";
const char* mqtt_server = "192.168.43.225";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  pinMode(14, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(16, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  digitalWrite(14, LOW);
  digitalWrite(16, LOW);
  digitalWrite(5, LOW);
  digitalWrite(4, LOW);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
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

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    if((char)payload[1] == '1') {
      digitalWrite(14, HIGH);
    }
    else {
      digitalWrite(14, LOW);
    }
  } else if ((char)payload[0] == '2') {
    if((char)payload[1] == '1') {
      digitalWrite(4, HIGH);
    }
    else {
      digitalWrite(4, LOW);
    }
  }
  else if ((char)payload[0] == '3') {
    if((char)payload[1] == '1') {
      digitalWrite(5, HIGH);
    }
    else {
      digitalWrite(5, LOW);
    }
  }
  else{
    
  }
  

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect//clientID, willTopic, willQoS, willRetain, willMessage)
    if (client.connect("ESP8266Clicvfrdecent","available",2,true,"0")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("available", "1");
      // ... and resubscribe
      client.subscribe("myhome");
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
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, 75, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    //client.publish("outTopic", msg);
  }
}
