/*
 *  Purpose: Navigate in GigaPix image via NodeMCU switches, as it makes POST request 
 *  Created on: 31/7/2018
 */

//http://10.208.34.149:85/gigapan/demo_image1.php

//http://10.208.26.254:81/gigapan/index.php // COMMON - SERVER

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>

//use any digital pin for input from touch switch other that D0, D4 ..
//static const uint8_t D0   = 16; // dont use for interrupts, LED_BUILTIN on this pin
//static const uint8_t D1   = 5;
//static const uint8_t D2   = 4; 
//static const uint8_t D3   = 0;
//static const uint8_t D4   = 2; // power issues, ESP12E LED on this pin 
//static const uint8_t D5   = 14;
//static const uint8_t D6   = 12;
//static const uint8_t D7   = 13;
//static const uint8_t D8   = 15;
//static const uint8_t D9   = 3;  // RX
//static const uint8_t D10  = 1;  // TX

// LAPTOP
const char* ssid = "CdacIoT";          const char* password = "cdac1234";
const char* host = "http://10.42.0.1:3304/image";             //main - Server address
volatile int i=0, j=0, k=0, switchNo=0;
// COMMON - SERVER
//const char* ssid = "cdac";          const char* password = "";
//const char* host = "http://10.208.26.254:81/gigapan/index.php";
//const char* host = "http://10.208.26.254:81";             //main - Server address
//String postPath = "/gigapan/index.php";                 //main - Post service Path


void wifiSetup()
{
  Serial.print("\n\nMAC : ");
  Serial.println(WiFi.macAddress());
  Serial.print("connecting to ");
  Serial.println(ssid);  
  WiFi.begin(ssid, password);
  delay(50);
  
  while (WiFi.status() != WL_CONNECTED) 
  { delay(10); //Serial.print("."); 
  }
  Serial.println("");

  if(WiFi.status() == WL_CONNECTED)
  {Serial.println("WiFi connected");
   Serial.println("IP address: ");
   Serial.println(WiFi.localIP());    
  }
//  else
//  { Serial.println("Could not connect to WiFi ..");// Trying again ..");
//  }   
//  delay(100);
}

//POST data to Server
bool postData(int sw)
{
  HTTPClient httpPost;  
  
  httpPost.begin(host);// + path);
  httpPost.addHeader("Content-Type","application/x-www-form-urlencoded");
  String st_sw = String(sw);
  
//  Serial.println("\nbuttonID:"+ st_sw);  // + ","+ "pointsSubmit:1");
  int httpCode = httpPost.POST("buttonID=" + st_sw);
//  int httpCode = httpPost.POST("pointID=" + st_sw +"&"+ "pointsSubmit=1"); // st_sw); // + "&" + "pointsSubmit=1");
//  int httpCode = httpPost.POST("pointID=" + st_sw + "&"+"pointValue=" + st_s + "&"+"pointsSubmit=1");
 
  //httpPost.writeToStream(&Serial);
  httpPost.end();
  
  Serial.println(httpCode);  
    
  if(httpCode == 200)
  {  
//    Serial.println("POST SUCCESS!\n\n");
    return true;
  }
  else  { return false; }    
}

//void IntCallback_sw1()
//{ switch_seq(1);
//}
//
//void IntCallback_sw2()
//{ switch_seq(2);
//}
//
//void IntCallback_sw3()
//{ switch_seq(3);
//}

void setup() 
{ //Open serial communications and wait for port to open:
  Serial.begin(115200); delay(50);
  
//  pinMode(D1, INPUT); // GPIO5 = D1 = sw1
//  pinMode(D2, INPUT); // GPIO4 = D2 = sw2
//  pinMode(10, INPUT); // GPIO0 = D3 = sw3
//  pinMode(LED_BUILTIN, OUTPUT);

//  attachInterrupt(digitalPinToInterrupt(D1), IntCallback_sw1, RISING);
//  attachInterrupt(digitalPinToInterrupt(D2), IntCallback_sw2, RISING);
//  attachInterrupt(digitalPinToInterrupt(10), IntCallback_sw3, RISING);

  wifiSetup();
  while (!Serial) {;} // wait for serial port to connect. Needed for native USB port only
}

//void switch_seq(int sw)
//{ if(sw)
//  { Serial.print(String(" -> ")+sw);
//    if(postData(sw))Serial.print(" POST SUCCESS !");  
//  }
//}

void loop() 
{ 
//  if(postData(1))Serial.print(" POST SUCCESS !"); delay(2500);
  
  if (Serial.available())
  {   int ch = Serial.read(); //    int ch = (int)c;

    if(ch!=10)
    { if(ch>47 && ch<58) 
        { switchNo*=10; switchNo+=ch-48; //          digCount++; 
//          Serial.println(" ch = "+String(ch));   //UNCOMMENT FOR DEBUGGING
        }
//          msg[j++]=c; // Serial.print(" j = "+String(j));
     }
     else 
     {  
//        Serial.println("Switch No. Received : "+String(switchNo)); //UNCOMMENT FOR DEBUGGING

        if(postData(switchNo)) 
          { //Serial.println(" POST SUCCESS !"); 
            delay(10);              
          }
        else
        { if(WiFi.status() != WL_CONNECTED) {   wifiSetup();    }
        } 
        
        j=0; switchNo=0; k=0;
      }
  }
  
}

