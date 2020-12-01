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

//Make GET request
bool devRegGetData(String path)
{
  HTTPClient http;
  http.begin( DEV_REG_host + path);

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

//deviceName,deviceDescription,deviceMAC,deviceIP,deviceConfigParams
//POST data to Server
bool devRegPostData(String path, String devName, String devDescription, String devMAC, String devIP, String devConfigParams)
{
  HTTPClient httpPost;  
  
  httpPost.begin(DEV_REG_host + path);
  httpPost.addHeader("Content-Type","application/x-www-form-urlencoded");
  
  //int httpCode = httpPost.POST("id=" + id + "&"+ "state="+data );
  int httpCode = httpPost.POST("deviceName="+deviceName+"&"+"deviceDescription="+ deviceDescription +"&"+
                 "deviceMAC="+ devMAC +"&"+"deviceIP="+ devIP +"&"+"deviceConfigParams="+devConfigParams);
 
  httpPost.writeToStream(&Serial);
  httpPost.end();
  
  Serial.println(httpCode);  
    
  if(httpCode>0)
  {
    
    Serial.println();
    return true;
  }
  else
  {
     return false;
  }
    
}


//void setup() {
//
//  //Input mode for the GPIO5 - D1 pin on NodeMCU
//  //GPIO5 = D1
//  //pinMode(5,INPUT); // GPIO5-> D1 pin on NodeMCU
//
//  pinMode(BUZZER,OUTPUT);
//  
//  Serial.begin(115200);
//  Serial.println();
//  Serial.print("connecting to ");
//  Serial.println(ssid);
//  WiFi.begin(ssid, password);
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//    Serial.print(".");
//  }
//  Serial.println("");
//  Serial.println("WiFi connected");
//  Serial.println("IP address: ");
//  Serial.println(WiFi.localIP());
//
//  String url = "/data";
//  Serial.print("requesting URL: ");
//  Serial.println(url);
//
//  if(getData(url))
//  {
//    Serial.println("Success");     
//  }
//  else
//  {
//    Serial.println("Failure");
//  }
//
// 
//  
//}
//
//void loop() {
//
//  sensorValue = analogRead(A0);
//  Serial.println(String("sensorValue = ") + sensorValue);
//  //if(digitalRead(5)==HIGH) //GPIO5 = D1
//  Serial.println();
//  Serial.print(count++);
//  
//  if(sensorValue > threshold)  
//    { 
//      Serial.println(") Artefact is missing");
//      postData(post,"1", "Lost");
//      //digitalWrite (LED_BUILTIN, LOW); // on
//      //digitalWrite (BUZZER, LOW);
//      digitalWrite (BUZZER, HIGH);
//      //Serial.println("ON");
//      //delay(100);
//    }
//    else
//    { Serial.println(") Artifact Present");
//      postData(post,"1","Present");
//      digitalWrite (BUZZER, LOW);
//      //digitalWrite (LED_BUILTIN, HIGH); // off
//      //delay(100);
//    } 
//  if(count > 5000) count = 0;  
//  
//    
////    if(digitalRead(5)==HIGH) //GPIO5 = D1
////    {
////      Serial.println("Artefact is missing");
////      postData(post,"1", "Lost");
////    }
////    else
////    {
////      Serial.println("GPIO is OFF");
////      postData(post,"1","Present");
////    }
//
//     delay(1000);
//     digitalWrite (BUZZER, LOW);
//     //digitalWrite (BUZZER, HIGH);
//}
