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

//deviceName,deviceDescription,deviceMAC,deviceIP,deviceConfigParams
//POST data to Server
bool devRegPostData(String path, String devName, String devDescription, String devMAC, String devIP, String devConfigParams)
{
  HTTPClient httpPost;  
  
  httpPost.begin(DEV_REG_host + path);
  Debug("\n\n\t Host Path : ");
  Debugln(DEV_REG_host + path);
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
