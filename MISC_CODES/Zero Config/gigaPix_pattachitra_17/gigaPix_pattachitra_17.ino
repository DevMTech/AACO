/*
 *  Purpose: Navigate in GigaPix image via NodeMCU switches, as it makes POST request 
 *  Created on: 18/1/2018
 */

//http://10.208.34.149:85/gigapan/demo_image1.php

//http://10.208.26.254:81/gigapan/index.php // COMMON - SERVER

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>

const int sensorPin = A0;    // select the input pin for sensor value
int buf = 0, switches = 17, sw=0, i=0, a, b, last_sw=0, press_count=0, adc_max = 1024;
float sensorValue = 0, s, t, s_map;
float unit_step = adc_max/(switches+1);
//float map_max = adc_max, unit_step = map_max/(switches+1);

const char* ssid = "cdac";
const char* password = "";

// COMMON - SERVER
const char* host = "http://10.208.26.254:81";             //main - Server address
String post = "/gigapan/index.php";                       //main - Post service Path

// MAIN
//const char* host = "http://10.208.34.149:85";             //main - Server address
//String post = "/gigapan/demo_image1.php";                 //main - Post service Path

// LOCAL
//const char* host = "http://10.208.43.70:3000";          //local - Server address
//String post = "/gigapan";                               //local - Post service Path



/*bool REQUEST_MADE = false;

typedef struct {
  int pointID;
} gigaPixelLocation;

gigaPixelLocation point;*/

//POST data to Server
bool postData(String path, int sw, float s)
{
  HTTPClient httpPost;  
  
  httpPost.begin(host + path);
  httpPost.addHeader("Content-Type","application/x-www-form-urlencoded");
  String st_sw = String(sw);
  //String st_s = String(s);
  
  Serial.println("pointID:" + st_sw + ","+ "pointsSubmit:1");
  int httpCode = httpPost.POST("pointID=" + st_sw + "&" + "pointsSubmit=1");
  //int httpCode = httpPost.POST("pointID=" + st_sw + "&"+"pointValue=" + st_s + "&"+"pointsSubmit=1");
 
  //httpPost.writeToStream(&Serial);
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
  //s_map = map(s, 0, adc_max, 0, map_max);
  
  sw=0;
  if (s>40){sw = round((s+buf)/unit_step);}
  //if (s_map>20){sw = round((s_map)/unit_step);}
  if(sw>0) 
  {   //point.pointID = sw;
      //Serial.print(String(" s_map -> ")+(s_map)+String(" -> "));
      Serial.print(String(" -> switch ")+(sw)+String(" pressed ")); 
     if(postData(post, sw, s))
     //if(postData(post, sw, s_map))
        digitalWrite(LED_BUILTIN, HIGH);
     
     
  }
  /*if(s>240 && s<260) Serial.print(String("sensorValue : ")+s+String(" -> switch 1 "));*/

  Serial.println("");

  delay(200);
  digitalWrite(LED_BUILTIN, LOW);
   /* 
  if(!REQUEST_MADE)
      {
        postData(post,point.sw);
        REQUEST_MADE = true;
      }  
     
    if(digitalRead(GPIOA)==HIGH)
          gpioHigh(PointA);      
    else
          REQUEST_MADE = false;*/

    //delay(1000);
}
