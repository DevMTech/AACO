
//#include <WiFi.h>
#include <WebServer.h>

WebServer server(80); 

void WEB_SERVER_setup() 
{
//  Serial.begin(115200);
//  delay(100);
//
//  Serial.println("Connecting to ");
//  Serial.println(ssid);
//
//  //connect to your local wi-fi network
//  WiFi.begin(ssid, password);
//
//  //check wi-fi is connected to wi-fi network
//  while (WiFi.status() != WL_CONNECTED) {
//  delay(1000);
//  Serial.print(".");
//  }
//  Serial.println("");
//  Serial.println("WiFi connected..!");
//  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());

  //BME680_Simple_setup();
  
  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");

}
void WEB_SERVER_loop() 
{
  server.handleClient();
}



void handle_OnConnect() 
{
//  BME680_Simple_loop();
  server.send(200, "text/html", SendHTML(tempC,humidity,pressure,alti)); 
}

void handle_NotFound()
{
  server.send(404, "text/plain", "Not found");
}

String SendHTML(float temperature,float humidity,float pressure,float altitude)
{
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>ESP32 Weather Station</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr +="p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<div id=\"webpage\">\n";
  ptr +="<h1>ESP32 Weather Station</h1>\n";
  ptr +="<p>Temperature: ";
  ptr +=temperature;
  ptr +="&deg;C</p>";
  ptr +="<p>Humidity: ";
  ptr +=humidity;
  ptr +="%</p>";
  ptr +="<p>Pressure: ";
  ptr +=pressure;
  ptr +="hPa</p>";
  ptr +="<p>Altitude: ";
  ptr +=altitude;
  ptr +="m</p>";
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
