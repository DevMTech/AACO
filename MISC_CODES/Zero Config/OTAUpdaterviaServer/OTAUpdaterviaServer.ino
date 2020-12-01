/**
 * File: Configuration.ino
 * Purpose: TO remotely configure the Time interval of Blinking LED of ESP12.
 * Created On: 12/01/2018
 *
 **/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

const char* ssid = "cdac";
const char* password = "";

ESP8266WebServer server(80);

const int led = 13;
      
void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/html","<h1>Hello from ESP</h1>");
  digitalWrite(led, 0);
}

void handleGPIODetais(){
  server.sendHeader("Location","/");
  if(!server.hasArg("token") || server.arg("token") !="123456789")
      server.send(400,"text/html","<h1>HTTP 400:</h1><h3> Bad Request</h3>");
  else
  {    
        String location = server.arg("binary_location");
        Serial.println(location);
        if(location != NULL)
        {
          Serial.println("New location received");         
         
          server.send(200,"{'status':'ok'}");
          
          /*---------------------
           * To Make OTA update
           *--------------------*/
         t_httpUpdate_return ret = ESPhttpUpdate.update(location);
         //t_httpUpdate_return  ret = ESPhttpUpdate.update("https://server/file.bin");

         switch(ret) {
            case HTTP_UPDATE_FAILED:
                Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
                server.send(404,"{'status':'Failed'}");
                break;

            case HTTP_UPDATE_NO_UPDATES:
                Serial.println("HTTP_UPDATE_NO_UPDATES");
                server.send(400,"{'status':'No Update'}");
                break;

            case HTTP_UPDATE_OK:
                Serial.println("HTTP_UPDATE_OK");
                server.send(200,"{'status':'ok'}");
                break;
          }
        }
        else
          server.send(400,"text/html","<h1>HTTP 400:</h1><h3> Bad Request</h3>");    
  }
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void){
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  WiFi.begin(ssid,password);

  
// Wait for connection
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
  Serial.println("");
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.macAddress());

  server.on("/", handleRoot);

  server.on("/data",HTTP_POST, handleGPIODetais);

 server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
  digitalWrite(LED_BUILTIN,LOW);  
}
