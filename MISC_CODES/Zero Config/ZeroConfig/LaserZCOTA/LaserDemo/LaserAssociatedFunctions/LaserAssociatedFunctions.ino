/*
 *  HTTP over TLS (HTTPS) example sketch
 *
 *  This example demonstrates how to use
 *  WiFiClientSecure class to access HTTPS API.
 *  We fetch and display the status of
 *  esp8266/Arduino project continuous integration
 *  build.
 *
 *  Created by Ivan Grokhotkov, 2015.
 *  This example is in public domain.
 */

//#include <ESP8266WiFi.h>
//#include <WiFiClientSecure.h>
//#include <ESP8266HTTPClient.h> 
//const char* ssid = "cdac";        const char* password = "";

#define BUZZER 15 // GPIO15-> D8 pin on NodeMCU

int sensorValue=0, threshold = 850;
unsigned long count = 0;

//Server address
const char* LaserHost = "http://10.42.0.1:3301"; // office laptop

//Post service Path
String LaserPost = "/artefactUpdate";

//Make GET request
bool laserGetData(String path)
{
  HTTPClient http;
  http.begin( LaserHost + path);

  int httpCode = http.GET();

  delay(100);
  http.end();
  if(httpCode == HTTP_CODE_OK)
  {
    String payload = http.getString();
    Serial.println(payload);
    return true;
  }
  else
    return false;
}


//POST data to Server
bool laserPostData(String path, String id, String data)
{
  HTTPClient httpPost;  
  
  httpPost.begin(LaserHost + path);
  httpPost.addHeader("Content-Type","application/x-www-form-urlencoded");
  
  int httpCode = httpPost.POST("id=" + id + "&"+ "state="+data );
 
  httpPost.writeToStream(&Serial);
  httpPost.end();
  
  Serial.println(httpCode);  
    
  if(httpCode>0)
  {   
    Serial.println();
    return true;
  }
  else
  {     return false;
  }    
}

void laserSetup() 
{
  //Input mode for the GPIO5 - D1 pin on NodeMCU
  //GPIO5 = D1
  //pinMode(5,INPUT); // GPIO5-> D1 pin on NodeMCU

  pinMode(BUZZER,OUTPUT);
   
}

void laserLoop() 
{

  sensorValue = analogRead(A0);
  Serial.println(String("sensorValue = ") + sensorValue);
  //if(digitalRead(5)==HIGH) //GPIO5 = D1
  Serial.println();
  Serial.print(count++);
  
  if(sensorValue > threshold)  
    { 
      Serial.println(") Artefact is missing");
      laserPostData(LaserPost,"1", "Lost");
      //digitalWrite (LED_BUILTIN, LOW); // on
      //digitalWrite (BUZZER, LOW);
      digitalWrite (BUZZER, HIGH);
      //Serial.println("ON");
      //delay(100);
    }
    else
    { Serial.println(") Artifact Present");
      laserPostData(LaserPost,"1","Present");
      digitalWrite (BUZZER, LOW);
      //digitalWrite (LED_BUILTIN, HIGH); // off
      //delay(100);
    } 
    
  if(count > 5000) count = 0;  
  
     delay(1000);
     digitalWrite (BUZZER, LOW);
     //digitalWrite (BUZZER, HIGH);
}
