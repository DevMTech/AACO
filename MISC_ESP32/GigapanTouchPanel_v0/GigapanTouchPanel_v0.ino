/*     
 *  Purpose: Navigate in GigaPixel pattachitra image via switch presses, by making POST requests 
 *  Created on: 05/02/2018
 *  Author: Debajyoti Maitra
 *  Connection: 1 common wire coming from all the 17 switches connected to the A0 pin of NodeMCU ESP12e
 */

//http://10.208.26.254:81/gigapan/index.php // COMMON - SERVER

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>

const int sensorPin = A0;    // select the input pin for sensor value
int buf = 0, switches = 17, sw=0, last_sw=0, press_count=0, adc_max = 1024;
float sensorValue = 0, s;
float unit_step = adc_max/(switches+1);

const char* ssid = "cdac";
const char* password = "";

// COMMON - SERVER
const char* host = "http://10.208.26.254:81";             //main - Server address
String post = "/gigapan/index.php";                 //main - Post service Path

// MAIN
//const char* host = "http://10.208.34.149:85";             //main - Server address
//String post = "/gigapan/demo_image1.php";                 //main - Post service Path

// LOCAL
//const char* host = "http://10.208.43.70:3000";          //local - Server address
//String post = "/gigapan";                               //local - Post service Path


//POST data to Server
bool postData(String path, int sw, float s)
{
  HTTPClient httpPost;  
  
  httpPost.begin(host + path);
  httpPost.addHeader("Content-Type","application/x-www-form-urlencoded");
  String st_sw = String(sw);
  
  Serial.println("pointID:" + st_sw + ","+ "pointsSubmit:1");
  int httpCode = httpPost.POST("pointID=" + st_sw + "&" + "pointsSubmit=1");
   
  httpPost.end();
  
  Serial.println(httpCode);  
    
  if(httpCode == 200)
  {  
    Serial.println("POST success");
    return true;
  }
  else  { return false; }    
}

void setup() 
{
  //LED setup
  pinMode(LED_BUILTIN, OUTPUT);
  
  Serial.begin(115200);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());   
  Serial.println(WiFi.macAddress());
}

void loop() 
{
    // read the value from the sensor:
  s=analogRead(sensorPin);
  Serial.print("sensorValue : ");
  Serial.print(s);
  
  
  sw=0;
  if (s>40){sw = round((s+buf)/unit_step);}
  
  if(sw>0) 
  {  Serial.print(String(" -> switch ")+(sw)+String(" pressed ")); 
     if(postData(post, sw, s))
             digitalWrite(LED_BUILTIN, HIGH);
     
     
  }
  
  Serial.println("");

  delay(200);
  digitalWrite(LED_BUILTIN, LOW);
  
    //delay(1000);
}
